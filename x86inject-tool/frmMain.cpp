
#define NOMINMAX

// https://github.com/x64dbg/asmjit_xedparse
#include <XEDParse/XEDParse.h>
#ifndef _WIN64
#pragma comment(lib, "../third_party/XEDParse/XEDParse_x86.lib")
#else
#pragma comment(lib, "../third_party/XEDParse/XEDParse_x64.lib")
#endif


// https://github.com/DarthTon/Blackbone
#include <BlackBone/Process/Process.h>
#include <BlackBone/Process/RPC/RemoteFunction.hpp>



#include "frmMain.h"

#include "frmProcesses.h"

#include "ProcessHelper.h"



namespace x86injecttool {

	using namespace System::IO;
	using namespace System::Diagnostics;
	using namespace System::Text::RegularExpressions;

	public ref struct clsAssembleJit
	{
	public:
		property String^ last_error;

		array<Byte>^ assembler(String^ content, UIntPtr baseAddr, Boolean is64) {
			bool isOk = false;

			XEDPARSE parse = { };

			auto instr = Marshal::StringToCoTaskMemAnsi(content);
			memcpy(parse.instr, instr.ToPointer(), content->Length);
			Marshal::FreeCoTaskMem(instr);

			// https://docs.microsoft.com/zh-cn/cpp/build/reference/base-base-address
			parse.x64 = is64;
			parse.cip = UIntPtr::Zero != baseAddr ? baseAddr.ToUInt64() : (is64 ?  0x140000000 : 0x400000);

			parse.cbUnknown = nullptr;

			isOk = XEDPARSE_STATUS::XEDPARSE_OK == XEDParseAssemble(&parse);

			last_error = gcnew String(parse.error);

			if (!isOk) return gcnew array<Byte>{ };

			array<Byte>^ codes = gcnew array<Byte>(parse.dest_size);
			for (unsigned i = 0; i < parse.dest_size; i++) {
				codes[i] = parse.dest[i];
			}

			return codes;
		}
	};



	inline System::Void frmMain::frmMain_Load(System::Object^ sender, System::EventArgs^ e) {

		return;
	}


	inline System::Void frmMain::m_btn_inject_Click(System::Object^ sender, System::EventArgs^ e) {

		if (0 == target_process().id) {
			return;
		}

		//
		m_txt_allocAddr->Text = ""; m_txt_retValue->Text = "";


		// 
		List<String^>^ asmCodes = gcnew List<String^>();

		// 
		StringReader reader(m_txt_assembler->Text);
		while (1) {
			String^ s = reader.ReadLine();
			if (nullptr == s) break;
			if (s->Length > 1) asmCodes->Add(s);
		}
		
		//
		if (0 == asmCodes->Count) return;


		//
		blackbone::Process proc;
		NTSTATUS ntStatus = proc.Attach(target_process().id);
		if (!NT_SUCCESS(ntStatus)) {
			MessageBox::Show(String::Format("{0} attach failed", target_process().name), "error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}


		// 
		auto stringsBlock = proc.memory().Allocate(4096, PAGE_READWRITE, 0, false);
		auto codeBlock = proc.memory().Allocate(4096, PAGE_EXECUTE_READWRITE, 0, false);
		if (!codeBlock.success() || !stringsBlock.success()) {
			MessageBox::Show(String::Format("allocate failed"), "error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		//
		ptr_t stringsAddr = stringsBlock->ptr(); ptr_t stringsOffset = 0;
		ptr_t codeAddr = codeBlock->ptr();  ptr_t retValue = 0;


		//
		List<array<Byte>^>^ asmValues = gcnew List<array<Byte>^>();

		// 
		clsAssembleJit asmJit;
		for (int i = 0; i < asmCodes->Count; i++) {
			String^ asmContent = asmCodes[i];
			{
				array<Byte>^ arrbytes = nullptr;
				System::Text::Encoding^ coder = System::Text::Encoding::Default;

				// https://docs.microsoft.com/zh-cn/cpp/c-language/c-comments 
				// ( "/* */" or "//" )
				asmContent = Regex::Replace(asmContent, "(/\\*.*\\*/)|(//.*)", String::Empty);

				// https://docs.microsoft.com/zh-cn/cpp/c-language/c-string-literals
				// https://zh.cppreference.com/w/cpp/language/string_literal
				// ( "" or L"" or u8"" )
				String^ ss = Regex::Match(asmContent, "[\"|L\"|u8\"].*\"")->Value;

				if (ss->StartsWith("\"")) {
					coder = System::Text::Encoding::Default;
					arrbytes = coder->GetBytes(ss->Substring(1, ss->Length - 2));
				}
				if (ss->StartsWith("L\"")) {
					auto coder = System::Text::Encoding::Unicode;
					arrbytes = coder->GetBytes(ss->Substring(2, ss->Length - 3));
				}
				if (ss->StartsWith("u8\"")) {
					auto coder = System::Text::Encoding::UTF8;
					arrbytes = coder->GetBytes(ss->Substring(3, ss->Length - 4));
				}

				if (arrbytes) {
					//
					IntPtr pptr = Marshal::AllocCoTaskMem(arrbytes->Length);
					Marshal::Copy(arrbytes, 0, pptr, arrbytes->Length);
					stringsBlock->Write(stringsOffset, arrbytes->Length, pptr.ToPointer());
					Marshal::FreeCoTaskMem(pptr);
					//
					asmContent = asmContent->Replace(ss, String::Format("0x{0:X}", stringsAddr + stringsOffset));
					stringsOffset += arrbytes->Length; stringsOffset += 4;
				}
			}

			array<Byte>^ asmCode = asmJit.assembler(asmContent, UIntPtr(codeAddr), target_process().is64);
			if (!asmCode->Length) {
				MessageBox::Show(String::Format("line {0}: {1}", i + 1, asmJit.last_error), "error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				goto _cleanup;
			}
			asmValues->Add(asmCode);
		}

		// 
		uint32_t iCount = 0;
		for each (auto asmValue in asmValues) {
			for each (auto c in asmValue) {
				codeBlock->Write(iCount, c); ++iCount;
			}
		}

		//
		m_txt_allocAddr->Text = String::Format("{0:X16}", codeAddr);


		// 
		if (!m_chk_exec->Checked) return;

		//
		{
			auto fnProcesser = blackbone::MakeRemoteFunction<void(*)(void)>(proc, codeAddr);
			retValue = fnProcesser().result();
		}

		//
		m_txt_retValue->Text = String::Format("{0:X16}", retValue);


	_cleanup:
		stringsBlock->Free(); codeBlock->Free();
		return;
	}


	inline System::Void frmMain::m_btn_processes_Click(System::Object^ sender, System::EventArgs^ e) {

		x86injecttool::frmProcesses processes;
		processes.ShowDialog();
		_target_process = processes.target_process();


		m_txt_processid->Text = String::Format("{0}", target_process().id);
		m_txt_processname->Text = target_process().name;

		return;
	}


}