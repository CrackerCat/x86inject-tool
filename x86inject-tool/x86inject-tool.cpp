#include "pch.h"
#include "frmMain.h"

#ifndef _DEBUG
// https://docs.microsoft.com/en-us/cpp/build/reference/entry-entry-point-symbol
#pragma comment( linker, "/subsystem:windows /entry:main" )
#endif


int main(array<System::String ^> ^args)
{
    x86injecttool::Application::EnableVisualStyles();
    x86injecttool::Application::Run(gcnew x86injecttool::frmMain());

    return 0;
}
