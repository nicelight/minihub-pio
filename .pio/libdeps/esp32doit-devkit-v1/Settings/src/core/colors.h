#pragma once
#include <Arduino.h>

#define SETS_DEFAULT_COLOR 0xffffffff

namespace sets {

// // базовые
// enum class Colors : uint32_t {
//     Black = 0x000000,
//     Gray = 0x808080,
//     White = 0xffffff,
//     Red = 0xcb2839,
//     Orange = 0xd55f30,
//     Yellow = 0xd69d27,
//     Green = 0x37A93C,
//     Mint = 0x25b18f,
//     Aqua = 0x2ba1cd,
//     Blue = 0x297bcd,
//     Violet = 0x825ae7,
//     Pink = 0xc8589a,
//     Default = SETS_DEFAULT_COLOR,
// };

// постельные
enum class Colors : uint32_t {
    Black = 0x4a4a4a,
    Gray = 0xafafaf,
    White = 0xffffff,
    Red = 0xfca8b0,
    Orange = 0xf8bfa8,
    Yellow = 0xfce1aa,
    Green = 0xaafcae,
    Mint = 0xa8fae6,
    Aqua = 0xa8e4fb,
    Blue = 0xa4cef8,
    Violet = 0xc1a9fd,
    Pink = 0xfaa5d7,
    Default = SETS_DEFAULT_COLOR,
};

}