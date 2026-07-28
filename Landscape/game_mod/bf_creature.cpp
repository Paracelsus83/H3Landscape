#include "bf_creature.hpp"

#include <nh3api/hd_mod.hpp>
#include <nh3api/core/combat.hpp>
#include <nh3api/core/interface/window_manager.hpp>

#include "asm_helper.h"
#include "pixel_rgb.hpp"
#include "types.hpp"


namespace BfScreen {
    int32_t firstRow;
    int32_t lastRow;
}

constexpr int PIXEL_SIZE = sizeof(color32_t);

const TEffectPattern fogGradient = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
const TEffectPattern airGradient = { 3, 4, 5, 6, 7, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
const TEffectPattern waterTransp = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8 };
const TEffectPattern darkWTransp = { 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8 };


template<int ALPHA, int ALPHA_H, bool SHADOW, bool hasEffect>
inline static char* DrawFrameRLE_S(
    char* __restrict screenDest, int screenPitch, int firstRow, int endOfRows,
    const CSpriteFrame* __restrict frameData, ptrdiff_t iterDirection,
    const Pixel32* __restrict pal32, Pixel32 highlightColor,
    [[ maybe_unused ]] const TEffectPattern* __restrict effectPattern = nullptr
)
{
    #define REPEAT_CONT(_oper) \
        for (; count > 0; --count, dst += iterDirection) { reinterpret_cast<Pixel32*>(dst)->_oper; } continue

    const uint32_t* lineOffsets = reinterpret_cast<const uint32_t*>(frameData->map) + firstRow;
    const int numOfRows = endOfRows - firstRow;

    for (int i = 0; i < numOfRows; ++i, screenDest += screenPitch) {
        const uint8_t* input = frameData->map + lineOffsets[i];
        char* dst = screenDest;

        for (int j = frameData->CroppedWidth; j > 0; ) {
            const uint8_t code = *input++;
            int count = *input++ + 1;
            j -= count;

            switch (code) {
            case 0:
                break;
            case 5:
                if (highlightColor.NonZero()) {
                    REPEAT_CONT(MixH<ALPHA_H>(highlightColor));
                }
                break;
            case 6:
                if (highlightColor.NonZero()) {
                    REPEAT_CONT(MixH<ALPHA_H>(highlightColor));
                }
                [[fallthrough]];
            case 4:
                if constexpr (SHADOW) {
                    if constexpr (!hasEffect) {
                        REPEAT_CONT(MakeFaction<8>());
                    }
                    else {
                        switch (i) {
                        case 0: REPEAT_CONT(MakeFaction<10>());
                        case 1: REPEAT_CONT(MakeFaction<12>());
                        case 2: REPEAT_CONT(MakeFaction<14>());
                        }
                    }
                }
                break;
            case 7:
                if (highlightColor.NonZero()) {
                    REPEAT_CONT(MixH<ALPHA_H>(highlightColor));
                }
                [[fallthrough]];
            case 1:
                if constexpr (SHADOW) { // light shadow
                    if constexpr (hasEffect) {
                        if (i > 0) break;
                    }
                    REPEAT_CONT(MakeFaction<12>());
                }
                break;
            case 0xFF:
                if constexpr (!hasEffect) {
                    REPEAT_CONT(Mix<ALPHA>(pal32[*input++]));
                }
                else {
                    switch ((*effectPattern)[i]) {
                    case 0: REPEAT_CONT(Mix<15>(pal32[*input++]));
                    case 1: REPEAT_CONT(Mix<14>(pal32[*input++]));
                    case 2: REPEAT_CONT(Mix<12>(pal32[*input++]));
                    case 3: REPEAT_CONT(Mix<10>(pal32[*input++]));
                    case 4: REPEAT_CONT(Mix<8>(pal32[*input++]));
                    case 5: REPEAT_CONT(Mix<6>(pal32[*input++]));
                    case 6: REPEAT_CONT(Mix<4>(pal32[*input++]));
                    case 7: REPEAT_CONT(Mix<2>(pal32[*input++]));
                    case 8: REPEAT_CONT(Mix<1>(pal32[*input++]));
                    default: input += count;
                    }
                }
                break;
            default:
                if constexpr (ALPHA == 16) {
                    const Pixel32 pixel32(pal32[code]);
                    REPEAT_CONT(Mix<16>(pixel32));
                }
                else {
                    Pixel32 pixel32(pal32[code]);
                    pixel32.MakeFaction<ALPHA>();
                    REPEAT_CONT(MixH<ALPHA>(pixel32));
                }
            }
            if (iterDirection > 0) dst += PIXEL_SIZE * count; else dst -= PIXEL_SIZE * count;
        }
    }
    return screenDest;

    #undef REPEAT
}


template<int ALPHA, int ALPHA_H, bool HAS_EFFECT, class... Args>
inline static char* DrawFrameRLE(bool disableShadow, Args... args) {
    if (disableShadow) {
        return DrawFrameRLE_S<ALPHA, ALPHA_H, false, HAS_EFFECT>(args...);
    }
    return DrawFrameRLE_S<ALPHA, ALPHA_H, true, HAS_EFFECT>(args...);
}


inline static int GridRowNumber(int n) {
    return ((n << 4) - n) >> 8;
}


inline static TEffectDef GetFortEffect(const army& rArmy) {
    
    const int pos = rArmy.gridIndex - combatManager::moatCell[GridRowNumber(rArmy.gridIndex)];
    if (pos > 0) {
        if (Combat::disableEffectInFort) return TEffectDef();
    }
    else {
        const auto& cm = *gpCombatManager;
        if (pos == 0) { // moat
            if (rArmy.gridIndex == 95 && cm.iDoorStatus != combatManager::DOOR_UP) {
                return TEffectDef();
            }
            if (cm.bMoatOn) {
                if (Combat::moatEffect.Enabled()) {
                    if (!rArmy.Is(CF_DOUBLE_WIDE) || (cm.moatIsWide && !rArmy.facing)) {
                        return Combat::moatEffect;
                    }
                }
                return TEffectDef();
            }
        }
        else if (pos == -1) {
            if (rArmy.gridIndex == 94 && cm.iDoorStatus != combatManager::DOOR_UP) {
                return TEffectDef();
            }
            if (cm.moatIsWide && Combat::moatEffect.Enabled()) {
                if (!rArmy.Is(CF_DOUBLE_WIDE) || rArmy.facing) {
                    return Combat::moatEffect;
                }
            }
        }
    }

    return Combat::terrainEffect;
}


static void DrawCreatureFrameRLE(
    const army& rArmy,
    char* __restrict screenDest,
    int screenX, int screenY,
    int screenPitch,
    const CSpriteFrame* __restrict frameData,
    const Pixel32* __restrict pal32,
    bool horizFlip,
    bool corpse,
    color16_t hColor
)
{
    const TEffectDef effect = gpCombatManager->fortificationLevel ? GetFortEffect(rArmy) : Combat::terrainEffect;

	int offsetY = screenY + frameData->CroppedY;
    int frameHeight = frameData->CroppedHeight;

	const int bottomMargin = BfScreen::lastRow - offsetY - frameHeight;
    if (bottomMargin < 0) {
		frameHeight += bottomMargin;
    }

    int firstFrameRow = BfScreen::firstRow - offsetY;
    if (firstFrameRow > 0) {
        offsetY = BfScreen::firstRow;
    }
    else {
        firstFrameRow = 0;
    }

    screenDest += offsetY * screenPitch;
    screenDest += PIXEL_SIZE * (screenX + (horizFlip ? (frameData->Width - frameData->CroppedX - 1) : frameData->CroppedX));

    const ptrdiff_t iterDirection = horizFlip ? -PIXEL_SIZE : PIXEL_SIZE;

    const bool solid = !IsOneOf(rArmy.armyType, CREATURE_AIR_ELEMENTAL, CREATURE_STORM_ELEMENTAL);

    Pixel32 highlightColor(hColor);

    if (corpse && effect.Enabled()) {
        DrawFrameRLE<10, 16, false>(effect.disableShadow, screenDest, screenPitch, firstFrameRow, frameHeight,
            frameData, iterDirection, pal32, highlightColor);
    }
    else {
        const int regularRows = std::min(frameHeight, effect.upperRow - frameData->CroppedY);

        if (solid) {
            screenDest = DrawFrameRLE<16, 16, false>(effect.disableShadow, screenDest, screenPitch, firstFrameRow, regularRows,
                frameData, iterDirection, pal32, highlightColor);
        }
        else {
            screenDest = DrawFrameRLE<10, 16, false>(effect.disableShadow, screenDest, screenPitch, firstFrameRow, regularRows,
                frameData, iterDirection, pal32, highlightColor);
        }

        if (effect.Enabled()) {
            highlightColor.MakeFaction<8>();

            const TEffectPattern* effectPattern = (solid || effect.pattern != &fogGradient) ? effect.pattern : &airGradient;

            DrawFrameRLE<8, 8, true>(effect.disableShadow, screenDest, screenPitch, regularRows, frameHeight,
                frameData, iterDirection, pal32, highlightColor, effectPattern);
        }
    }
}


inline static const Pixel32* GetRGBPalette(const CSprite& sprite) {
    return reinterpret_cast<const Pixel32*>(reinterpret_cast<const HD::Palette*>(sprite.p16)->pal32);
}


void __fastcall HotA_Sprite_DrawCreature(
    CSprite& sprite,
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
    color16_t hColor)
{
    const CSequence* seq = sprite.s[seqNum];
    const CSpriteFrame* frameData = seq->f[frameNum];

    if (frameData->EncodingMethod != eEncodeGeneralRLE) {
        sprite.DrawCreature(seqNum, frameNum, sx, sy, sw, sh,
            screenMap, x, y, screenWidth, screenHeight, screenPitch, horizFlip, hColor);
    }

    DrawCreatureFrameRLE(
        *reinterpret_cast<army*>(PARAM32_STACK[16]),
        screenMap,
        x - sx,
        y - sy,
        screenPitch,
        frameData,
        GetRGBPalette(sprite),
        horizFlip,
        (seqNum == 5) && (frameNum == seq->numFrames -1),
        hColor
    );
}


int __fastcall SoD_CM_DrawCreature(
    combatManager& cm, // ECX
    int32_t frameNum,  // EDX
    CSprite* sprite,
    int32_t seqNum,
    const army& rArmy, // EBX
    int32_t x, int32_t y,
    SLimitData* limitData,
    [[ maybe_unused ]] int32_t hexId,
    bool horizFlip,
    color16_t hColor)
{
    if (cm.SaveBiggestExtent || cm.LimitToExtent) {
        SLimitData data;
        if (!limitData) limitData = &data;

        cm.ComputeExtent(sprite, seqNum, frameNum, x, y, limitData, horizFlip, cm.SaveBiggestExtent);
        if (cm.ComputeExtentOnly) return 0;

        if (cm.LimitToExtent
            && (limitData->MinX > cm.Extent.MaxX
             || limitData->MaxX < cm.Extent.MinX
             || limitData->MinY > cm.Extent.MaxY
             || limitData->MaxY < cm.Extent.MinY)
        ) {
            return 0;
        }
    }

    const CSequence* seq = sprite->s[seqNum];
    const CSpriteFrame* frameData = seq->f[frameNum];

    if (frameData->EncodingMethod != eEncodeGeneralRLE) {
        return 1;
    }

    const Bitmap16Bit* screen = gpWindowManager->screenBitmap;

    DrawCreatureFrameRLE(
        rArmy,
        reinterpret_cast<char*>(screen->map),
        (screen->Width >> 1) - 400 + x,
        BfScreen::firstRow + y,
        screen->Pitch,
        frameData,
        GetRGBPalette(*sprite),
        horizFlip,
        (seqNum == 5) && (frameNum == seq->numFrames - 1),
        hColor
    );
    return 1;
}
