#pragma once

#include "utils.h"

namespace x86injecttool {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// frmMain 摘要
	/// </summary>
	public ref class frmMain : public System::Windows::Forms::Form
	{
	public:
		frmMain(void)
		{
			InitializeComponent();
			//
			//TODO:  在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~frmMain()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	protected:

	private: System::Windows::Forms::Button^ m_btn_processes;
	private: System::Windows::Forms::Button^ m_btn_inject;
	private: System::Windows::Forms::RichTextBox^ m_txt_assembler;
	private: System::Windows::Forms::CheckBox^ m_chk_exec;

	private: System::Windows::Forms::TextBox^ m_txt_allocAddr;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ m_txt_processid;
	private: System::Windows::Forms::TextBox^ m_txt_processname;





	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要修改
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(frmMain::typeid));
			this->m_btn_processes = (gcnew System::Windows::Forms::Button());
			this->m_btn_inject = (gcnew System::Windows::Forms::Button());
			this->m_txt_assembler = (gcnew System::Windows::Forms::RichTextBox());
			this->m_chk_exec = (gcnew System::Windows::Forms::CheckBox());
			this->m_txt_allocAddr = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->m_txt_processid = (gcnew System::Windows::Forms::TextBox());
			this->m_txt_processname = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// m_btn_processes
			// 
			this->m_btn_processes->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"m_btn_processes.Image")));
			this->m_btn_processes->Location = System::Drawing::Point(21, 12);
			this->m_btn_processes->Name = L"m_btn_processes";
			this->m_btn_processes->Size = System::Drawing::Size(45, 42);
			this->m_btn_processes->TabIndex = 1;
			this->m_btn_processes->UseVisualStyleBackColor = true;
			this->m_btn_processes->Click += gcnew System::EventHandler(this, &frmMain::m_btn_processes_Click);
			// 
			// m_btn_inject
			// 
			this->m_btn_inject->Location = System::Drawing::Point(658, 60);
			this->m_btn_inject->Name = L"m_btn_inject";
			this->m_btn_inject->Size = System::Drawing::Size(179, 37);
			this->m_btn_inject->TabIndex = 2;
			this->m_btn_inject->Text = L"注入代码";
			this->m_btn_inject->UseVisualStyleBackColor = true;
			this->m_btn_inject->Click += gcnew System::EventHandler(this, &frmMain::m_btn_inject_Click);
			// 
			// m_txt_assembler
			// 
			this->m_txt_assembler->Location = System::Drawing::Point(21, 60);
			this->m_txt_assembler->Name = L"m_txt_assembler";
			this->m_txt_assembler->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::ForcedVertical;
			this->m_txt_assembler->Size = System::Drawing::Size(627, 646);
			this->m_txt_assembler->TabIndex = 3;
			this->m_txt_assembler->Text = L"";
			// 
			// m_chk_exec
			// 
			this->m_chk_exec->AutoSize = true;
			this->m_chk_exec->CheckAlign = System::Drawing::ContentAlignment::BottomLeft;
			this->m_chk_exec->Checked = true;
			this->m_chk_exec->CheckState = System::Windows::Forms::CheckState::Checked;
			this->m_chk_exec->Location = System::Drawing::Point(778, 103);
			this->m_chk_exec->Name = L"m_chk_exec";
			this->m_chk_exec->Size = System::Drawing::Size(59, 19);
			this->m_chk_exec->TabIndex = 4;
			this->m_chk_exec->Text = L"执行";
			this->m_chk_exec->UseVisualStyleBackColor = true;
			// 
			// m_txt_allocAddr
			// 
			this->m_txt_allocAddr->Location = System::Drawing::Point(658, 681);
			this->m_txt_allocAddr->Name = L"m_txt_allocAddr";
			this->m_txt_allocAddr->ReadOnly = true;
			this->m_txt_allocAddr->Size = System::Drawing::Size(179, 25);
			this->m_txt_allocAddr->TabIndex = 5;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(658, 652);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(135, 15);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Allocated Addr :";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(100, 26);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(87, 15);
			this->label2->TabIndex = 7;
			this->label2->Text = L"ProcessId:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(368, 26);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(103, 15);
			this->label3->TabIndex = 8;
			this->label3->Text = L"ProcessName:";
			// 
			// m_txt_processid
			// 
			this->m_txt_processid->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->m_txt_processid->Location = System::Drawing::Point(193, 26);
			this->m_txt_processid->Name = L"m_txt_processid";
			this->m_txt_processid->ReadOnly = true;
			this->m_txt_processid->Size = System::Drawing::Size(141, 18);
			this->m_txt_processid->TabIndex = 9;
			this->m_txt_processid->TabStop = false;
			// 
			// m_txt_processname
			// 
			this->m_txt_processname->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->m_txt_processname->Location = System::Drawing::Point(477, 26);
			this->m_txt_processname->Name = L"m_txt_processname";
			this->m_txt_processname->ReadOnly = true;
			this->m_txt_processname->Size = System::Drawing::Size(360, 18);
			this->m_txt_processname->TabIndex = 10;
			this->m_txt_processname->TabStop = false;
			// 
			// frmMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(849, 730);
			this->Controls->Add(this->m_txt_processname);
			this->Controls->Add(this->m_txt_processid);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->m_txt_allocAddr);
			this->Controls->Add(this->m_chk_exec);
			this->Controls->Add(this->m_txt_assembler);
			this->Controls->Add(this->m_btn_inject);
			this->Controls->Add(this->m_btn_processes);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"frmMain";
			this->Text = L"代码注入器";
			this->Load += gcnew System::EventHandler(this, &frmMain::frmMain_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: const auto target_process() { return _target_process; };
	private: process_info _target_process;

	private: System::Void m_btn_processes_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void frmMain_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void m_btn_inject_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
