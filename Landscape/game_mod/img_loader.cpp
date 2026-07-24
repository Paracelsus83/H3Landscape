#include <nh3api/core/resources/files.hpp>
#include <nh3api/core/resources/resource_enums.hpp>
#include <nh3api/core/nh3api_std/patcher_x86.hpp>

#include "img_loader.hpp"
#include "asm_patch.hpp"
#include "types.hpp"


namespace Addr {

namespace BmpLdr {
    constexpr uintptr_t ENTRY_POINT = 0x55ABFB;
    constexpr uintptr_t LOAD_BITMAP = 0x55ACA6;
}

namespace SprLdr {
    constexpr uintptr_t ENTRY_POINT = 0x55CA9A;
    constexpr uintptr_t LOAD_SPRITE = 0x55CB13;
}

} // namespace Addr


namespace {

static char fullLodPath[MAX_PATH];
static LODFile lodFile;


inline void ResourceError(CStrPtr caller, EResourceType resType, CStrPtr name) {
    FASTCALL_3(void, 0x559750, caller, resType, name);
}


static LODFile* __stdcall FindResInLandscapeLod(CStrPtr resName) {
    if (lodFile.fileptr == nullptr) {
        if (lodFile.open(fullLodPath, LODFile::openmode::read) != 0) {
            ResourceError("", RType_data, fullLodPath);
            return nullptr;
        }
    }

    if (lodFile.pointAt(resName)) {
        return &lodFile;
    }

    ResourceError("", RType_bitmap, resName);
    return nullptr;
}

} // namespace


void ImgLoaderPatch(PatcherInstance& p, HMODULE hModule) {
    const size_t pathLen = GetModuleFileName(hModule, fullLodPath, sizeof(fullLodPath));

    if ((pathLen > 3) && (pathLen < MAX_PATH)) {
        // Replace extension "dll" -> "lod"
        memcpy(fullLodPath + pathLen - 3, "lod", 3);
    }
    else {
        const char defaultLodPath[] = "_HD3_Data\\Packs\\Landscape\\Landscape.lod";
        memcpy(fullLodPath, defaultLodPath, sizeof(defaultLodPath));
    }

    using namespace Asm;
    Sequence seqBmp{
        Call(FindResInLandscapeLod),
        SetReg(Reg::ESI, Reg::EAX),
        TestReg(Reg::EAX, Reg::EAX),
        Jump32IfNotZero(Addr::BmpLdr::LOAD_BITMAP)
    };
    static_assert(seqBmp.Size() == 0x55AC0A - Addr::BmpLdr::ENTRY_POINT);
    seqBmp.Apply(p, Addr::BmpLdr::ENTRY_POINT);

    Sequence seqSpr{
        Call(FindResInLandscapeLod),
        SetReg(Reg::ESI, Reg::EAX),
        TestReg(Reg::EAX, Reg::EAX),
        Jump32IfNotZero(Addr::SprLdr::LOAD_SPRITE)
    };
    static_assert(seqSpr.Size() == 0x55CAA9 - Addr::SprLdr::ENTRY_POINT);
    seqSpr.Apply(p, Addr::SprLdr::ENTRY_POINT);
}
