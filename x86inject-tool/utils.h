#pragma once


namespace x86injecttool {

	using namespace System;
	using namespace System::Collections::Generic;

	typedef size_t ptr_t;


	value struct module_info {
		UIntPtr base;
		UInt64 size;
		String^ path;
	};

	value struct process_info {
		Boolean used;
		UInt32 id;
		String^ title;
		String^ name;
		String^ path;
		Boolean is64;
	};


	typedef List<process_info> processes;
	typedef List<module_info> modules;

}