#include <nh3api/core/combat.hpp>

#include "obstacles.hpp"
#include "asm_patch.hpp"
#include "types.hpp"


namespace {

    struct TObstacleSpriteInfo {
        const char spriteName[1] = {};
        uint8_t obstacleId = 0;
        const char undergndSpriteName[13] = {};
        CStrPtr surfaceSpriteName = nullptr;

        bool HasOneSpite() const { return spriteName[0] != 0; }
    };

    constexpr size_t MAX_UG_OBSTALCES = 10;

    std::array<TObstacleSpriteInfo, MAX_UG_OBSTALCES> obstacleSpriteNames = { {
        { "", 5,  "ObBDt1Ug.def" },
        { "", 12, "ObDtS3Ug.def" },
        { "", 13, "ObDtS4Ug.def" },
        { "", 19, "ObGMs2Ug.def" },
        { "", 21, "ObGMs1Ug.def" },
        { "", 26, "ObSnRk3U.def" },
        { "", 28, "ObSnRk5U.def" },
        { "", 29, "ObSnRk6U.def" },
        { "", 33, "OBSnCrUg.def" },
        { "", 34, "ObSwMsUg.def" },
    } };


    static CSprite* __fastcall GetObstacleSprite(const TObstacleSpriteInfo& spriteInfo) {
        const CStrPtr spriteName = spriteInfo.HasOneSpite() ?
            spriteInfo.spriteName : (gpCombatManager->map_point.z) ?
                spriteInfo.undergndSpriteName : spriteInfo.surfaceSpriteName;
        return ResourceManager::GetSprite(spriteName);
    }

} // namespace


void InitializeUndergroundObstacles(PatcherInstance& p) {
    Asm::WritePseudoFastCall(p, 0x465DA0, GetObstacleSprite);

    for (TObstacleSpriteInfo& obsSprInfo : obstacleSpriteNames) {
        const CStrPtr& spriteNameRef = combatManager::ObstacleInfo[obsSprInfo.obstacleId].FileName;
        obsSprInfo.surfaceSpriteName = spriteNameRef;
        p.WriteAddressOf(uintptr_t(&spriteNameRef), obsSprInfo);
    }
}
