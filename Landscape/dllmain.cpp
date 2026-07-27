// SPDX-FileCopyrightText: 2026 Paracelsus83
// SPDX-License-Identifier: MIT

#include <nh3api/hd_mod.hpp>
#include "game_mod/adv_map.hpp"
#include "game_mod/battlefield.hpp"
#include "game_mod/img_loader.hpp"
#include "game_mod/obstacles.hpp"
#include "game_mod/mode.hpp"


namespace Mode {
    bool HotA = false;
    bool ERA = false;
}

namespace {
    static Patcher* patcher;
    static PatcherInstance* patcherInstance;
}


static void LateInit() {
    const HD_game_version gameVersion = getHDModEXEVersion(patcher);
    Mode::HotA = gameVersion & HD_HOTA;
    Mode::ERA = gameVersion & HD_ERA;

    assert(patcherInstance);
    AdvMapPatch(*patcherInstance);
    AdvMapTownPatch(*patcherInstance);
    BattlefieldPatch(*patcherInstance);
}


extern "C" __declspec(dllexport) int H3L_Init() {
    LateInit();
    return FASTCALL_0(int, 0x4EEA70); // old main
}


extern BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        patcher = GetPatcher();
        if (patcher == nullptr) return FALSE;

        patcherInstance = patcher->CreateInstance("HD.Plugin.Landscape");
        if (patcherInstance == nullptr) return FALSE;

        ImgLoaderPatch(*patcherInstance, hModule);
        InitializeUndergroundObstacles(*patcherInstance);

        if (lpReserved == nullptr) {
            LateInit(); // The Landscape.dll has been dynamically loaded
        }
    }
    return TRUE;
}
