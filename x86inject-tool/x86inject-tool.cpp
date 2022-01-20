#include "pch.h"

using namespace System;
using namespace System::Windows::Forms;

#include "frmMain.h"


#include <filesystem>

int main(array<System::String ^> ^args)
{
    Application::EnableVisualStyles();
    Application::Run(gcnew x86injecttool::frmMain());

    return 0;
}
