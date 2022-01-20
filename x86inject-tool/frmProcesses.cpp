
#include "frmProcesses.h"

#include "ProcessHelper.h"


namespace x86injecttool {


	inline System::Void frmProcesses::frmProcesses_Load(System::Object^ sender, System::EventArgs^ e) {
		return m_btn_refresh_Click(nullptr, nullptr);
	}

	inline System::Void frmProcesses::m_btn_refresh_Click(System::Object^ sender, System::EventArgs^ e) {

		m_processes->Items->Clear();

		for each (auto p in clsProcessHelper::enum_processes()) {
			//
			if (!p.used) continue;
			//
			auto item = m_processes->Items->Add(String::Format("{0:X8}[{1}]", p.id, p.id));
			item->SubItems->AddRange(gcnew array<String^>{ String::Format("{0}{1}", p.name, p.is64 ? "" : " *32"), p.title, p.path });
			//
			if (p.used) item->Tag = p;
		}
	}

	inline System::Void frmProcesses::m_processes_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {

		m_modules->Items->Clear();

		if (1 != m_processes->SelectedItems->Count || !m_processes->SelectedItems[0]->Tag) return;

		process_info^ proc = static_cast<process_info^>(m_processes->SelectedItems[0]->Tag);
		if (128 > proc->id) return;

		for each (auto m in clsProcessHelper(proc->id).enumModules()) {
			auto item = m_modules->Items->Add(String::Format("{0:X16}", m.base.ToUInt64()));
			item->SubItems->AddRange(gcnew array<String^>{ String::Format("{0:X8}", m.size), m.path });
		}
	}

	inline System::Void frmProcesses::m_processes_DoubleClick(System::Object^ sender, System::EventArgs^ e) {

		if (1 != m_processes->SelectedItems->Count || !m_processes->SelectedItems[0]->Tag) return;

		process_info^ proc = static_cast<process_info^>(m_processes->SelectedItems[0]->Tag);
		if (128 > proc->id) return;

		_target_process = process_info(proc);

		Close();
	}

}