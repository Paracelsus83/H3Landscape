#include <algorithm>

#include <nh3api/core/map.hpp>
#include <nh3api/core/map_header.hpp>
#include <nh3api/core/resources/resources.hpp>

#include "asm_patch.hpp"
#include "mode.hpp"
#include "types.hpp"
#include "hota_terrain.hpp"


enum TMapVersion : uint32_t {
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
    int16_t mapVersion = 0;
    uint8_t height = 0;
    CStrPtr imageName = nullptr;
};


static const ObjAlterationInfo ObjSnowAlterations1[] = {
    /* GARRISON          */ {},
    /* HERO              */ {},
    /* HILL_FORT         */ { MAPVER_HOTA, 3, "AVXhilS0.def" },
    /* GRAIL             */ {},
    /* HUT_OF_MAGI       */ {},
    /* IDOL_OF_FORTUNE   */ { MAPVER_HOTA, 2, "AVSidlS0.def" },
    /* LEAN_TO           */ {},
    /* DECORATIVE        */ {},
    /* LIBRARY           */ { MAPVER_HOTA, 3, "AVSlibS0.def" }
};

static const ObjAlterationInfo ObjSnowAlterations1ERA[] = {
    /* GARRISON          */ { MAPVER_WOG,  2, "zgarn001.def" },
    /* HERO              */ {},
    /* HILL_FORT         */ { MAPVER_WOG,  3, "zfohill0.def" },
    /* GRAIL             */ {},
    /* HUT_OF_MAGI       */ {},
    /* IDOL_OF_FORTUNE   */ { MAPVER_WOG,  2, "z358o2.def"   },
    /* LEAN_TO           */ {},
    /* DECORATIVE        */ {},
    /* LIBRARY           */ { MAPVER_WOG,  2, "z358o3.def"   }
};

inline const ObjAlterationInfo& ObjSnowAlt1(size_t idx) {
    return (Mode::ERA ? ObjSnowAlterations1ERA : ObjSnowAlterations1)[idx - OBJECT_GARRISON];
}


static const ObjAlterationInfo ObjSnowAlterations2[] = {
    /* TAVERN            */ { MAPVER_HOTA, 4, "AVXtvrS0.def" },
    /* TEMPLE            */ {},
    /* DEN_OF_THIEVES    */ { MAPVER_HOTA, 3, "AvXDenS0.def" },
    /* TOWN              */ {},
    /* TRADING_POST      */ { MAPVER_AB,   3, "AVXpsSn.def"  },
    /* LEARNING_STONE    */ { MAPVER_HOTA, 2, "AVSgzbS0.def" },
    /* TREASURE_CHEST    */ {},
    /* TREE_OF_KNOWLEDGE */ { MAPVER_HOTA, 3, "AVXtrkS0.def" },
    /* SUBTERRANEAN_GATE */ { MAPVER_HOTA, 3, "AvTCave2.def" },
    /* UNIVERSITY        */ {},
    /* WAGON             */ {},
    /* WAR_MACHINE_FACT  */ {},
    /* SCHOOL_OF_WAR     */ {},
    /* WARRIORS_TOMB     */ {},
    /* WATER_WHEEL       */ {},
    /* WATERING_HOLE     */ {},
    /* WHIRLPOOL         */ {},
    /* WINDMILL          */ {},
    /* WITCH_HUT         */ { MAPVER_HOTA, 3, "AVSwtcS0.def" }
};

static const ObjAlterationInfo ObjSnowAlterations2ERA[] = {
    /* TAVERN            */ { MAPVER_WOG,  4, "z358o7.def"   },
    /* TEMPLE            */ {},
    /* DEN_OF_THIEVES    */ { MAPVER_WOG,  2, "zdent001.def" },
    /* TOWN              */ {},
    /* TRADING_POST      */ { MAPVER_AB,   3, "AVXpsSn.def"  },
    /* LEARNING_STONE    */ { MAPVER_WOG,  2, "z358o1.def"   },
    /* TREASURE_CHEST    */ {},
    /* TREE_OF_KNOWLEDGE */ { MAPVER_WOG,  3, "z358o6.def"   },
    /* SUBTERRANEAN_GATE */ { MAPVER_WOG,  3, "z358o5.def"   },
    /* UNIVERSITY        */ {},
    /* WAGON             */ {},
    /* WAR_MACHINE_FACT  */ {},
    /* SCHOOL_OF_WAR     */ {},
    /* WARRIORS_TOMB     */ {},
    /* WATER_WHEEL       */ {},
    /* WATERING_HOLE     */ {},
    /* WHIRLPOOL         */ {},
    /* WINDMILL          */ {},
    /* WITCH_HUT         */ { MAPVER_WOG,  3, "z358o4.def"   }
};

inline const ObjAlterationInfo& ObjSnowAlt2(size_t idx) {
    return (Mode::ERA ? ObjSnowAlterations2ERA : ObjSnowAlterations2)[idx - OBJECT_TAVERN];
}


ObjAlterationInfo AbandonedMineAlterations[] = {
	/* DIRT  */ {},
	/* SAND  */ {},
    /* GRASS */ {},
	/* SNOW  */ { MAPVER_AB, 3, "AVXamSn.def" },
	/* SWAMP */ {},
	/* ROUGH */ {},
	/* SUBTR */ { MAPVER_AB, 2, "AVXamSu.def" },
    /* LAVA  */ { MAPVER_AB, 3, "AVXamLv.def" }
};


static inline TerrainType8& CellMagicTerrain(NewmapCell& cell) {
    // Use the byte after RoadIndex to store magic terrain mark
    return reinterpret_cast<TerrainType8*>(&cell)[10];
}


static inline TerrainType8 CellMagicTerrain(const NewmapCell& cell) {
    return reinterpret_cast<const TerrainType8*>(&cell)[10];
}


static bool __fastcall IsTerrainType(const NewfullMap& map, const CObject& obj, int width, TerrainType8 terType) {
    const NewmapCell* cells = map.cellData + (obj.y + obj.z * map.Size) * map.Size;
    const int startX = std::max(0, obj.x - width + 2);
    const int endX   = std::min(obj.x + 1, map.Size);

    for (int x = startX; x < endX; ++x) {
        const auto& cell = cells[x];
        const TerrainType8 specTerType = CellMagicTerrain(cell);

        if (specTerType < 0) {
            if (cell.GroundSet != terType) return false;
        }
        else if (specTerType != terType) {
            return false;
		}
    }
    return true;
}


static TerrainType8 __fastcall GetTerrainType(const NewfullMap& map, const CObject& obj, int width) {
    const NewmapCell* cells = map.cellData + (obj.y + obj.z * map.Size) * map.Size;
    const int startX = std::max(0, obj.x - width + 2);
    const int endX = std::min(obj.x + 1, map.Size);

    const auto& fCell = cells[startX];
    TerrainType8 terType = CellMagicTerrain(fCell);
    if (terType < 0) {
        terType = fCell.GroundSet;
	}

    for (int x = startX + 1; x < endX; ++x) {
        const auto& cell = cells[x];
        const TerrainType8 specTerType = CellMagicTerrain(cell);

        if (specTerType < 0) {
            if (cell.GroundSet != terType) return eTerrainNone;
        }
        else if (specTerType != terType) {
            return eTerrainNone;
        }
    }
    return terType;
}


static void __fastcall MarkMagicTerrain(NewfullMap& map) {
    for (CObject& obj : map.Objects) {
        const CObjectType& objType = map.ObjectTypes[obj.TypeID];
        const TAdventureObjectType advType = objType.Type;

        TerrainType8 stt;

        if (advType == OBJECT_CURSED_GROUND_ROE) {
			stt = eTerrainRough;
        }
        else if (advType == OBJECT_MAGIC_PLAINS_ROE) {
            stt = eTerrainHighlands;
        }
        else if (advType == OBJECT_HOTA_MAGIC_TERRAIN) {
			static const TerrainType8 hotaMTMap[3] = {
                eTerrainSnow, eTerrainSand, eTerrainDirt
            };
            stt = hotaMTMap[objType.Subtype];
        }
        else if (advType >= OBJECT_CLOVER_FIELD && advType <= OBJECT_ROCKLANDS) {
            static const TerrainType8 sodMTMap[10] = {
                /* CLOVER_FIELD  */ eTerrainHighlands,
                /* CURSED_GROUND */ eTerrainRough,
                /* EVIL_FOG      */ eTerrainLava,
                /* FAVOR_WINDS   */ eTerrainNone,
                /* FIERY_FIELDS  */ eTerrainWasteland,
                /* HOLY_GROUNDS  */ eTerrainHighlands,
                /* LUCID_POOLS   */ eTerrainWater,
                /* MAGIC_CLOUDS  */ eTerrainRock,
                /* MAGIC_PLAINS  */ eTerrainHighlands,
				/* ROCKLANDS     */ eTerrainRough
            };
            stt = sodMTMap[advType - OBJECT_CLOVER_FIELD];
        }
        else {
            continue;
        }

        int h = std::max(6, int(objType.Height));
        int iniX[6];
        int endX[6];
        iniX[1] = std::max(0, obj.x - objType.Width + 1);
        endX[1] = std::min(obj.x + 1, map.Size);

		if (objType.Width > 2 && objType.Height > 2) { // ellipse
            iniX[0] = iniX[1] + 1;
            endX[0] = endX[1] - 1;
            --h;
            iniX[h] = iniX[0];
			endX[h] = endX[0];
        }
        else { // regular rectangle
            iniX[0] = iniX[1];
            endX[0] = endX[1];
		}
        for (int y = 2; y < h; ++y) {
            iniX[y] = iniX[1];
            endX[y] = endX[1];
        }

        const int iniY = std::max(0, obj.y - objType.Height + 1);
        const int endY = std::min(obj.y + 1, map.Size);

        for (int y = iniY, i = 0; y < endY; ++y, ++i) {
            for (int x = iniX[i]; x < endX[i]; ++x) {
                auto& cell = map.cellData[x + map.Size * (y + obj.z * map.Size)];
                CellMagicTerrain(cell) = stt;
            }
        }
    }
}


static exe_vector<std::pair<uint32_t, uint16_t>> objTypeMap;

static uint16_t __fastcall GetOrCreateTypeId(NewfullMap& map, const CObjectType& objType, TerrainType8 terType, ObjAlterationInfo altInfo) {

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
    newObjType.ImageName.assign(altInfo.imageName);
    newObjType.Height = altInfo.height;

    map.Sprites.push_back(ResourceManager::GetSprite(altInfo.imageName));

    return typeId;
}


static void __fastcall UpdateAbandonedMine(NewfullMap& map, CObject& obj, const CObjectType& objType) {
    TerrainType8 tt = GetTerrainType(map, obj, objType.Width);
    if (tt >= eTerrainSnow && tt < eTerrainWater) {
        const ObjAlterationInfo& altMineInfo = AbandonedMineAlterations[tt];
        if (altMineInfo.imageName) {
            obj.TypeID = GetOrCreateTypeId(map, objType, tt, altMineInfo);
        }
    }
}


static void __fastcall UpdateRoEObjectsToSoD(NewfullMap& map) {
    for (CObject& obj : map.Objects) {
        const CObjectType& objType = map.ObjectTypes[obj.TypeID];
        const TAdventureObjectType advType = objType.Type;

        switch (advType) {
        case OBJECT_TRADING_POST:
            if (IsTerrainType(map, obj, objType.Width, eTerrainSnow)) {
                obj.TypeID = GetOrCreateTypeId(map, objType, eTerrainSnow, ObjSnowAlt2(OBJECT_TRADING_POST));
            }
			break;
        case OBJECT_MINE:
            if (objType.Subtype == ABANDONED) {
				UpdateAbandonedMine(map, obj, objType);
            }
            break;
        default:
            break;
        }
    }
}


static void __fastcall UpdateAllObjects(NewfullMap& map, int32_t mapVer) {
    const bool mapVerRoE = (mapVer == MAPVER_ROE);

    for (CObject& obj : map.Objects) {
        const CObjectType& objType = map.ObjectTypes[obj.TypeID];
        const TAdventureObjectType advType = objType.Type;

        if (advType >= OBJECT_TAVERN) {
            if (advType <= OBJECT_WITCH_HUT) {
                if (objType.Subtype == 0 && IsTerrainType(map, obj, objType.Width, eTerrainSnow)) {
                    const ObjAlterationInfo& altInfo = ObjSnowAlt2(advType);
                    if (altInfo.imageName && mapVer < altInfo.mapVersion) {
                        obj.TypeID = GetOrCreateTypeId(map, objType, eTerrainSnow, altInfo);
                    }
                }
            }
        }
        else if (advType >= OBJECT_GARRISON) {
            if (advType <= OBJECT_LIBRARY_OF_ENLIGHTENMENT) {
                if (objType.Subtype == 0 && IsTerrainType(map, obj, objType.Width, eTerrainSnow)) {
                    const ObjAlterationInfo& altInfo = ObjSnowAlt1(advType);
                    if (altInfo.imageName && mapVer < altInfo.mapVersion) {
                        obj.TypeID = GetOrCreateTypeId(map, objType, eTerrainSnow, altInfo);
                    }
                }
			}
            else if (mapVerRoE && advType == OBJECT_MINE && objType.Subtype == ABANDONED) {
                UpdateAbandonedMine(map, obj, objType);
            }
        }
    }
}


static void __fastcall NewMap_UpdateObjects(NewfullMap& map) {
	const NewSMapHeader& mapHeader = reinterpret_cast<NewSMapHeader*>(&map)[-1];

    if (Mode::HotA || Mode::ERA) {
        MarkMagicTerrain(map);
        UpdateAllObjects(map, mapHeader.iVersion);
    }
    else if (mapHeader.iVersion == MAPVER_ROE) {
        MarkMagicTerrain(map);
        UpdateRoEObjectsToSoD(map);
	}
    objTypeMap.clear();

    FASTCALL_1(void, 0x4ED930, true); // IncProgressBar(true)
}


void AdvMapPatch(PatcherInstance& p) {
    Asm::WritePseudoFastCall(p, 0x50505E, NewMap_UpdateObjects, Asm::ESI);

    // Change the NewmapCell constructor:
    // Set the byte following the RoadIndex to -1 (eTerrainNone)
    // This byte is used to mark cells with magic terrain
    p.WriteWord(0x4FDCBA, 0xBA90); // replace AND opcode with NOP and MOV
}
