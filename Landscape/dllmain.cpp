#include <nh3api/hd_mod.hpp>
#include <nh3api/core/nh3api_std/patcher_x86.hpp>
#include "game_mod/battlefield.hpp"
#include "game_mod/img_loader.hpp"


bool HotAMode = false;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        Patcher* patcher = GetPatcher();
        if (patcher == nullptr) return FALSE;

        const HD_game_version gameVersion = getHDModEXEVersion(patcher);
        HotAMode = (gameVersion == 0) || (gameVersion & HD_HOTA) != 0;

        PatcherInstance* instance = patcher->CreateInstance("HD.Plugin.Landscape");
        if (instance) {
            ImgLoaderPatch(*instance, hModule);
            BattleBackgroundPatch(*instance);
        }
    }
    return TRUE;
}
