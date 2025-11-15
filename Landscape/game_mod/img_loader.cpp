#include <nh3api/core/resources/files.hpp>

#include "img_loader.hpp"


namespace ImgLdrAddr {
	constexpr uintptr_t ENTRY_POINT = 0x55ABFB;

    constexpr uintptr_t NO_IMAGE = 0x55AC00;
    constexpr uintptr_t LOAD_IMAGE = 0x55ACA6;

    constexpr uintptr_t FUNC_FIND_IN_LOD = 0x4FB100;
}

namespace FuncAddr {
    constexpr uintptr_t LODFILE_OPEN = 0x4FAF30;
}


static char fullLodPath[MAX_PATH];
static LODFile bgLodFile;


static void __declspec(naked) CheckBgLodFile() {
    __asm {
        cmp  dword ptr[bgLodFile], 0 // check if bgLodFile is opened
        jne  search
        // if not, call bgLodFile.open(fullLodPath, 1);
        push 1
#ifdef __clang__
        mov  ecx, OFFSET fullLodPath
		push ecx // workaround for clang bug
#else
        push OFFSET fullLodPath
#endif
        mov  ecx, OFFSET bgLodFile
        call FuncAddr::LODFILE_OPEN
        test eax, eax
        jne fail_end
    search:
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
