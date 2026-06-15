#include <nh3api/core/global.hpp>

#include "asm_helper.h"
#include "asm_patch.hpp"
#include "adv_map.hpp"
#include "adv_mt.hpp"
#include "hota_terrain.hpp"


typedef const char SpriteName[13];

const SpriteName TowerGrassSprites[5] = {
    "AVCTowVG.def",
    "AVCTowFG.def",
    "AVCTowCG.def",
    "AVCTowXG.def",
    "AVCTowZG.def"
};

const SpriteName CastleSnowSprites[5] = {
    "AVCCasVS.def",
    "AVCCasFS.def",
    "AVCCasCS.def",
    "AVCCasXS.def",
    "AVCCasZS.def"
};

const SpriteName InfernoSnowSprites[5] = {
    "AVCInfVS.def",
    "AVCInfFS.def",
    "AVCInfCS.def",
    "AVCInfXS.def",
    "AVCInfZS.def"
};

const SpriteName NecropolisSnowSprites[5] = {
    "AVCNecVS.def",
    "AVCNecFS.def",
    "AVCNecCS.def",
    "AVCNecXS.def",
    "AVCNecZS.def"
};

const SpriteName StrongholdSnowSprites[5] = {
    "AVCStrVS.def",
    "AVCStrFS.def",
    "AVCStrCS.def",
    "AVCStrXS.def",
    "AVCStrZS.def"
};


inline void CopySpriteName(char dest[], const SpriteName& sprName) {
    memcpy(dest, sprName, sizeof(SpriteName));
}


static uint32_t GetTownLevel(const town& tw) {
    uint32_t buildingMask = uint32_t(tw.populationMask);
    if (buildingMask & (1 << HALL_CAPITOL_ID)) {
        return 4;
    }
    if (buildingMask & (1 << CASTLE_CASTLE_ID)) {
        return 3;
    }
    if (buildingMask & (1 << CASTLE_CITADEL_ID)) {
        return 2;
    }
    if (buildingMask & (1 << CASTLE_FORT_ID)) {
        return 1;
    }
    return 0;
}


static const town* __fastcall GetSpriteNameForTown(NewmapCell& cell, char spriteName[]) {
    if (cell.extraInfo < 0) {
        // This should not have happened
        CopySpriteName(spriteName, "AVCranx0.def"); // fallback to placeholder sprite
        return nullptr;
    }

    const town& tw = gpGame->townPool[cell.get_map_extraInfo().extraInfo];
    const TerrainType8 tt = GetTerrainType(gpGame->worldMap, tw);

    switch (tw.townType) {
    case eTownCastle:
        if (tt == eTerrainSnow) {
            CopySpriteName(spriteName, CastleSnowSprites[GetTownLevel(tw)]);
            return nullptr;
        }
        break;
    case eTownTower:
        if (IsOneOf(tt, eTerrainGrass, eTerrainSwamp, eTerrainHighlands)) {
            CopySpriteName(spriteName, TowerGrassSprites[GetTownLevel(tw)]);
            return nullptr;
        }
        break;
    case eTownInferno:
        if (tt == eTerrainSnow) {
            CopySpriteName(spriteName, InfernoSnowSprites[GetTownLevel(tw)]);
            return nullptr;
        }
        break;
    case eTownNecropolis:
        if (tt == eTerrainSnow) {
            CopySpriteName(spriteName, NecropolisSnowSprites[GetTownLevel(tw)]);
            return nullptr;
        }
        break;
    case eTownStronghold:
        if (tt == eTerrainSnow) {
            CopySpriteName(spriteName, StrongholdSnowSprites[GetTownLevel(tw)]);
            return nullptr;
        }
        break;
    }

    RETURN_ADDRESS = 0x4C9792;
    return &tw;
}


void AdvMapTownPatch(PatcherInstance& p) {
    Asm::Sequence{
        Asm::LoadAddress(Asm::EDX, Asm::EBP, -0x84),
        Asm::PushConst32(0x4C9844), // return address
        Asm::Jump(GetSpriteNameForTown)
    }
    .Apply(p, 0x4C9770);
}
