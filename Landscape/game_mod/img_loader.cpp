#include <nh3api/core/resources/files.hpp>

#include "img_loader.hpp"


namespace ImgLdrAddr {
	constexpr uintptr_t ENTRY_POINT = 0x55ABFB;

    constexpr uintptr_t NO_IMAGE = 0x55AC00;
    constexpr uintptr_t LOAD_IMAGE = 0x55ACA6;

    constexpr uintptr_t FUNC_FIND_IN_LOD = 0x4FB100;
}


static char fullLodPath[MAX_PATH];
static LODFile bgLodFile;


static void __declspec(naked) CheckBgLodFile() {
    if (!bgLodFile.fileptr) {
        bgLodFile.open(fullLodPath, 1);
        _asm {
            test eax, eax
            jne fail_end
        }
    }
    __asm {
        mov  esi, OFFSET bgLodFile
        mov  ecx, esi
        call ImgLdrAddr::FUNC_FIND_IN_LOD
        test al, al
        je   fail
        jmp  ImgLdrAddr::LOAD_IMAGE // go to loading pcx
    fail:
        push ebx
    fail_end:
        mov  edx, 10h
        jmp  ImgLdrAddr::NO_IMAGE  // go to warning mesage
    }
}


void ImgLoaderPatch(PatcherInstance& p, HMODULE hModule) {
	const char defaultLodPath[] = "_HD3_Data\\Packs\\Backgrounds\\Backgrounds.lod";

    size_t pathLen = GetModuleFileName(hModule, fullLodPath, sizeof(fullLodPath));

    if ((pathLen > 3) && (pathLen < MAX_PATH)) {
		// Replace extension "dll" -> "lod"
		memcpy(fullLodPath + pathLen - 3, "lod", 3);
    }
    else {
		memcpy(fullLodPath, defaultLodPath, sizeof(defaultLodPath));
    }
	
    p.WriteJmp(ImgLdrAddr::ENTRY_POINT, uintptr_t(CheckBgLodFile));
}
