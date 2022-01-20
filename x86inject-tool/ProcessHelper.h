#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <msclr\marshal.h>

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;


#include <fstream>
#include <filesystem>
namespace fs {
	using namespace std::filesystem;
	using ifstream = std::ifstream;
	using ofstream = std::ofstream;
	using fstream = std::fstream;
}


#include "utils.h"


namespace x86injecttool {

	ref struct clsProcessHelper {

		// https://docs.microsoft.com/en-us/windows/win32/procthread/process-security-and-access-rights
		static \
		const uint32_t process_access() {
			return PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE \
				| PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD;
		}


		// https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#optional-header-image-only
		static \
		inline bool is64exec(fs::path exec) {
			if (!fs::exists(exec)) return false;
			
			std::string content(1024, '\x00');
			fs::ifstream(exec, std::ios::binary).read(content.data(), content.length());

			const uintptr_t buffer = uintptr_t(content.data());

			PIMAGE_NT_HEADERS inth = reinterpret_cast<PIMAGE_NT_HEADERS>(buffer + *reinterpret_cast<PDWORD>(buffer + 0x3c));

			return inth->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC;
		}


		// https://docs.microsoft.com/zh-cn/windows/win32/psapi/enumerating-all-processes
		static \
		processes^ enum_processes() {
			DWORD aProcesses[1024] = { };
			DWORD cbNeeded = 0, cProcesses = 0;

			// https://docs.microsoft.com/zh-cn/windows/win32/api/psapi/nf-psapi-enumprocesses
			if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
				return gcnew processes();

			//
			auto arrProcesses = gcnew processes();

			// Calculate how many process identifiers were returned.

			cProcesses = cbNeeded / sizeof(DWORD);

			// Print the name and process identifier for each process.

			for (size_t i = 0; i < cProcesses; i++)
			{
				if (aProcesses[i] != 0)
				{
					DWORD dwSize = MAX_PATH;
					WCHAR szExeName[MAX_PATH] = { };

					HANDLE hProcess = OpenProcess(clsProcessHelper::process_access(), FALSE, aProcesses[i]);

					if (aProcesses[i] != GetCurrentProcessId() && NULL != hProcess && QueryFullProcessImageNameW(hProcess, 0, szExeName, &dwSize)) {
						arrProcesses->Add({
							true,
							aProcesses[i],
							"", IO::Path::GetFileName(marshal_as<String^>(szExeName)), marshal_as<String^>(szExeName),
							clsProcessHelper::is64exec(szExeName)
						});
					}
					CloseHandle(hProcess);
				}
			}
			return arrProcesses;
		}



		const auto process_handle() { return _process_handle; }
		const auto process_id() { return GetProcessId(HANDLE(process_handle())); }


		~clsProcessHelper() {
			CloseHandle(HANDLE(process_handle()));
		}

		clsProcessHelper(uint32_t processId) {
			_process_handle = ptr_t(
				OpenProcess(clsProcessHelper::process_access(), FALSE, processId)
			);
		}

		// https://docs.microsoft.com/zh-cn/windows/win32/psapi/enumerating-all-modules-for-a-process
		modules^ enumModules() {
			HMODULE hMods[1024] = { };
			DWORD cbNeeded = 0;
			HANDLE hProcess = HANDLE(process_handle());

			// https://docs.microsoft.com/zh-cn/windows/win32/api/psapi/nf-psapi-enumprocessmodulesex
			if (NULL == hProcess || !EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_DEFAULT))
				return gcnew modules();

			//
			auto arrModules = gcnew modules();

			for (size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				WCHAR szModName[MAX_PATH] = { };
				if (!GetModuleFileNameExW(hProcess, hMods[i], szModName, MAX_PATH)) 
					continue;

				MODULEINFO moduleInfo = { };
				if (!GetModuleInformation(hProcess, hMods[i], &moduleInfo, sizeof(MODULEINFO)))
					continue;

				arrModules->Add({
					UIntPtr(moduleInfo.lpBaseOfDll),
					moduleInfo.SizeOfImage,
					marshal_as<String^>(szModName)
				});
			}

			return arrModules;
		}


		private:
			ptr_t _process_handle = 0;
	};

}
