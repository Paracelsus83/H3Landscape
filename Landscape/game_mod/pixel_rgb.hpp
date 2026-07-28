#pragma once

#include <cstdint>


typedef uint16_t color16_t;
typedef uint32_t color32_t;


struct Pixel32 {
    color32_t value;

    explicit constexpr Pixel32(color16_t c16) : value(((c16 << 8) & 0xF80000) | ((c16 << 5) & 0xFC00) | ((c16 << 3) & 0xF8)) {}

    explicit constexpr Pixel32(color32_t c32) : value(c32) {}

    constexpr bool NonZero() const noexcept { return value; }

    template<uint8_t H>
    void MakeFaction();

    template<uint8_t ALPHA>
    void Mix(Pixel32 other) {
        other.MakeFaction<ALPHA>();
        MakeFaction<16 - ALPHA>();
        value += other.value;
    }

    template<uint8_t ALPHA>
    void MixH(Pixel32 other) {
        MakeFaction<16 - ALPHA>();
        value += other.value;
    }
};


template<>
inline void Pixel32::MakeFaction<1>() {
    value >>= 4;
    value &= 0x0F0F0F;
}

template<>
inline void Pixel32::MakeFaction<2>() {
    value >>= 3;
    value &= 0x1F1F1F;
}

template<>
inline void Pixel32::MakeFaction<4>() {
    value >>= 2;
    value &= 0x3F3F3F;
}

template<>
inline void Pixel32::MakeFaction<8>() {
    value >>= 1;
    value &= 0x7F7F7F;
}

template<>
inline void Pixel32::MakeFaction<6>() {
    MakeFaction<8>(); // half
    const color32_t o = value >> 2;
    value -= (o & 0x1F1F1F);
}

template<>
inline void Pixel32::MakeFaction<10>() {
    MakeFaction<8>(); // half
    const color32_t o = value >> 2;
    value += (o & 0x1F1F1F);
}

template<>
inline void Pixel32::MakeFaction<12>() {
    const color32_t q = value >> 2;
    value -= (q & 0x3F3F3F);
}

template<>
inline void Pixel32::MakeFaction<14>() {
    const color32_t o = value >> 3;
    value -= (o & 0x1F1F1F);
}

template<>
inline void Pixel32::MakeFaction<15>() {
    const color32_t h = value >> 4;
    value -= (h & 0x0F0F0F);
}

template<>
inline void Pixel32::Mix<16>(Pixel32 other) {
    value = other.value;
}

template<>
inline void Pixel32::MixH<16>(Pixel32 other) {
    value = other.value;
}
