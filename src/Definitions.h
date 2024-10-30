#pragma once

// Define basic types

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef int             i32;

typedef float           f32;
typedef double          f64;

const i32 CELL_SIZE = 64;

// COLOR PALETTE!
const i32 PALETTE_SIZE = 8;

// BRIGHTS
//const Color PALETTE_BLACK  = { 0,   0,   0,   255 };
//const Color PALETTE_BLUE   = { 31,  36,  106, 255 };
//const Color PALETTE_PINK   = { 138, 17,  129, 255 };
//const Color PALETTE_ORANGE = { 209, 68,  68,  255 };
//const Color PALETTE_GREEN  = { 44,  165, 62,  255 };
//const Color PALETTE_CYAN   = { 104, 203, 203, 255 };
//const Color PALETTE_YELLOW = { 227, 199, 45,  255 };
//const Color PALETTE_WHITE  = { 255, 255, 255, 255 };

const Color PALETTE_BLACK   = { 18,   14,   35,   255 };
const Color PALETTE_PURPLE    = { 71,  40,  89, 255 };
const Color PALETTE_TEAL    = { 34, 88,  117, 255 };
const Color PALETTE_PINK  = { 197, 48,  98,  255 };
const Color PALETTE_DARK_GREEN  = { 36, 158, 79,  255 };
const Color PALETTE_ORANGE   = { 242,  137, 65,  255 };
const Color PALETTE_LIGHT_GREEN    = { 180, 220, 58, 255 };
const Color PALETTE_WHITE   = { 255, 241, 169, 255 };



const Color PALETTE[8] =
{
    PALETTE_BLACK,
    PALETTE_PURPLE,
    PALETTE_TEAL,
    PALETTE_PINK,
    PALETTE_ORANGE,
    PALETTE_LIGHT_GREEN,
    PALETTE_DARK_GREEN,
    PALETTE_WHITE
};
