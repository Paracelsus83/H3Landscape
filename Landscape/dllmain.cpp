// SPDX-FileCopyrightText: 2026 Paracelsus83
// SPDX-License-Identifier: MIT

#include <nh3api/hd_mod.hpp>
#include "game_mod/adv_map.hpp"
#include "game_mod/battlefield.hpp"
#include "game_mod/img_loader.hpp"
#include "game_mod/obstacles.hpp"
#include "game_mod/era_vfs.hpp"
#include "game_mod/mode.hpp"


namespace Mode {
    bool HotA = false;
    bool ERA = false;
    bool WogFix = false;
}

namespace {
    static Patcher* patcher;
    static PatcherInstance* patcherInstance;
}


inline static uint32_t getVar(const char name[]) {
    const Variable* const v = patcher->VarFind(name);
	return (v == nullptr) ? 0 : v->GetValue();
}


static void LateInit() {
    const HD_game_version gameVersion = getHDModEXEVersion(patcher);
    Mode::HotA = gameVersion & HD_HOTA;
    Mode::ERA = gameVersion & HD_ERA;
    if (Mode::ERA) {
		Mode::WogFix = EraVfs::IsModActive("WoG Fix Lite");
    }

    const uint32_t colorMode = getVar("HD.Option.ColorMode");
    const bool color32bit = colorMode >= 4 && colorMode != 6 && (!Mode::ERA || getVar("HD.NewTrue32"));

    assert(patcherInstance);
    AdvMapPatch(*patcherInstance);
    AdvMapTownPatch(*patcherInstance);
    BattlefieldPatch(*patcherInstance, color32bit, getVar("HD.Battle.Y"));
}


extern "C" __declspec(dllexport) int H3L_Init() {
    LateInit();
    return FASTCALL_0(int, 0x4EEA70); // old main
}


extern BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    static const char pluginName[] = "HD.Plugin.Landscape";

    if (fdwReason == DLL_PROCESS_ATTACH) {
        patcher = GetPatcher();
        if (patcher == nullptr) return FALSE;

        if (patcher->GetInstance(pluginName)) return FALSE;

        patcherInstance = patcher->CreateInstance(pluginName);
        if (patcherInstance == nullptr) return FALSE;

        ImgLoaderPatch(*patcherInstance, hModule);
        InitializeUndergroundObstacles(*patcherInstance);

        if (lpReserved == nullptr) {
            LateInit(); // The Landscape.dll has been dynamically loaded
        }
    }
    return TRUE;
}
