#include <nh3api/core/map_header.hpp>

#include "adv_map.hpp"
#include "adv_mt.hpp"
#include "asm_patch.hpp"
#include "hota_terrain.hpp"
#include "mode.hpp"


enum EMapVersion : uint16_t {
    EXTRA_OBJ   = 0x00,
    MAPVER_ROE  = 0x0E,
    MAPVER_AB   = 0x15,
    MAPVER_SOD  = 0x1C,
    MAPVER_HOTA = 0x20,
    MAPVER_WOG  = 0x33
};

struct ObjExtraType {
    TerrainType8 terrainType;
    uint8_t advObjType;
    uint16_t advObjSubtype;
};

static_assert(sizeof(ObjExtraType) == 4);

struct ObjAlterationInfo {
    EMapVersion version = EXTRA_OBJ;
    uint8_t height = 0;
    uint8_t eraHeight = 0;
    std::string_view imageName;
    std::string_view eraImageName = {};
};

enum TerrainFlags : uint16_t {
    fTerrainDirt    = 1 << eTerrainDirt,
    fTerrainSand    = 1 << eTerrainSand,
    fTerrainGrass   = 1 << eTerrainGrass,
    fTerrainSnow    = 1 << eTerrainSnow,
    fTerrainSwamp   = 1 << eTerrainSwamp,
    fTerrainRough   = 1 << eTerrainRough,
    fTerrainSubter  = 1 << eTerrainSubterranean,
    fTerrainLava    = 1 << eTerrainLava,
    fTerrainWater   = 1 << eTerrainWater,
    fTerrainHighlands = 1 << eTerrainHighlands,
    fTerrainWasteland = 1 << eTerrainWasteland,
    fTerrainDead      = fTerrainSubter | fTerrainLava,
    fTerrainVeryDry   = fTerrainRough | fTerrainWasteland,
    fTerrainDryOrUnd  = fTerrainVeryDry | fTerrainSubter,
    fTerrainAny     = 0xFFF,
    fTerrainNoWater = fTerrainAny & ~(fTerrainSwamp | fTerrainWater)
};

namespace Alt {
    constexpr auto FIRST_ADV_TYPE = OBJECT_DRAGON_UTOPIA;
    constexpr auto LAST_ADV_TYPE = OBJECT_WITCH_HUT;
}

struct ObjAltSetup {
    struct Selector {
        size_t objType;
        TerrainFlags terrainTypes;
        constexpr Selector(TAdventureObjectType advType, TerrainFlags tTypes) :
            objType(advType - Alt::FIRST_ADV_TYPE), terrainTypes(tTypes) {
        }
    } key;
    ObjAlterationInfo info;
};


const ObjAlterationInfo AbandonedMineAlterations[] = {
    /* DIRT  */ {},
    /* SAND  */ {},
    /* GRASS */ {},
    /* SNOW  */ { MAPVER_AB, 3, 0, "AVXamSn.def" },
    /* SWAMP */ {},
    /* ROUGH */ {},
    /* SUBTR */ { MAPVER_AB, 2, 0, "AVXamSu.def" },
    /* LAVA  */ { MAPVER_AB, 3, 0, "AVXamLv.def" }
};

static const ObjAlterationInfo SactLandAlteration = {
    EXTRA_OBJ, 3, 0, "Sanct_Sd.def"
};

static const ObjAlterationInfo CavernSnowAlteration = {
    EXTRA_OBJ, 3, 0, "HAPCavSn.def"
};

static const ObjAlterationInfo RuinsSnowAlteration = {
    EXTRA_OBJ, 3, 0, "AVXruiSn.def"
};


static exe_vector<std::pair<uint32_t, uint16_t>> objTypeMap;

static uint16_t __fastcall GetOrCreateTypeId(
    const CObjectType& objType, TerrainType8 terType, NewfullMap& map, const ObjAlterationInfo& altInfo) {

    const uint32_t key = nh3api::bit_cast<uint32_t>(ObjExtraType{
        terType, uint8_t(objType.Type), uint16_t(objType.Subtype)
    });

    auto it = std::lower_bound(objTypeMap.begin(), objTypeMap.end(), key,
        [](const auto& e, uint32_t k) { return e.first < k; });

    if (it != objTypeMap.end() && it->first == key) {
        return it->second;
    }

    const uint16_t typeId = (uint16_t)map.ObjectTypes.size();
    objTypeMap.insert(it, { key, typeId });

    CObjectType& newObjType = map.ObjectTypes.emplace_back(objType);
    newObjType.Height = (Mode::ERA && altInfo.eraHeight) ? altInfo.eraHeight : altInfo.height;

    std::string_view imageName = (Mode::ERA && !altInfo.eraImageName.empty()) ?
        altInfo.eraImageName : altInfo.imageName;

    if (altInfo.version != EXTRA_OBJ) {
        newObjType.ImageName.assign(imageName);
    }
    else {
        newObjType.ImageName.push_back('\0');
        newObjType.ImageName.append(imageName);
    }

    map.Sprites.push_back(ResourceManager::GetSprite(imageName.data()));

    return typeId;
}


template<typename... Args>
struct TObjectAlterations {
    static constexpr size_t IDX_SIZE = Alt::LAST_ADV_TYPE - Alt::FIRST_ADV_TYPE + 1;

    uint8_t altMap[IDX_SIZE][MAX_HOTA_TERRAIN_TYPES] = {};
    const ObjAlterationInfo definition[sizeof...(Args)];

    constexpr explicit TObjectAlterations(Args&&... args) : definition{ args.info... } {
        uint8_t idx = 0;
        for (auto key : { args.key... }) {
            ++idx;
            for (int i = 0, tf = key.terrainTypes; tf != 0; tf >>= 1, ++i) {
                if (tf & 1) altMap[key.objType][i] = idx;
            }
        }
    }

    bool HasAlteration(TAdventureObjectType advType) const {
        return *reinterpret_cast<const uint64_t*>(altMap[advType - Alt::FIRST_ADV_TYPE]);
    }

    const ObjAlterationInfo* GetAlterationInfo(TAdventureObjectType advType, int tt) const {
        const size_t idx = altMap[advType - Alt::FIRST_ADV_TYPE][tt];
        return idx ? &definition[idx - 1] : nullptr;
    }
};


static constexpr TObjectAlterations ObjAlterations{
    ObjAltSetup{ { OBJECT_DRAGON_UTOPIA,     fTerrainSnow      }, { EXTRA_OBJ,   5, 0, "UtopiaSn.def"  } },
    ObjAltSetup{ { OBJECT_GARDEN_OF_REVELATION,fTerrainSnow    }, { EXTRA_OBJ,   2, 0, "GoRsnow.def"   } },
    ObjAltSetup{ { OBJECT_GARDEN_OF_REVELATION,fTerrainDead    }, { EXTRA_OBJ,   2, 0, "GoRlava.def"   } },
    ObjAltSetup{ { OBJECT_HILL_FORT,         fTerrainSand      }, { EXTRA_OBJ,   3, 0, "AVXhilSd.def"  } },
    ObjAltSetup{ { OBJECT_HILL_FORT,         fTerrainSnow      }, { MAPVER_HOTA, 3, 3, "AVXhilS0.def", "zfohill0.def" } },
    ObjAltSetup{ { OBJECT_HILL_FORT,         fTerrainSubter    }, { EXTRA_OBJ,   3, 0, "AVXhilU0.def"  } },
    ObjAltSetup{ { OBJECT_IDOL_OF_FORTUNE,   fTerrainSnow      }, { MAPVER_HOTA, 2, 2, "AVSidlS0.def", "z358o2.def"   } },
    ObjAltSetup{ { OBJECT_LIBRARY_OF_ENLIGHTENMENT,fTerrainSnow}, { MAPVER_HOTA, 3, 2, "AVSlibS0.def", "z358o3.def"   } },
    ObjAltSetup{ { OBJECT_STABLES,           fTerrainSnow      }, { EXTRA_OBJ,   2, 0, "AVXstbl1.def"  } },
    ObjAltSetup{ { OBJECT_TAVERN,            fTerrainSnow      }, { MAPVER_HOTA, 4, 4, "AVXtvrS0.def", "z358o7.def"   } },
    ObjAltSetup{ { OBJECT_TEMPLE,            fTerrainSnow      }, { EXTRA_OBJ,   2, 0, "AVStmpl1.def"  } },
    ObjAltSetup{ { OBJECT_DEN_OF_THIEVES,    fTerrainSnow      }, { MAPVER_HOTA, 3, 2, "AvXDenS0.def", "zdent001.def" } },
    ObjAltSetup{ { OBJECT_TRADING_POST,      fTerrainSnow      }, { MAPVER_AB,   3, 0, "AVXpsSn.def"   } },
    ObjAltSetup{ { OBJECT_LEARNING_STONE,    fTerrainSnow      }, { MAPVER_HOTA, 2, 2, "AVSgzbS0.def", "z358o1.def"   } },
    ObjAltSetup{ { OBJECT_TREE_OF_KNOWLEDGE, fTerrainSnow      }, { MAPVER_HOTA, 3, 3, "AVXtrkS0.def", "z358o6.def"   } },
    ObjAltSetup{ { OBJECT_TREE_OF_KNOWLEDGE, fTerrainVeryDry   }, { EXTRA_OBJ,   3, 0, "AVXtrek3.def"  } },
    ObjAltSetup{ { OBJECT_TREE_OF_KNOWLEDGE, fTerrainDead      }, { EXTRA_OBJ,   3, 0, "AVXtrkLv.def"  } },
    ObjAltSetup{ { OBJECT_SUBTERRANEAN_GATE, fTerrainSnow      }, { MAPVER_HOTA, 3, 3, "AVTCave2.def", "z358o5.def"   } },
    ObjAltSetup{ { OBJECT_UNIVERSITY,        fTerrainSnow      }, { EXTRA_OBJ,   3, 0, "AVSuniv1.def"  } },
    ObjAltSetup{ { OBJECT_WATER_WHEEL,       fTerrainDryOrUnd  }, { EXTRA_OBJ,   3, 0, "AVMwwhRg.def"  } },
    ObjAltSetup{ { OBJECT_WITCH_HUT,         fTerrainSnow      }, { MAPVER_HOTA, 3, 3, "AVSwtcS0.def", "z358o4.def"   } }
};


static void __fastcall UpdateAllObjects(NewfullMap& map, int32_t mapVer) {
    const bool mapVerRoE = (mapVer == MAPVER_ROE);
    const int32_t supportedVer = Mode::ERA ? MAPVER_WOG : Mode::HotA ? MAPVER_HOTA : MAPVER_SOD;

    for (CObject& obj : map.Objects) {
        const CObjectType& objType = map.ObjectTypes[obj.TypeID];
        const TAdventureObjectType advType = objType.Type;

        if (advType >= Alt::FIRST_ADV_TYPE) {
            if (advType <= Alt::LAST_ADV_TYPE) {
                if (objType.Subtype == 0) {
                    if (ObjAlterations.HasAlteration(advType)) {
                        const TerrainType8 tt = GetTerrainType(map, obj, objType.Width);
                        if (tt >= 0) {
                            const ObjAlterationInfo* altInfo = ObjAlterations.GetAlterationInfo(advType, tt);
                            if (altInfo) {
                                const EMapVersion objVer = altInfo->version;
                                if (!objVer || (objVer > mapVer && objVer <= supportedVer)) {
                                    obj.TypeID = GetOrCreateTypeId(objType, tt, map, *altInfo);
                                }
                            }
                        }
                    }
                }
                else if (mapVerRoE && advType == OBJECT_MINE && objType.Subtype == ABANDONED) {
                    const TerrainType8 tt = GetTerrainType(map, obj, objType.Width);
                    if (tt >= eTerrainSnow && tt < eTerrainWater) {
                        const ObjAlterationInfo& altMineInfo = AbandonedMineAlterations[tt];
                        if (!altMineInfo.imageName.empty()) {
                            obj.TypeID = GetOrCreateTypeId(objType, tt, map, altMineInfo);
                        }
                    }
                }
            }
        }
        else if (advType == OBJECT_CREATURE_BANK) {
            switch (objType.Subtype) {
            case BANK_HOTA_BEHOLDERS_SANCTUARY:
                if (const TerrainType8 tt = GetTerrainType(map, obj, objType.Width);
                    tt != eTerrainNone && tt != eTerrainSwamp && tt != eTerrainWater) {
                    obj.TypeID = GetOrCreateTypeId(objType, eTerrainDirt, map, SactLandAlteration);
                }
                break;
            case BANK_HOTA_PIRATE_CAVERN:
                if (IsTerrainType(map, obj, objType.Width, eTerrainSnow)) {
                    obj.TypeID = GetOrCreateTypeId(objType, eTerrainSnow, map, CavernSnowAlteration);
                }
                break;
            case BANK_HOTA_RUINS:
                if (IsTerrainType(map, obj, objType.Width, eTerrainSnow)) {
                    obj.TypeID = GetOrCreateTypeId(objType, eTerrainSnow, map, RuinsSnowAlteration);
                }
                break;
            }
        }
    }
}


inline void IncProgressBar() {
    FASTCALL_1(void, 0x4ED930, true);
}


static void __fastcall NewMap_UpdateObjects(NewfullMap& map) {
    const NewSMapHeader& mapHeader = reinterpret_cast<NewSMapHeader*>(&map)[-1];

    MarkMagicTerrain(map);
    UpdateAllObjects(map, mapHeader.iVersion);
    objTypeMap.clear();

    IncProgressBar();
}



static CSprite* __fastcall GetSpriteForObject(const exe_string& imageName) {
    if (imageName.size() > 14) {
        const size_t altNameOffs = strlen(imageName.data()) + 1;
        if (altNameOffs < imageName.size()) {
            CSprite* altSprite = ResourceManager::GetSprite(imageName.data() + altNameOffs);
            if (altSprite) {
                return altSprite;
            }
        }
    }
    return ResourceManager::GetSprite(imageName.c_str());
}


static void __fastcall SavedMap_LoadSprites(NewfullMap& map) {
    const size_t numObjTypes = map.ObjectTypes.size();
    const size_t progressBarStep = numObjTypes / 3;

    for (size_t i = 0; i < numObjTypes;) {
        map.Sprites[i] = GetSpriteForObject(map.ObjectTypes[i].ImageName);
        ++i;
        if (i % progressBarStep == 0) {
            IncProgressBar();
        }
    }
}


static int __fastcall ReadString(TGzFile& infile, exe_string& str) {
    uint16_t strLength;
    if (infile.read(&strLength, 2) < 2) {
        return -1;
    }
    if (strLength == 0) {
        str.clear();
    }
    else {
        str.resize(strLength);
        if (infile.read(str.data(), strLength) < strLength) {
            return -1;
        }
    }
    return strLength;
}


static int __fastcall WriteString(TGzFile& outfile, const exe_string& str) {
    int strLength = str.size();
    if (outfile.write(&strLength, 2) < 2) {
        return -1;
    }
    if (strLength > 0) {
        if (outfile.write(str.data(), strLength) < strLength) {
            return -1;
        }
    }
    return strLength;
}


void AdvMapPatch(PatcherInstance& p) {
    Asm::WritePseudoFastCall(p, 0x50505E, NewMap_UpdateObjects, Asm::ESI);
    Asm::WritePseudoFastCall(p, 0x50541A, SavedMap_LoadSprites, Asm::ESI, 0x5054E0);
    Asm::WritePseudoFastCall(p, 0x504563, ReadString);  // reading sprite name from GM file
    Asm::WritePseudoFastCall(p, 0x5042A3, WriteString); // writing sprite name to GM file

    // Change the NewmapCell constructor:
    // Set the byte following the RoadIndex to -1 (eTerrainNone)
    // This byte is used to mark cells with magic terrain
    p.WriteWord(0x4FDCBA, 0xBA90); // replace AND opcode with NOP and MOV
}
