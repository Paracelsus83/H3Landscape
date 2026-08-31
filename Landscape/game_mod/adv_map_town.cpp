#include <nh3api/core/global.hpp>

#include "asm_patch.hpp"
#include "adv_map.hpp"
#include "adv_mt.hpp"
#include "hota_terrain.hpp"
#include "mode.hpp"


namespace {

using TownSpriteName = const char[6];
using TownSpriteNameSet = const std::array<TownSpriteName, 5>;


const std::array<TownSpriteNameSet, MAX_HOTA_TOWN_TYPES> regularTownSprites = { {
    { "cast0", "casF0", "casC0", "casX0", "casZ0" },
    { "ramp0", "ramF0", "ramC0", "ramX0", "ramZ0" },
    { "towr0", "towF0", "towC0", "towX0", "towZ0" },
    { "inft0", "infF0", "infC0", "infX0", "infZ0" },
    { "necr0", "necF0", "necC0", "necX0", "necZ0" },
    { "dung0", "dunF0", "dunC0", "dunX0", "dunZ0" },
    { "stro0", "strF0", "strC0", "strX0", "strZ0" },
    { "ftrt0", "forF0", "forC0", "ftrX0", "forZ0" },
    { "hfor0", "hfoF0", "hfoC0", "hforX", "hforZ" },
    { "cove0", "covF0", "covC0", "covX0", "covZ0" },
    { "facE0", "facF0", "facC0", "facX0", "facZ0" },
    { "bulE0", "bulF0", "bulC0", "bulX0", "bulZ0" }
} };

const TownSpriteNameSet castleSnowSprites     = { "casVS", "casFS", "casCS", "casXS", "casZS" };
const TownSpriteNameSet infernoSnowSprites    = { "infVS", "infFS", "infCS", "infXS", "infZS" };
const TownSpriteNameSet necropolisSnowSprites = { "necVS", "necFS", "necCS", "necXS", "necZS" };
const TownSpriteNameSet strongholdSnowSprites = { "strVS", "strFS", "strCS", "strXS", "strZS" };
const TownSpriteNameSet fortressSnowSprites   = { "ftrt0", "forFS", "forCS", "forXS", "forZS" };

const std::array<TownSpriteNameSet*, MAX_HOTA_TOWN_TYPES> snowTownSprites = { {
    &castleSnowSprites,
    nullptr,
    nullptr,
    &infernoSnowSprites,
    &necropolisSnowSprites,
    nullptr,
    &strongholdSnowSprites,
    &fortressSnowSprites,
    nullptr,
    nullptr,
    nullptr,
    nullptr
} };

const TownSpriteNameSet towerGrassSprites = { "towVG", "towFG", "towCG", "towXG", "towZG" };

const std::array<bool, MAX_HOTA_TOWN_TYPES> wfHasFortSprites = { {
	true,  // Castle
	false, // Rampart
	false, // Tower
	true,  // Inferno
	true,  // Necropolis
	false, // Dungeon
	true,  // Stronghold
    true,  // Fortress
    true,  // Conflux
    false,
    false,
    false
} };


static uint32_t GetTownLevel(const town& tw) {
    const uint32_t buildingMask = uint32_t(tw.populationMask);
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


static CStrPtr __fastcall GetTownSpriteName(const NewmapCell& cell, CObjectType& objType) {
    static const char fallbackSpriteName[] = "AVCranx0.def";
    static const char spriteNameTemplate[] = "AVC_____.def\0AVC_____.def";

    if (int32_t(cell.extraInfo) < 0) {
        // This should not have happened
        objType.ImageName.assign(fallbackSpriteName);
        return fallbackSpriteName;
    }

    objType.Type = OBJECT_TOWN;
	const town& tw = gpGame->townPool[cell.get_map_extraInfo().extraInfo];
    uint32_t tl = GetTownLevel(tw);
    const TerrainType8 tt = GetTerrainType(gpGame->worldMap, tw);
	CStrPtr specSpriteName = nullptr;

    if (tw.townType == eTownTower) {
        if (IsOneOf(tt, eTerrainGrass, eTerrainSwamp, eTerrainHighlands)) {
			specSpriteName = towerGrassSprites[tl];
        }
    }
    else if (tt == eTerrainSnow) {
        const TownSpriteNameSet* snowSprites = snowTownSprites[tw.townType];
        if (snowSprites != nullptr) {
            specSpriteName = (*snowSprites)[tl];
        }
    }

    objType.ImageName.assign(spriteNameTemplate, specSpriteName ? 25 : 12);
    if (!Mode::HotA && (tl == 1 || tl == 2) && (!Mode::WogFix || !wfHasFortSprites[tw.townType])) {
        tl = 3; // use original H3 fortified town sprite
    }
    std::memcpy(objType.ImageName.data() + 3, regularTownSprites[tw.townType][tl], 5);

    if (specSpriteName == nullptr) {
        return objType.ImageName.c_str(); // original town sprite
    }
    std::memcpy(objType.ImageName.data() + 16, specSpriteName, 5);
    return objType.ImageName.c_str() + 13; // altered town sprite
}

} // namespace


void AdvMapTownPatch(PatcherInstance& p) {
    Asm::Sequence{
		Asm::SetReg(Asm::EDX, Asm::ESI), // string addres - sprite name
        Asm::PushConst32(0x4C98B7), // return address - before ResourceManager::GetSprite()
        Asm::Jump(GetTownSpriteName)
    }
    .Apply(p, 0x4C9770);
}
