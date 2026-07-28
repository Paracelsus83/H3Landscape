#pragma once

#include <cstdint>


typedef unsigned char TEffectPattern[24];

struct TEffectDef {
    int32_t upperRow = 400;
    const TEffectPattern* pattern = nullptr;
    bool disableShadow = false;

    bool Enabled() const {
        return pattern != nullptr;
    }

    void Reset() {
        upperRow = 400;
        pattern = nullptr;
        disableShadow = false;
    }
};


extern const TEffectPattern fogGradient;
extern const TEffectPattern waterTransp;
extern const TEffectPattern darkWTransp;

namespace BfScreen {
    extern int32_t firstRow;
    extern int32_t lastRow;
}

namespace Combat {
    extern bool disableEffectInFort;
    extern TEffectDef terrainEffect;
    extern TEffectDef moatEffect;
}

void __fastcall HotA_Sprite_DrawCreature(
    struct CSprite& sprite, // ECX
    int32_t, // EDX
    int32_t seqNum,
    int32_t frameNum,
    int32_t sx, int32_t sy,
    int32_t sw, int32_t sh,
    char* screenMap,
    int32_t x, int32_t y,
    int32_t screenWidth,
    int32_t screenHeight,
    int32_t screenPitch,
    bool horizFlip,
    uint16_t hColor
);

int __fastcall SoD_CM_DrawCreature(
    class combatManager& cm, // ECX
    int32_t frame,  // EDX
    struct CSprite* sprite,
    int32_t sequence,
    const class army& rArmy,
    int32_t x, int32_t y,
    struct SLimitData*,
    int32_t id,
    bool hFlip,
    uint16_t hColor
);
