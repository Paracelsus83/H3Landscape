#pragma once

#include <nh3api/core/terrain.hpp>


constexpr TTerrainType eTerrainHighlands = static_cast<TTerrainType>(10);
constexpr TTerrainType eTerrainWasteland = static_cast<TTerrainType>(11);

constexpr uint32_t MAX_HOTA_TERRAIN_TYPES = 12;

constexpr EMagicTerrain MAGIC_TERRAIN_CRACKED_ICE = static_cast<EMagicTerrain>(11);
constexpr EMagicTerrain MAGIC_TERRAIN_DUNES = static_cast<EMagicTerrain>(12);
constexpr EMagicTerrain MAGIC_TERRAIN_FIELDS_GLORY = static_cast<EMagicTerrain>(13);

constexpr uint32_t MAX_HOTA_MAGIC_TERRAINS = 14;

constexpr TTownType eTownCove = static_cast<TTownType>(9);
constexpr TTownType eTownFactory = static_cast<TTownType>(10);
constexpr TTownType eTownBulwark = static_cast<TTownType>(11);

constexpr uint32_t MAX_HOTA_TOWN_TYPES = 12;


enum ECreatureBankExtType : int16_t {
	BANK_WOG_HUNTING_LODGE            = 11,
	BANK_WOG_SNOW_GROTTO              = 12,
	BANK_WOG_PALACE_OF_MARTIAL_SPIRIT = 13,
	BANK_WOG_CITADEL_OF_PACIFICATION  = 14,
	BANK_WOG_MONASTERY_OF_MAGICIANS   = 15,
	BANK_WOG_LIBRARY_OF_LEGENDS       = 16,
	BANK_WOG_TRANSYLVANIAN_TAVERN     = 17,
	BANK_WOG_HOME_OF_THE_BAT          = 18,
	BANK_WOG_LOST_BOTTLE              = 19,
	BANK_WOG_GROTTO                   = 20,

	BANK_HOTA_BEHOLDERS_SANCTUARY = 21,
	BANK_HOTA_TEMPLE_OF_THE_SEA   = 22,
	BANK_HOTA_PIRATE_CAVERN       = 23,
	BANK_HOTA_MANSION             = 24,
	BANK_HOTA_SPIT                = 25,
	BANK_HOTA_RED_TOWER           = 26,
	BANK_HOTA_BLACK_TOWER         = 27,
	BANK_HOTA_IVORY_TOWER         = 28,
	BANK_HOTA_CHURCHYARD          = 29,
	BANK_HOTA_EXPERIMENTAL_SHOP   = 30,
	BANK_HOTA_WOLF_RAIDER_PICKET  = 31,
	BANK_HOTA_RUINS               = 32
};
