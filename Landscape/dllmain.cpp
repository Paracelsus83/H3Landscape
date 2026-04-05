#include <nh3api/hd_mod.hpp>
#include "game_mod/adv_map.hpp"
#include "game_mod/battlefield.hpp"
#include "game_mod/img_loader.hpp"


namespace Mode {
    bool HotA = false;
    bool ERA = false;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        Patcher* patcher = GetPatcher();
        if (patcher == nullptr) return FALSE;

        const HD_game_version gameVersion = getHDModEXEVersion(patcher);
        Mode::HotA = !gameVersion || (gameVersion & HD_HOTA);
        Mode::ERA = gameVersion & HD_ERA;

        PatcherInstance* instance = patcher->CreateInstance("HD.Plugin.Landscape");
        if (instance) {
            ImgLoaderPatch(*instance, hModule);
            AdvMapPatch(*instance);
            BattlefieldPatch(*instance);
        }
    }
    return TRUE;
}
