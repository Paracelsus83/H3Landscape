#include <nh3api/core/nh3api_std/patcher_x86.hpp>
#include "game_mod/battlefield.hpp"
#include "game_mod/img_loader.hpp"


BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        Patcher* patcher = GetPatcher();
        if (patcher == nullptr) return FALSE;

        PatcherInstance* instance = patcher->CreateInstance("HD.Plugin.Landscape");
        if (instance) {
            ImgLoaderPatch(*instance, hModule);
            BattleBackgroundPatch(*instance);
        }
    }
    return TRUE;
}
