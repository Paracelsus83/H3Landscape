// SPDX-FileCopyrightText: 2026 Paracelsus83
// SPDX-License-Identifier: MIT

#include "Win32EXE_Lib/exe_file.hpp"
#include <WinBase.h>
#include <WinUser.h>

namespace {

	static void MsgBox(LPCSTR text, UINT type) {
		MessageBoxA(NULL, text, "H3Landscape HotA Patcher", type | MB_SYSTEMMODAL);
	}

	static void LoadErrorMsgBox(EXEFile::LoadResult r) {
		LPCSTR errMsg;
		switch (r) {
		case EXEFile::LoadResult::CANT_OPEN:
			errMsg = "Error: The `H3HotA HD.exe` file cannot be opened.";
			break;
		case EXEFile::LoadResult::READ_ERROR:
			errMsg = "An error occurred while reading the `H3HotA HD.exe` file.";
			break;
		case EXEFile::LoadResult::CORRUPTED:
			errMsg = "Error: The `H3HotA HD.exe` file is corrupted.";
			break;
		case EXEFile::LoadResult::UNSUPPORTED:
			errMsg = "Error: Unknown version of the `H3HotA HD.exe` file.";
			break;
		default:
			errMsg = "";
		}

		std::string fullMsg;
		fullMsg.reserve(220);
		fullMsg.assign(errMsg);
		fullMsg.append("\n\n"
			"The H3Landscape Patcher is unable to create the `H3HotA_HD_L.exe' executable file.\n"
			"You can not launch `Horn of the Abyss' with the H3Landscape add-on."
		);
		MsgBox(fullMsg.c_str(), MB_ICONERROR);
	}

} // namespace


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR /*lpCmdLine*/, _In_ int /*nShowCmd*/) {

	WCHAR exePath[MAX_PATH];

	const DWORD pathLen = GetModuleFileNameW(hInstance, exePath, MAX_PATH);
	if (pathLen > 0) {
		WCHAR* lastBackslash = wcsrchr(exePath, '\\');
		if (lastBackslash) {
			*lastBackslash = '\0';
			SetCurrentDirectoryW(exePath);
		}
	}

	EXEFile exeFile;
	auto r1 = exeFile.Load("h3hota HD.exe");
	if (r1 != EXEFile::LoadResult::OK) {
		auto r2 = exeFile.Load("h3hota_HD.exe");
		if (r2 != EXEFile::LoadResult::OK) {
			if (r1 == EXEFile::LoadResult::CANT_OPEN && r2 == EXEFile::LoadResult::CANT_OPEN) {
				MsgBox("No `Horn of the Abyss' installation detected\n"
					"(the file `h3hota HD.exe' cannot be found).\n\n"
					"If you install HotA, run the `H3L_HotA_Patcher.exe' to be able to play HotA with the H3Landscape add-on.",
					MB_ICONASTERISK);
			}
			else {
				LoadErrorMsgBox(r1);
			}
			return EXIT_FAILURE;
		}
	}

	DWORD initFuncAddr = exeFile.AddDllImport("_HD3_Data\\Packs\\Landscape\\Landscape.dll", "H3L_Init");
	if (initFuncAddr) {
		BYTE initOpcodes[] = {
			0xFF, 0x15, 0, 0, 0, 0, // call [ptr]
			0x58 | 7, // pop EDI
			0x58 | 6, // pop ESI
		};
		memcpy(initOpcodes + 2, &initFuncAddr, sizeof(initFuncAddr));
		exeFile.Patch(0x4F824A, initOpcodes);

		ResourceParser parser = exeFile.GetResourceParser();
		if (parser) {
			parser.UpdateVersion({ 3, 3, 0, 0 }, {}, {
				{ L"FileVersion", L"3.3" },
				{ L"ProductName", L"HoM&M\u00AE III: Horn of the Abyss [+ H3Landscape mod]" },
			});
		}
		if (exeFile.Save("H3HotA_HD_L.exe")) {
			MsgBox("The H3Landscape add-on for `Horn of the Abyss' has been successfully installed.\n\n"
				"To launch Heroes 3 HotA with the H3Landscape add-on, use the `H3HotA_HD_L.exe' file.",
				MB_ICONASTERISK);
			return EXIT_SUCCESS;
		}
		else {
			MsgBox("Error: The file `H3HotA_HD_L.exe' could not be created.\n\n"
				"You can not launch `Horn of the Abyss' with the H3Landscape add-on.", MB_ICONERROR);
		}
	}
	else {
		LoadErrorMsgBox(EXEFile::LoadResult::UNSUPPORTED);
	}

	return EXIT_FAILURE;
}
