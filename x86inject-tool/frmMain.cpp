
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


	}


	inline System::Void frmMain::m_btn_inject_Click(System::Object^ sender, System::EventArgs^ e) {

		if (0 == target_process().id) {
			return;
		}

		//
		m_txt_allocAddr->Text = ""; m_txt_retValue->Text = "";

		//
		blackbone::Process proc;
		NTSTATUS ntStatus = proc.Attach(target_process().id);
		if (!NT_SUCCESS(ntStatus)) {
			MessageBox::Show(String::Format("{0} attach failed", target_process().name), "error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		// 
		auto memBlock = proc.memory().Allocate(4096, PAGE_EXECUTE_READWRITE, 0, false);
		if (!memBlock.success()) {
			MessageBox::Show(String::Format("allocate failed"), "error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		//
		ptr_t codeAddr = memBlock->ptr();

		
		if (0 == codeAddr) return;
		m_txt_allocAddr->Text = String::Format("{0:X16}", codeAddr);


		// 
		List<String^>^ asmCodes = gcnew List<String^>();
		List<array<Byte>^>^ asmValues = gcnew List<array<Byte>^>();

		// 
		StringReader reader(m_txt_assembler->Text);
		while (1) {
			String^ s = reader.ReadLine();
			if (nullptr == s) break;
			if (s->Length > 1) asmCodes->Add(s);
		}

		// 
		clsAssembleJit asmJit;
		for (int i = 0; i < asmCodes->Count; i++) {
			array<Byte>^ asmCode = asmJit.assembler(asmCodes[i], UIntPtr(codeAddr), target_process().is64);
			if (!asmCode->Length) {
				MessageBox::Show(String::Format("line {0}: {1}", i + 1, asmJit.last_error), "error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			asmValues->Add(asmCode);
		}

		// 
		uint32_t iCount = 0;
		for each (auto asmValue in asmValues)
		{
			for each (auto c in asmValue)
			{
				memBlock->Write(iCount, c); ++iCount;
			}
		}

		// 
		if (m_chk_exec->Checked) {
			auto fnProcesser = blackbone::MakeRemoteFunction<void(*)(void)>(proc, codeAddr);
			ptr_t r = fnProcesser().result();
			memBlock->Free();

			//
			m_txt_retValue->Text = String::Format("{0:X16}", r);
		}

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