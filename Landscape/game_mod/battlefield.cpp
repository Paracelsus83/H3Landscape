#include <nh3api/core/combat.hpp>
#include <nh3api/core/global.hpp>

#include "battlefield.hpp"
#include "asm_helper.h"


namespace Addr { // Function and data addresses inside Heroes3.exe

constexpr uintptr_t FUNC_RANDOM = 0x50C7C0; // "random" function

namespace Bg { // Addresses inside the function getting battlefield background
    constexpr uintptr_t GET_TOWN_BF_BG = 0x4642BE;
    constexpr uintptr_t GET_AREA_BF_BG = 0x4642DA;
    constexpr uintptr_t STD_BF_BG = 0x46431E;
    constexpr uintptr_t END_OF_FUNC = 0x46435E;
}

namespace Obst { // Address inside the function setting obstacles on the battlefield
	constexpr uintptr_t HERO_ON_BOAT = 0x4662E6;
	constexpr uintptr_t TWO_BOATS = 0x4662F0;
    constexpr uintptr_t STD_PLACEMENT = 0x466387;
}

namespace Img { // file names of battlefield background images
	constexpr uintptr_t TownBfBackgr = 0x63D2A0;   // array of town backgrounds
	constexpr uintptr_t MagicBfBackgr = 0x63D2C8;  // array of magic backgrounds
	constexpr uintptr_t CoastBfBackgr = 0x66FF40;  // coast background
	constexpr uintptr_t BoatDeckBackgr = 0x66FF4C; // boat background
	constexpr uintptr_t TwoBoatsBackgr = 0x66FF5C; // boat vs boat background
}

} // namespace Addr


typedef const char* CStrPtr;

#define H3TownBfBackgr reinterpret_cast<CStrPtr*>(Addr::Img::TownBfBackgr)
#define H3MagicBfBackgr reinterpret_cast<CStrPtr*>(Addr::Img::MagicBfBackgr)

#define H3CoastBfBackgr reinterpret_cast<CStrPtr>(Addr::Img::CoastBfBackgr)
#define H3BoatDeckBackgr reinterpret_cast<CStrPtr>(Addr::Img::BoatDeckBackgr)
#define H3TwoBoatsBackgr reinterpret_cast<CStrPtr>(Addr::Img::TwoBoatsBackgr)

/* Background of the battlefield on the surface - "Red Rocks" */
CStrPtr RedRocksBackgr = "CmBkRedMt.pcx";

/* Backgrounds of underground battlefields */
const CStrPtr BfUndBackgr[MAX_TERRAIN_TYPES] = {
    /* eTerrainDirt    */ "CmBkDrUg.pcx",
    /* eTerrainSand    */ "CmBkDesUg.pcx",
    /* eTerrainGrass   */ "CmBkGrUg.pcx",
    /* eTerrainSnow    */ "CmBkSnUg.pcx",
    /* eTerrainSwamp   */ "CmBkSwUg.pcx",
    /* eTerrainRough   */ "CmBkRghUg.pcx",
    /* eTerrainSubter. */ "CmBkSub.pcx",
    /* eTerrainLava    */ "CmBkLvUg.pcx",
    /* eTerrainWater   */ "CmBkDkUg.pcx",
    /* eTerrainRock    */ "CmBkSub.pcx"
};

/* Backgrounds of underground magic battlefields */
const CStrPtr MagicBfUndBackgr[10] = {
    /* MAGIC_TERRAIN_COAST         */ "CmBkLkUg.pcx",
    /* MAGIC_TERRAIN_MAGIC_PLAINS  */ "CmBkMgUg.pcx",
    /* MAGIC_TERRAIN_CURSED_GROUND */ "CmBkRghUg.pcx",
    /* MAGIC_TERRAIN_HOLY_GROUND   */ "CmBkHGUg.pcx",
    /* MAGIC_TERRAIN_EVIL_FOG      */ "CmBkEFUg.pcx",
    /* MAGIC_TERRAIN_CLOVER_FIELD  */ "CmBkCFUg.pcx",
    /* MAGIC_TERRAIN_LUCID_POOLS   */ "CmBkLPUg.pcx",
    /* MAGIC_TERRAIN_FIERY_FIELDS  */ "CmBkFFUg.pcx",
    /* MAGIC_TERRAIN_ROCKLANDS     */ "CmBkRkUg.pcx",
    /* MAGIC_TERRAIN_MAGIC_CLOUDS  */ "CmBkMCUg.pcx"
};

CStrPtr MagicCloudBoatBackgr = "CmBkMCDk.pcx";

/* Backgrounds of underground town battlefields */
const CStrPtr TownBfUndBackgr[kNumTowns] = {
    /* eTownCastle     */ "SgCsUgBk.pcx",
    /* eTownRampart    */ "SgRmUgBk.pcx",
    /* eTownTower      */ "SgTwUgBk.pcx",
    /* eTownInferno    */ "SgInUgBk.pcx",
    /* eTownNecropolis */ "SgNcUgBk.pcx",
    /* eTownDungeon    */ "SgDnBack.pcx",
    /* eTownStronghold */ "SgStUgBk.pcx",
    /* eTownFortress   */ "SgFrUgBk.pcx",
    /* eTownConflux    */ "SgElUgBk.pcx"
};


CODE_PATCH GetTownBfBackgr(combatManager* cm) {
/* input:
    ECX: address of combatManager object - cm
    EAX: fortification level - fortLvl
*/
    TFortificationLevel fortLvl;
    GET_FROM_REG(fortLvl, eax);

    const TTownType tt = TTownType(cm->combatTown->townType);
    const bool underground = (cm->map_point.z != 0);
    CStrPtr bgStrPtr = nullptr;

    switch (cm->magic_terrain) {

    case MAGIC_TERRAIN_INVALID:
        if ((tt == eTownNecropolis) && (cm->EventCell->GroundSet == eTerrainSnow)) {
            bgStrPtr = underground ? TownBfUndBackgr[eTownTower] : H3TownBfBackgr[eTownTower];
        }
        break;
    case MAGIC_TERRAIN_CURSED_GROUND:
        if (tt == eTownNecropolis || tt == eTownStronghold || tt == eTownConflux) {
            bgStrPtr = underground ? TownBfUndBackgr[eTownStronghold] : H3TownBfBackgr[eTownStronghold];
        }
        break;
    case MAGIC_TERRAIN_CLOVER_FIELD:
        if (tt == eTownCastle || tt == eTownRampart || tt == eTownFortress || tt == eTownConflux) {
            bgStrPtr = underground ? "SgCFUgBk.pcx" : "SgCFBack.pcx";
        }
        break;
    case MAGIC_TERRAIN_FIERY_FIELDS:
        if (tt == eTownInferno) {
            bgStrPtr = "SgFFBack.pcx";
        }
        break;
    case MAGIC_TERRAIN_ROCKLANDS:
        if (tt == eTownCastle || tt == eTownInferno || tt == eTownNecropolis || tt == eTownConflux) {
            bgStrPtr = underground ? "SgRkUgBk.pcx" : "SgRkBack.pcx";
        }
        break;
    case MAGIC_TERRAIN_MAGIC_CLOUDS:
        if (tt == eTownTower) {
            bgStrPtr = underground ?
                MagicBfUndBackgr[MAGIC_TERRAIN_MAGIC_CLOUDS] : H3MagicBfBackgr[MAGIC_TERRAIN_MAGIC_CLOUDS];
        }
        break;
    default:
        break;
    }

    if (bgStrPtr == nullptr) {
        bgStrPtr = (underground ? TownBfUndBackgr : H3TownBfBackgr)[tt];
    }

    PATCH_RETURN(bgStrPtr, Addr::Bg::END_OF_FUNC);
}


static bool IsCave(const combatManager* cm) {
    switch (cm->Heroes[0]->type) {

    case OBJECT_CREATURE_BANK: {
            return cm->EventCell->objectIndex == 0; // Cyclops Stockpile
        }
    case OBJECT_CREATURE_GENERATOR1: {
            const int16_t genType = cm->EventCell->objectIndex;
            return genType == 1 || genType == 9; // Behemoth Crag or Cyclops Cave
        }
    case OBJECT_MINE: {
            const int16_t res = cm->EventCell->objectIndex;
			return res == CRYSTAL || res == GOLD || res == ABANDONED;
		}
    default:
        break;
    }
    return false;
}


static int32_t CaveTerrain(const combatManager* cm) {
    switch (cm->Heroes[0]->type) {

    case OBJECT_CREATURE_BANK:
        // Creature bank type == 0 means Cyclops Stockpile
        if (cm->EventCell->objectIndex == 0) {
            const int32_t tt = cm->combatTerrain;
            // If terrain type is Grass or Snow, change it to Rough
            return (tt == eTerrainGrass || tt == eTerrainSnow) ? eTerrainRough : tt;
        }
        break;

    case OBJECT_CREATURE_GENERATOR1:
        // Creature generator types: 1 means Behemoth Crag, 9 means Cyclops Cave
        if (int16_t genType = cm->EventCell->objectIndex; genType == 1 || genType == 9) {
            const int8_t tt = cm->EventCell->GroundSet;
            // If terrain type is Grass or Snow, change it to Rough
			return (tt == eTerrainGrass || tt == eTerrainSnow) ? eTerrainRough : tt;
        }
        break;

    case OBJECT_MINE:
        switch (cm->EventCell->objectIndex) {
        case ABANDONED:
            return eTerrainSubterranean;
        case CRYSTAL:
        case GOLD:
            const int8_t tt = cm->EventCell->GroundSet;
            return (tt == eTerrainGrass || tt == eTerrainSnow || tt == eTerrainSwamp) ? eTerrainDirt : tt;
        }
        break; // case OBJECT_MINE

    default:
        break;
    }
    return eTerrainNone;
}


static bool IsNearWater(type_point mapPoint) {
#define IF_WATER_RETURN_TRUE(point) \
    if (gm->get_cell(point)->GroundSet == eTerrainWater) return true

    const game* gm = *reinterpret_cast<game**>(0x699538);

    if (mapPoint.x > 0) {
        type_point mapPointLeft = mapPoint;
        --mapPointLeft.x;
        IF_WATER_RETURN_TRUE(mapPointLeft);
    }
    if (mapPoint.y > 0) {
        type_point mapPointUp = mapPoint;
        --mapPointUp.y;
        IF_WATER_RETURN_TRUE(mapPointUp);
    }
    const int32_t mapBorder = gm->worldMap.Size - 1;
    if (mapPoint.x < mapBorder) {
        type_point mapPointRight = mapPoint;
        ++mapPointRight.x;
        IF_WATER_RETURN_TRUE(mapPointRight);
    }
    if (mapPoint.y < mapBorder) {
        type_point mapPointDown = mapPoint;
        ++mapPointDown.y;
        IF_WATER_RETURN_TRUE(mapPointDown);
    }
    return false;

#undef IF_WATER_RETURN_TRUE
}


CODE_PATCH GetAreaBfBackgr(EMagicTerrain mt, combatManager* cm) {
/* input:
    ECX: type of magic terrain - mt
    ESI: address of combatManager object - cm
*/
    GET_FROM_REG(cm, esi);

    CStrPtr bgStrPtr;
    const bool underground = (cm->map_point.z != 0);

    if (cm->Heroes[0]->flags & HF_ISINBOAT) {
        const hero* enemyHero = cm->Heroes[1];
        if (enemyHero && (enemyHero->flags & HF_ISINBOAT)) {
            bgStrPtr = H3TwoBoatsBackgr; // Set background for battle between two boats
        }
        else { /* Set background for battle on one boat */
            bgStrPtr = (mt == MAGIC_TERRAIN_MAGIC_CLOUDS) ? MagicCloudBoatBackgr :
                underground ? BfUndBackgr[eTerrainWater] : H3BoatDeckBackgr;
        }
    }
    else if (mt > MAGIC_TERRAIN_COAST) {
        bgStrPtr = ((underground || IsCave(cm)) ? MagicBfUndBackgr : H3MagicBfBackgr)[mt];
    }
    else if (int32_t ctt = CaveTerrain(cm); ctt != eTerrainNone) {
        cm->combatTerrain = ctt;
        bgStrPtr = BfUndBackgr[ctt];
    }
    else if (mt == MAGIC_TERRAIN_COAST) {
        bgStrPtr = H3CoastBfBackgr;
    }
    else if (underground) {
        if (IsNearWater(cm->map_point)) {
            // Set the battlefield background to underground lake
			bgStrPtr = MagicBfUndBackgr[MAGIC_TERRAIN_COAST];
        }
        else {
            const int8_t tt = cm->EventCell->GroundSet;
            cm->combatTerrain = tt;
            bgStrPtr = BfUndBackgr[tt];
        }
    }
    else if (cm->combatTerrain == eTerrainSubterranean) {
        bgStrPtr = RedRocksBackgr; // Set the battlefield background to Red Rocks
    }
    else {
        bgStrPtr = nullptr;
    }

    if (bgStrPtr != nullptr) {
		SET_EBX(Addr::Bg::END_OF_FUNC);
	}
    else { // Go to standard H3 procedure for non-magic terrain
        SET_EBX(Addr::Bg::STD_BF_BG);
    }
    PATCH_RETURN(bgStrPtr, ebx);
}


ASM_CODE_PATCH Obstacles_HeroOnBoat() {
/* input:
    ECX:   address of second hero object
    FlagE: address of second hero is NULL
	EDX:   constant - hero flag HF_ISINBOAT
*/
    __asm {
		je OneBoat   // if ECX == 0 (no second hero) => OneBoat
		test dword ptr[ecx + 0x105], edx // check if the second hero has flag HF_ISINBOAT
        je OneBoat   // if the second hero is not on the boat => OneBoat
		jmp Addr::Obst::TWO_BOATS // return to the h3 code - handling two boats
        
    OneBoat:
        mov  edx, 0x0C
        mov  ecx, 5
        call Addr::FUNC_RANDOM // get a random number in the range [5..0xC]
	    xor ebx, ebx    // ebx = 0
        mov  dword ptr[ebp - 0x10], eax // save random number
        mov  dword ptr[ebp - 0x14], ebx
        mov  ebx, 0x100 // 1<<8 - water flag
		jmp  Addr::Obst::STD_PLACEMENT // return to the h3 code
    }
}


void BattleBackgroundPatch(PatcherInstance & p) {
    p.WriteJmp(Addr::Bg::GET_TOWN_BF_BG, uintptr_t(GetTownBfBackgr));
    p.WriteJmp(Addr::Bg::GET_AREA_BF_BG, uintptr_t(GetAreaBfBackgr));
    p.WriteJmp(Addr::Obst::HERO_ON_BOAT, uintptr_t(Obstacles_HeroOnBoat));

    // Setting the background image for the dungeon battlefield on the surface
    H3TownBfBackgr[eTownDungeon] = "SgDnSfBk.pcx";
}


/*
* Assembler code for the function in Heroes3_HD.exe that returns the name of the battlefield background file.
* 
* input:  ESI = the address of the combatManager object.
* output: EAX = the address of the background file name (string).
* 
004642B0    jmp         {far addr} --> jump to the HDmod code (it returns to addr 004642BA)
004642B5    nop
...
004642B9    nop

<- the HDmod code returns to that address
004642BA    test        eax,eax         * EAX is the level of the town walls
004642BC    jle         004642D4        * If there are no wals, go to CHECK_MAGIC_TERRAIN

Addr::Bg::GET_TOWN_BF_BG:
004642BE    code patch here --> jump to function GetTownBfBackgr
* original instruction:
* 004642BE  mov         eax,dword ptr [esi+53C8h]   * EAX = address of the town object

004642C4    movsx       ecx,byte ptr [eax+4]        * ECX = town type
004642C8    mov         eax,dword ptr [ecx*4+63D2A0h]
004642CF    jmp         0046435E        -> go to Addr::END_OF_FUNC

CHECK_MAGIC_TERRAIN:
004642D4    mov         ecx,dword ptr [esi+53C0h]   * ECX = magic terrain type
Addr::GET_AREA_BF_BG:
004642DA    code patch here --> jump to function GetAreaBfBackgr
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
00464303    je          0046431E        * If the hero is not on the boat, go to Addr::Bg::STD_BF_BG
00464305    mov         eax,dword ptr [esi+53D0h]   * EAX = address of the enemy hero object
0046430B    test        eax,eax
0046430D    je          0046431E
0046430F    test        dword ptr [eax+105h],edx    * check the enemy hero's flags
00464315    je          0046431E        * If the enemy hero is not on the boat, go to Addr::Bg::STD_BF_BG
00464317    mov         eax,66FF5Ch     * EAX = address of the battlefield background with two boats
0046431C    jmp         0046435E        -> go to Addr::END_OF_FUNC

Addr::Bg::STD_BF_BG:  <- return from the GetAreaBfBackgr function
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
