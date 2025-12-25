#include "AT4RE_Lib/pefile.hpp"


static BOOL FileExists(LPCSTR path) {
	DWORD attrib = GetFileAttributesA(path);
	return attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
}


static void MsgBox(LPCSTR text, UINT type = MB_ICONASTERISK) {
	MessageBoxA(NULL, text, "H3Landscape HotA Patcher", type | MB_SYSTEMMODAL);
}


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR /*lpCmdLine*/, _In_ int /*nShowCmd*/) {

	WCHAR exePath[MAX_PATH];

	DWORD pathLen = GetModuleFileNameW(hInstance, exePath, MAX_PATH);
	if (pathLen > 0) {
		WCHAR* lastBackslash = wcsrchr(exePath, '\\');
		if (lastBackslash) {
			*lastBackslash = '\0';
			SetCurrentDirectoryW(exePath);
		}
	}

	LPCSTR hotaExeFileName = "h3hota HD.exe";
	if (!FileExists(hotaExeFileName)) {
		hotaExeFileName = "h3hota_HD.exe";
		if (!FileExists(hotaExeFileName)) {
			LPCSTR message = FileExists("h3hota.exe") ?
				"`Horn of the Abyss' installation detected, but neither the `h3hota HD.exe' nor the `h3hota_HD.exe' file can be found.\n\n"
				"Create the file `h3hota_HD.exe' using the `HD_Launcher.exe' application, then run the `H3L_HotA_Patcher.exe'."
				:
				"No `Horn of the Abyss' installation detected\n(the file `h3hota.exe' cannot be found).\n\n"
				"If you install HotA, run the `H3L_HotA_Patcher.exe' to be able to play HotA with the H3Landscape add-on.";
			MsgBox(message);
			return EXIT_SUCCESS;
		}
	}

	PEFile pe;
	if (pe.loadFromFile(hotaExeFileName)) {
		const char* functions[] = { "ImgLoaderPatch" };
		pe.addImport("_HD3_Data\\Packs\\Landscape\\Landscape.dll", functions, 1);

		// Save the modified .exe file
		if (pe.saveToFile("H3HotA_HD_L.exe")) {
			MsgBox("The H3Landscape add-on for `Horn of the Abyss' has been successfully installed.\n\nTo launch Heroes 3 HotA with the H3Landscape add-on, use the `H3HotA_HD_L.exe' file.");
		}
		else {
			MsgBox("Error: The file `H3HotA_HD_L.exe' could not be created.\n\nYou can not launch `Horn of the Abyss' with the H3Landscape add-on.", MB_ICONERROR);
		}
	}
	else {
		MsgBox("Failed to load file `h3hota_HD.exe'.\n\nThe file `H3HotA_HD_L.exe', which launches `Horn of the Abyss' with the H3Landscape add-on, has not been created.", MB_ICONERROR);
	}

	return EXIT_SUCCESS;
}
