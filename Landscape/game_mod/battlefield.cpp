#include <nh3api/core/global.hpp>
#include <nh3api/core/combat.hpp>

#include "battlefield.hpp"
#include "asm_helper.h"
#include "asm_patch.hpp"
#include "bf_creature.hpp"
#include "mode.hpp"
#include "hota_terrain.hpp"
#include "types.hpp"


namespace Addr { // Function and data addresses inside Heroes3.exe

namespace CT {
    constexpr uintptr_t WOBJ_ENTRY = 0x464020;
    constexpr uintptr_t ENTRY = 0x464031;
    constexpr uintptr_t ENTRY_HOTA = 0x464050;
    constexpr uintptr_t END_OF_FUNC = 0x4640F7;
}

namespace Bg { // Addresses inside the function selecting battlefield background
    constexpr uintptr_t FORT_BF_ENTRY = 0x4642BE;
    constexpr uintptr_t AREA_BF_ENTRY = 0x4642D4;
    constexpr uintptr_t END_OF_FUNC = 0x46435E;
}

} // namespace Addr


namespace {

inline CStrPtr StrAddr(uintptr_t addr) { return reinterpret_cast<CStrPtr>(addr); }

/* Background of the battlefield on boat */
#define H3BoatDeckBackgr StrAddr(0x66FF4C)

/* Background of the battlefield on two boats */
#define H3TwoBoatsBackgr StrAddr(0x66FF5C)

/* Background of the battlefield on the surface - "Red Rocks" */
CStrPtr const RedRocksBackgr = "CmBkRedMt.pcx";

/* Backgrounds of underground battlefields */
const CStrPtr BfUndBackgr[MAX_HOTA_TERRAIN_TYPES] = {
    /* eTerrainDirt    */ "CmBkDrUg.pcx",
    /* eTerrainSand    */ "CmBkDeU.pcx",
    /* eTerrainGrass   */ "CmBkGrUg.pcx",
    /* eTerrainSnow    */ "CmBkSnUg.pcx",
    /* eTerrainSwamp   */ "CmBkSwUg.pcx",
    /* eTerrainRough   */ "CmBkRghUg.pcx",
    /* eTerrainSubter. */ "CmBkSub.pcx",
    /* eTerrainLava    */ "CmBkLvUg.pcx",
    /* eTerrainWater   */ "CmBkDkUg.pcx",
    /* eTerrainRock    */ "CmBkSub.pcx",
    /* eTerrainHighlnd */ "CmBkFGrUg.pcx",
    /* eTerrainWaste.  */ "CmBkWlUg.pcx"
};

/* Backgrounds of battlefields on magical terrain */
const CStrPtr MagicBfBackgr[MAX_HOTA_MAGIC_TERRAINS] = {
    /* MAGIC_TERRAIN_COAST         */ StrAddr(0x66FF40),
    /* MAGIC_TERRAIN_MAGIC_PLAINS  */ StrAddr(0x66D15C),
    /* MAGIC_TERRAIN_CURSED_GROUND */ StrAddr(0x66D150),
    /* MAGIC_TERRAIN_HOLY_GROUND   */ StrAddr(0x66D144),
    /* MAGIC_TERRAIN_EVIL_FOG      */ StrAddr(0x66D138),
    /* MAGIC_TERRAIN_CLOVER_FIELD  */ StrAddr(0x66D12C),
    /* MAGIC_TERRAIN_LUCID_POOLS   */ StrAddr(0x66D120),
    /* MAGIC_TERRAIN_FIERY_FIELDS  */ StrAddr(0x66D114),
    /* MAGIC_TERRAIN_ROCKLANDS     */ StrAddr(0x66D108),
    /* MAGIC_TERRAIN_MAGIC_CLOUDS  */ StrAddr(0x66D0FC),
    nullptr,
    /* MAGIC_TERRAIN_CRACKED_ICE   */ "CmBkIce.pcx",
    /* MAGIC_TERRAIN_DUNES         */ "CmBkDun.pcx",
    /* MAGIC_TERRAIN_FIELDS_GLORY  */ "CmBkFlGl.pcx"
};

/* Backgrounds of underground battlefields on magical terrain */
const CStrPtr MagicBfUndBackgr[MAX_HOTA_MAGIC_TERRAINS] = {
    /* MAGIC_TERRAIN_COAST         */ "CmBkCstUg.pcx",
    /* MAGIC_TERRAIN_MAGIC_PLAINS  */ "CmBkMgUg.pcx",
    /* MAGIC_TERRAIN_CURSED_GROUND */ "CmBkRghUg.pcx",
    /* MAGIC_TERRAIN_HOLY_GROUND   */ "CmBkHGUg.pcx",
    /* MAGIC_TERRAIN_EVIL_FOG      */ "CmBkEFUg.pcx",
    /* MAGIC_TERRAIN_CLOVER_FIELD  */ "CmBkCFUg.pcx",
    /* MAGIC_TERRAIN_LUCID_POOLS   */ "CmBkLPUg.pcx",
    /* MAGIC_TERRAIN_FIERY_FIELDS  */ "CmBkFFUg.pcx",
    /* MAGIC_TERRAIN_ROCKLANDS     */ "CmBkRkUg.pcx",
    /* MAGIC_TERRAIN_MAGIC_CLOUDS  */ "CmBkMCUg.pcx",
    nullptr,
    /* MAGIC_TERRAIN_CRACKED_ICE   */ "CmBkIceUg.pcx",
    /* MAGIC_TERRAIN_DUNES         */ "CmBkDunUg.pcx",
    /* MAGIC_TERRAIN_FIELDS_GLORY  */ "CmBkFGUg.pcx"
};

CStrPtr const UndergrLakeBfBackgr = "CmBkLkUg.pcx";
CStrPtr const MagicCloudBoatBackgr = "CmBkMCDk.pcx";

/* Backgrounds of town battlefields */
const CStrPtr TownBfBackgr[MAX_HOTA_TOWN_TYPES] = {
    /* eTownCastle     */ StrAddr(0x66d1E8),
    /* eTownRampart    */ StrAddr(0x66d1D8),
    /* eTownTower      */ StrAddr(0x66d1C8),
    /* eTownInferno    */ StrAddr(0x66d1B8),
    /* eTownNecropolis */ StrAddr(0x66d1A8),
    /* eTownDungeon    */ "SgDnSfBk.pcx",
    /* eTownStronghold */ StrAddr(0x66d188),
    /* eTownFortress   */ StrAddr(0x66d178),
    /* eTownConflux    */ StrAddr(0x66d168),
    /* eTownCove       */ "SgCvBack.pcx",
    /* eTownFactory    */ "SgFaBack.pcx",
    /* eTownBulwark    */ "SgBuBack.pcx"
};

/* Backgrounds of underground town battlefields */
const CStrPtr TownBfUndBackgr[MAX_HOTA_TOWN_TYPES] = {
    /* eTownCastle     */ "SgCsUgBk.pcx",
    /* eTownRampart    */ "SgRmUgBk.pcx",
    /* eTownTower      */ "SgTwUgBk.pcx",
    /* eTownInferno    */ "SgInUgBk.pcx",
    /* eTownNecropolis */ "SgNcUgBk.pcx",
    /* eTownDungeon    */ StrAddr(0x66d198),
    /* eTownStronghold */ "SgStUgBk.pcx",
    /* eTownFortress   */ "SgFrUgBk.pcx",
    /* eTownConflux    */ "SgElUgBk.pcx",
    /* eTownCove       */ "SgCvUgBk.pcx",
    /* eTownFactory    */ "SgFaUgBk.pcx",
    /* eTownBulwark    */ "SgBwUgBk.pcx"
};

constexpr int16_t GENERATOR_BEHEMOTH_GHOST = 86;
constexpr int16_t GENERATOR_GORYNYCH = 91;

} // namespace


namespace Combat {
    static bool Cave = false; // battle is taking place in a cave
    static TTerrainType BgTerrain = eTerrainDirt;
    bool disableEffectInFort = false;
    TEffectDef terrainEffect;
    TEffectDef moatEffect;
} // namespace Combat

namespace Fort::Img { /* Fortification image file names */
    static CStrPtr Moat = nullptr;
    static CStrPtr MoatLip = nullptr;
} // namespace Fort::Img


static TTerrainType __fastcall GetCombatTerrain(combatManager* cm) {
    Combat::Cave = false;
    const NewmapCell& cell = *cm->EventCell;
    Combat::BgTerrain = TTerrainType(cell.GroundSet);

    switch (cm->Heroes[0]->type) {

    case OBJECT_CREATURE_BANK:
        if (IsOneOf(cell.objectIndex,
            BANK_CYCLOPS_STOCKPILE,
            BANK_WOG_SNOW_GROTTO,
            BANK_WOG_PALACE_OF_MARTIAL_SPIRIT,
            BANK_WOG_CITADEL_OF_PACIFICATION,
            BANK_WOG_MONASTERY_OF_MAGICIANS,
            BANK_WOG_LIBRARY_OF_LEGENDS,
            BANK_WOG_GROTTO,
            BANK_HOTA_PIRATE_CAVERN,
            BANK_HOTA_SPIT
        )) {
            Combat::Cave = true;
            const TTerrainType realTerrain = Combat::BgTerrain;
            if (realTerrain == eTerrainGrass || realTerrain == eTerrainHighlands
                || (realTerrain == eTerrainSnow && cell.objectIndex != BANK_WOG_SNOW_GROTTO)) {
                // If terrain type is Grass, Snow or Highlands, use the Dirt background
                Combat::BgTerrain = eTerrainDirt;
            }
            return realTerrain;
        }
        break;

    case OBJECT_CREATURE_GENERATOR1:
        if (IsOneOf(cell.objectIndex, GENERATOR_BEHEMOTH, GENERATOR_CYCLOPS, GENERATOR_BEHEMOTH_GHOST)) {
            Combat::Cave = true;
            if (IsOneOf(Combat::BgTerrain, eTerrainGrass, eTerrainSnow, eTerrainHighlands)) {
                Combat::BgTerrain = eTerrainRough;
            }
            return Combat::BgTerrain;
        }
        if (Mode::ERA && cell.objectIndex == GENERATOR_GORYNYCH) {
            Combat::Cave = true;
            if (IsOneOf(Combat::BgTerrain, eTerrainGrass, eTerrainSnow, eTerrainHighlands)) {
                Combat::BgTerrain = eTerrainDirt;
            }
            return Combat::BgTerrain;
        }
        break;

    case OBJECT_MINE:
        switch (cell.objectIndex) {
        case ABANDONED:
            Combat::Cave = true;
            Combat::BgTerrain = eTerrainSubterranean;
            break;
        case CRYSTAL:
        case GOLD:
            Combat::Cave = true;
            if (IsOneOf(Combat::BgTerrain, eTerrainGrass, eTerrainSnow, eTerrainSwamp, eTerrainHighlands)) {
                Combat::BgTerrain = eTerrainRough;
            }
            break;
        }
        break;

    case OBJECT_ABANDONED_MINE2:
        Combat::Cave = true;
        Combat::BgTerrain = eTerrainSubterranean;
        break;

    default:
        break;
    }

    if (!Combat::Cave && cell.IsBeachBorder
        && (Combat::BgTerrain != eTerrainSubterranean || cm->map_point.z == 0)) {
        cm->magic_terrain = MAGIC_TERRAIN_COAST;
        Combat::BgTerrain = eTerrainSand;
    }

    return Combat::BgTerrain;
}


static TTerrainType __fastcall GetCombatTerrainWaterObj(combatManager* cm) {
    Combat::Cave = false;
    Combat::BgTerrain = TTerrainType(cm->EventCell->GroundSet);

    if (Combat::BgTerrain == eTerrainWater) {
        cm->OnBoats = true;
    }
    else {
        switch (cm->Heroes[0]->type) {

        case OBJECT_CREATURE_BANK:
            switch (cm->EventCell->objectIndex) {
            case BANK_HOTA_BEHOLDERS_SANCTUARY:
                Combat::Cave = true;
                if (IsOneOf(Combat::BgTerrain, eTerrainGrass, eTerrainHighlands)) {
                    Combat::BgTerrain = eTerrainSwamp;
                }
                else if (IsOneOf(Combat::BgTerrain, eTerrainSnow, eTerrainLava)) {
                    Combat::BgTerrain = eTerrainDirt;
                }
                return eTerrainWater;
            case BANK_HOTA_TEMPLE_OF_THE_SEA:
                Combat::Cave = true;
                if (Combat::BgTerrain == eTerrainLava){
                    Combat::BgTerrain = eTerrainDirt;
                }
                return eTerrainWater;
            }
            break;

        case OBJECT_DERELICT_SHIP:
            cm->OnBoats = true;
            Combat::BgTerrain = eTerrainWater;
            break;

        default:
            break;
        }
    }

    return Combat::BgTerrain;
}


#if defined(_MSC_VER)
#pragma warning(disable : 4063)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wswitch"
#endif

static CStrPtr __fastcall GetFortBfBackgr(combatManager* cm) {
    const bool underground = cm->map_point.z > 0;
    const TTownType tt = TTownType(cm->combatTown->townType);

    Combat::disableEffectInFort = false;
    Combat::terrainEffect.Reset();
    Combat::moatEffect.Reset();

    if (cm->bMoatOn) {
        switch (tt) {
        case eTownCastle:
        case eTownConflux:
        case eTownCove:
            Combat::moatEffect.upperRow = 253;
            Combat::moatEffect.pattern = &waterTransp;
            break;
        case eTownInferno:
            Combat::moatEffect.upperRow = 254;
            Combat::moatEffect.pattern = &fogGradient;
            Combat::moatEffect.disableShadow = true;
            break;
        case eTownDungeon:
        case eTownFortress:
            Combat::moatEffect.upperRow = 253;
            Combat::moatEffect.pattern = &darkWTransp;
            break;
        case eTownFactory:
            if (underground) {
                Fort::Img::Moat = "SgFaMoatUg.pcx";
            }
            break;
        default:
            break;
        }
    }

    switch (cm->magic_terrain) {

    case MAGIC_TERRAIN_INVALID:
        if (cm->EventCell->GroundSet == eTerrainSnow) {
            switch (tt) {
            case eTownStronghold:
                if (cm->bMoatOn) {
                    Fort::Img::Moat = "SgStMoTr.pcx";
                }
                [[fallthrough]];
            case eTownNecropolis:
                return underground ? TownBfUndBackgr[eTownTower] : TownBfBackgr[eTownTower];
            default:
                break;
            }
        }
        break;
    case MAGIC_TERRAIN_CURSED_GROUND:
        if (tt == eTownTower) break;

        if (cm->bMoatOn && tt == eTownDungeon) {
            Fort::Img::MoatLip = "SgDnCGMlip.pcx";
        }
        if (cm->fortificationLevel == eFortificationFort
            || IsOneOf(tt, eTownNecropolis, eTownDungeon, eTownStronghold, eTownConflux)) {
            return underground ? TownBfUndBackgr[eTownStronghold] : "SgCurBack.pcx";
        }
        break;
    case MAGIC_TERRAIN_EVIL_FOG:
        if (cm->fortificationLevel == eFortificationFort && tt != eTownTower) {
            Combat::terrainEffect.upperRow = 248;
            Combat::terrainEffect.pattern = &fogGradient;
            return underground ?
                MagicBfUndBackgr[MAGIC_TERRAIN_EVIL_FOG] : MagicBfBackgr[MAGIC_TERRAIN_EVIL_FOG];
        }
        break;
    case MAGIC_TERRAIN_CLOVER_FIELD:
        if (cm->bMoatOn && tt == eTownStronghold) {
            Fort::Img::Moat = "SgStMoTr.pcx";
        }
        if (IsOneOf(tt, eTownCastle, eTownRampart, eTownStronghold, eTownFortress, eTownConflux, eTownCove)) {
            return underground ? "SgCFUgBk.pcx" : "SgCFBack.pcx";
        }
        break;
    case MAGIC_TERRAIN_FIERY_FIELDS:
        if (tt == eTownInferno) {
            Combat::disableEffectInFort = true;
            Combat::terrainEffect.upperRow = 250;
            Combat::terrainEffect.pattern = &fogGradient;
            Combat::terrainEffect.disableShadow = true;
            return "SgFFBack.pcx";
        }
        break;
    case MAGIC_TERRAIN_ROCKLANDS:
        if (cm->bMoatOn) {
            switch (tt) {
            case eTownDungeon:
                Fort::Img::MoatLip = "SgDnRkMlip.pcx";
                break;
            case eTownStronghold:
                Fort::Img::Moat = "SgStMoTr.pcx";
                break;
            default:
                break;
            }
        }
        if (IsOneOf(tt, eTownCastle, eTownInferno, eTownNecropolis, eTownDungeon, eTownStronghold, eTownConflux, eTownCove)) {
            return underground ? "SgRkUgBk.pcx" : "SgRkBack.pcx";
        }
        break;
    case MAGIC_TERRAIN_MAGIC_CLOUDS:
        if (tt == eTownTower) {
            Combat::terrainEffect.upperRow = 248;
            Combat::terrainEffect.pattern = &fogGradient;
            return underground ?
                MagicBfUndBackgr[MAGIC_TERRAIN_MAGIC_CLOUDS] : MagicBfBackgr[MAGIC_TERRAIN_MAGIC_CLOUDS];
        }
        break;
    case MAGIC_TERRAIN_CRACKED_ICE:
        if (tt == eTownNecropolis) {
            return underground ? TownBfUndBackgr[eTownTower] : TownBfBackgr[eTownTower];
        }
        break;
    default:
        break;
    }

    return (underground ? TownBfUndBackgr : TownBfBackgr)[tt];
}


static CStrPtr __fastcall GetAreaBfBackgr(combatManager* cm) {
    Combat::disableEffectInFort = false;
    Combat::terrainEffect.Reset();
    Combat::moatEffect.Reset();

    if (cm->OnBoats) {
        const bool magicClouds = cm->magic_terrain == MAGIC_TERRAIN_MAGIC_CLOUDS;
        const hero* enemyHero = cm->Heroes[1];

        if (enemyHero && (enemyHero->flags & HF_ISINBOAT)) {
            return magicClouds ? "CmBkMCBt.pcx" : H3TwoBoatsBackgr; // Set background for battle between two boats
        }
        /* Set background for battle on one boat */
        return magicClouds ? MagicCloudBoatBackgr : (cm->map_point.z ? BfUndBackgr[eTerrainWater] : H3BoatDeckBackgr);
    }

    const bool underground = cm->map_point.z || (Combat::Cave && !cm->combatTown);

    if (cm->magic_terrain >= MAGIC_TERRAIN_COAST) {
        switch (cm->magic_terrain) {
        case MAGIC_TERRAIN_LUCID_POOLS:
            Combat::terrainEffect.upperRow = 253;
            Combat::terrainEffect.pattern = &waterTransp;
            break;
        case MAGIC_TERRAIN_EVIL_FOG:
        case MAGIC_TERRAIN_MAGIC_CLOUDS:
            Combat::terrainEffect.upperRow = 248;
            Combat::terrainEffect.pattern = &fogGradient;
            break;
        case MAGIC_TERRAIN_FIERY_FIELDS:
            Combat::terrainEffect.upperRow = 250;
            Combat::terrainEffect.pattern = &fogGradient;
            Combat::terrainEffect.disableShadow = true;
            break;
        default:
            break;
        }
        return (underground ? MagicBfUndBackgr : MagicBfBackgr)[cm->magic_terrain];
    }
    if (underground) {
        if (cm->combatTown) {
            Combat::BgTerrain = cm->combatTerrain;
        }
        if (Combat::BgTerrain == eTerrainSubterranean && cm->EventCell->IsBeachBorder) {
            return UndergrLakeBfBackgr; // Set the battlefield background to Underground Lake
        }
        return BfUndBackgr[Combat::BgTerrain];
    }
    if (cm->combatTerrain == eTerrainSubterranean) {
        return RedRocksBackgr; // Set the battlefield background to Red Rocks
    }

    RETURN_ADDRESS = 0x46433A; // Go to the H3 instructions for setting the original battlefield background
    return nullptr;
}


static uint64_t __fastcall SetupFortSprites() {
    typedef std::array<combatManager::TWallTraits, combatManager::kNumWallSections> ArrayOfWallTraits;

    const int8_t combatTownType = gpCombatManager->combatTown->townType;

    const ArrayOfWallTraits& townWallTraits = (*reinterpret_cast<ArrayOfWallTraits**>(0x462FBA))[combatTownType];

    for (size_t sectNum = 0; sectNum < townWallTraits.size(); ++sectNum) {
        const auto& bmpNames = townWallTraits[sectNum].filenames;
        auto& wi = gpCombatManager->wallImages[sectNum];

        switch (sectNum) {
        case combatManager::eWallSectionMoat:
            wi[0] = (!bmpNames[0] || (combatTownType == eTownStronghold && gpGame->iGameType < 2)) ?
                nullptr : ResourceManager::GetBitmap816(Fort::Img::Moat ? Fort::Img::Moat : bmpNames[0]);
            for (int j = 1; j < 5; ++j) { wi[j] = nullptr; }
            break;
        case combatManager::eWallSectionMoatLip:
            wi[0] = bmpNames[0] ? ResourceManager::GetBitmap816(Fort::Img::MoatLip ? Fort::Img::MoatLip : bmpNames[0]) : nullptr;
            for (int j = 1; j < 5; ++j) { wi[j] = nullptr; }
            break;
        default:
            for (int j = 0; j < 5; ++j) {
                const CStrPtr bmpName = bmpNames[j];
                wi[j] = bmpName ? ResourceManager::GetBitmap816(bmpName) : nullptr;
            }
            break;
        }
    }
    return 0; // set EDX to 0
}


#if defined(__GNUG__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif

static void PatchSetupObstacles_HeroOnBoat(PatcherInstance& p) {
    using namespace Asm;

    // Addresses inside the function setting obstacles on the battlefield
    constexpr uintptr_t aCheckBoats   = 0x4662C3;
    constexpr uintptr_t aTwoBoats     = 0x4662F0;
    constexpr uintptr_t aCkeckFort    = 0x466330;
    constexpr uintptr_t aStdPlacement = 0x46634C;

    constexpr int32_t oIgnoreMT = -0x10; // offset of local variable used to ignore magic terrain for obstacle placement
    constexpr Reg cmbtMgr = EDI; // register containing the address of combatManager object
    constexpr int32_t oFirstHero = offsetof(combatManager, Heroes); // offset of the first hero object in combatManager
    constexpr int32_t oSecondHero = oFirstHero + sizeof(hero*); // offset of the first hero object in combatManager

    Sequence seq{
        ClearReg(EBX), // EBX = 0
        SetLocalVar(oIgnoreMT, EBX), // set 0 - don't ignore magic terrain
        JumpIfNotZero(aCkeckFort),   // jump, if battle is not on the water

        SetRegConst(EBX, HF_ISINBOAT),
        SetRegPtr(ECX, cmbtMgr, oFirstHero),  // ECX = address of the first hero object
        TestRegPtr(EBX, ECX, offsetof(hero, flags)), // check if the hero is on the boat
        JumpIfZero(aCkeckFort),      // jump, if the hero is not on the boat

        SetLocalVar(oIgnoreMT, EBX), // set 0x40000 - ignore all magic terrains
        SetRegPtr(ECX, cmbtMgr, oSecondHero), // ECX = address of the second hero object
        JumpIfEcxZero(aStdPlacement),
        TestRegPtr(EBX, ECX, offsetof(hero, flags)), // check if  the second hero is on the boat
        JumpIfZero(aStdPlacement),   // jump, if the hero is not on the boat
    };
    static_assert(seq.Size() == (aTwoBoats - aCheckBoats), "code patch has wrong size");

    seq.Apply(p, aCheckBoats);

    Sequence{ CmpLocalVar(oIgnoreMT, ECX) }.Apply(p, 0x466363); // check if magic terrain should be ignored

    p.WriteByte(0x46636C, 0x7C); // change JE to JL
}


void BattlefieldPatch(PatcherInstance & p, bool color32bit, uint32_t battleY) {
    /* Combat Terrain */
    const uintptr_t cmbtTrHookAddr = Mode::HotA ? Addr::CT::ENTRY_HOTA : Addr::CT::ENTRY;
    Asm::WritePseudoFastCall(p, cmbtTrHookAddr, GetCombatTerrain, Asm::ESI, Addr::CT::END_OF_FUNC);
    if (Mode::HotA) {
        Asm::WritePseudoFastCall(p, Addr::CT::WOBJ_ENTRY, GetCombatTerrainWaterObj, Asm::ESI, Addr::CT::END_OF_FUNC);
    }

    /* Background */
    Asm::WritePseudoFastCall(p, Addr::Bg::FORT_BF_ENTRY, GetFortBfBackgr, Addr::Bg::END_OF_FUNC);
    Asm::WritePseudoFastCall(p, Addr::Bg::AREA_BF_ENTRY, GetAreaBfBackgr, Addr::Bg::END_OF_FUNC);

    /* Fortification */
    Asm::WritePseudoFastCall(p, 0x462F9B, SetupFortSprites, 0x463034);

    /* Obstacles */
    PatchSetupObstacles_HeroOnBoat(p);

    if (!color32bit) return;

    BfScreen::firstRow = (battleY > 3720) ? 0 : static_cast<int>(battleY);
    BfScreen::lastRow = BfScreen::firstRow + 556;

    /* Creatures */
    if (Mode::HotA) {
        const HMODULE hotaDll = GetModuleHandle("HotA.dll");
        if (hotaDll) {
            const uintptr_t drawCrFuncAddr = uintptr_t(hotaDll) + 0x801EB;

            // check whether the address of CSprite::DrawCreature is located at [drawCrFuncAddr]
            if (*reinterpret_cast<uintptr_t*>(drawCrFuncAddr) == 0x47B680) {
                // replace the address of the creature drawing function
                p.WriteDword(drawCrFuncAddr, uintptr_t(HotA_Sprite_DrawCreature));
            }
        }
    }
    else {
        // Hook for combatManager::DrawCreature
        Asm::WriteFuncAddress(p, 0x43E251, SoD_CM_DrawCreature);
        // replace the `frame` param (EDX) with the `army` object (EBX)
        p.WriteByte(0x43E247, Asm::PushReg(Asm::EBX).opcode);
    }
}


/*
* Assembler code for the function in Heroes3_HD.exe that returns the name of the battlefield background file.
* 
* input:  ECX = the address of the combatManager object.
* output: EAX = the address of the background file name (string).
* 
004642B0    push        esi
004642B1    mov         esi,ecx
004642B3    push        edi
004642B4    mov         eax,dword ptr [esi+132F4h]  * EAX = the level of the town walls
004642BA    test        eax,eax         
004642BC    jle         004642D4        * If there are no wals, go to CHECK_MAGIC_TERRAIN

Addr::Bg::GET_TOWN_BF_BG:
004642BE    code patch here --> jump to the function GetFortBfBackgr
* original instruction:
* 004642BE  mov         eax,dword ptr [esi+53C8h]   * EAX = address of the town object

004642C4    movsx       ecx,byte ptr [eax+4]        * ECX = town type
004642C8    mov         eax,dword ptr [ecx*4+63D2A0h]
004642CF    jmp         0046435E        -> go to Addr::END_OF_FUNC

CHECK_MAGIC_TERRAIN:
004642D4    mov         ecx,dword ptr [esi+53C0h]   * ECX = magic terrain type
Addr::GET_AREA_BF_BG:
004642DA    code patch here --> jump to the function GetAreaBfBackgr
* original instructions:
* 004642DA  cmp         ecx,0FFFFFFFFh
* 004642DD  je          004642EC        * If there is no magic terrain, go to CHECK_BOATS

004642DF    test        ecx,ecx
004642E1    je          004642EC        * If it's the coast, go to CHECK_BOATS
004642E3    mov         eax,dword ptr [ecx*4+63D2C8h]
004642EA    jmp         0046435E        -> go to Addr::END_OF_FUNC

CHECK_BOATS:
004642EC    mov         eax,dword ptr [esi+53CCh]   * EAX = address of the hero object
004642F2    test        eax,eax
004642F4    je          0046431E
004642F6    mov         edi,dword ptr [eax+105h]    * EDI = hero flags
004642FC    mov         edx,40000h                  * EDX = HF_ISINBOAT
00464301    test        edx,edi
00464303    je          0046431E        * If the hero is not on the boat, go to STD_BF_BG
00464305    mov         eax,dword ptr [esi+53D0h]   * EAX = address of the enemy hero object
0046430B    test        eax,eax
0046430D    je          0046431E
0046430F    test        dword ptr [eax+105h],edx    * check the enemy hero's flags
00464315    je          0046431E        * If the enemy hero is not on the boat, go to STD_BF_BG
00464317    mov         eax,66FF5Ch     * EAX = address of the battlefield background with two boats
0046431C    jmp         0046435E        -> go to Addr::END_OF_FUNC

STD_BF_BG:  <- return from the GetAreaBfBackgr function
0046431E    mov         al,byte ptr [esi+53C6h]
00464324    test        al,al
00464326    je          0046432F
00464328    mov         eax,66FF4Ch
0046432D    jmp         0046435E        -> go to Addr::END_OF_FUNC
0046432F    test        ecx,ecx
00464331    jne         0046433A
00464333    mov         eax,66FF40h
00464338    jmp         0046435E        -> go to Addr::END_OF_FUNC
0046433A    mov         edx,dword ptr [esi+13FF0h]
00464340    mov         ecx,dword ptr ds:[6992B8h]
00464346    push        edx
00464347    call        0041AD10
0046434C    mov         ecx,dword ptr [esi+5394h]
00464352    lea         eax,[eax+ecx*2]
00464355    add         ecx,eax
00464357    mov         eax,dword ptr [ecx*4+63D2F0h]

Addr::END_OF_FUNC:   <- return from functions GetTownBfBackgr and GetAreaBfBackgr
0046435E    mov         dword ptr [esi+539Ch],1
00464368    mov         dword ptr [esi+5398h],0FFFFFFFFh
00464372    pop         edi
00464373    pop         esi
00464374    ret
*/
