#include <nh3api/core/resources/files.hpp>
#include <nh3api/core/nh3api_std/patcher_x86.hpp>

#include "img_loader.hpp"
#include "asm_helper.h"


namespace Addr {

namespace BmpLdr {
	constexpr uintptr_t ENTRY_POINT = 0x55ABFB;
    constexpr uintptr_t LOAD_BITMAP = 0x55ACA6;
    constexpr uintptr_t NO_BITMAP = 0x55AC00;

}

namespace SprLdr {
    constexpr uintptr_t ENTRY_POINT = 0x55CA9A;
    constexpr uintptr_t LOAD_SPRITE = 0x55CB3B;
    constexpr uintptr_t NO_SPRITE = 0x55CA9F;
}

namespace Func {
    constexpr uintptr_t LODFILE_OPEN = 0x4FAF30;
    constexpr uintptr_t FIND_IN_LOD = 0x4FB100;
}

} // namespace Addr


static char fullLodPath[MAX_PATH];
static LODFile lodFile;


ASM_CODE_PATCH CheckBitmapInLodFile() {
    __asm {
        cmp  dword ptr [lodFile], 0 // check if lodFile is opened
        jne  search
        // if not, call lodFile.open(fullLodPath, 1);
        push 1
#ifdef __clang__
        mov  ecx, OFFSET fullLodPath
        push ecx // workaround for clang bug
#else
        push OFFSET fullLodPath
#endif
        mov  ecx, OFFSET lodFile
        call Addr::Func::LODFILE_OPEN
        test eax, eax
        jne  fail_end
    search:
        mov  esi, OFFSET lodFile
        mov  ecx, esi
        call Addr::Func::FIND_IN_LOD
        test al, al
        je   fail
        jmp  Addr::BmpLdr::LOAD_BITMAP // go to loading pcx
    fail:
        push ebx
    fail_end:
        mov  edx, 10h
        jmp  Addr::BmpLdr::NO_BITMAP  // go to warning mesage
    }
}


ASM_CODE_PATCH CheckSpriteInLodFile() {
    __asm {
        cmp  dword ptr [lodFile], 0 // check if lodFile is opened
        jne  search
        // if not, call lodFile.open(fullLodPath, 1);
        push 1
#ifdef __clang__
        mov  ecx, OFFSET fullLodPath
        push ecx // workaround for clang bug
#else
        push OFFSET fullLodPath
#endif
        mov  ecx, OFFSET lodFile
        call Addr::Func::LODFILE_OPEN
        test eax, eax
        jne  fail_end
    search:
        mov  ecx, OFFSET lodFile
        mov  dword ptr [ebp-10h], ecx
        call Addr::Func::FIND_IN_LOD
        test al, al
        je   fail
        jmp  Addr::SprLdr::LOAD_SPRITE // go to loading pcx
    fail:
        push ebx
    fail_end:
        mov  edx, 40h
        jmp  Addr::SprLdr::NO_SPRITE  // go to warning mesage
    }
}


void ImgLoaderPatch(PatcherInstance& p, HMODULE hModule) {
    size_t pathLen = GetModuleFileName(hModule, fullLodPath, sizeof(fullLodPath));

    if ((pathLen > 3) && (pathLen < MAX_PATH)) {
        // Replace extension "dll" -> "lod"
        memcpy(fullLodPath + pathLen - 3, "lod", 3);
    }
    else {
        const char defaultLodPath[] = "_HD3_Data\\Packs\\Landscape\\Landscape.lod";
        memcpy(fullLodPath, defaultLodPath, sizeof(defaultLodPath));
    }

    p.WriteJmp(Addr::BmpLdr::ENTRY_POINT, uintptr_t(CheckBitmapInLodFile));
    p.WriteJmp(Addr::SprLdr::ENTRY_POINT, uintptr_t(CheckSpriteInLodFile));
}
