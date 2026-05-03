#include "adv_mt.hpp"
#include "hota_terrain.hpp"


static inline TerrainType8& CellMagicTerrain(NewmapCell& cell) {
    // Use the byte after RoadIndex to store magic terrain mark
    return reinterpret_cast<TerrainType8*>(&cell)[10];
}


static inline TerrainType8 CellMagicTerrain(const NewmapCell& cell) {
    return reinterpret_cast<const TerrainType8*>(&cell)[10];
}


bool __fastcall IsTerrainType(const NewfullMap& map, const CObject& obj, int width, TerrainType8 terType) {
    const NewmapCell* cells = map.cellData + (obj.y + obj.z * map.Size) * map.Size;
    const int startX = std::max(0, obj.x - width + 2);
    const int endX = std::min(obj.x + 1, map.Size);

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


TerrainType8 __fastcall GetTerrainType(const NewfullMap& map, const CObject& obj, int width) {
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


void __fastcall MarkMagicTerrain(NewfullMap& map) {
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
