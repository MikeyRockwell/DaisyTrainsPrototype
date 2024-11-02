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

const Color PALETTE_BLACK        = { 89,  88,  93,  255 };
const Color PALETTE_YELLOW       = { 254, 220, 125, 255 };
const Color PALETTE_PINK         = { 236, 165, 203, 255 };
const Color PALETTE_ORANGE       = { 217, 91,  57,  255 };
const Color PALETTE_GREEN        = { 101, 128, 108, 255 };
const Color PALETTE_PURPLE       = { 189, 175, 208, 255 };
const Color PALETTE_LIGHT_GRAY   = { 151, 151, 151, 255 };
const Color PALETTE_WHITE        = { 238, 238, 238, 255 };

const Color PALETTE[8] =
{
    PALETTE_BLACK,
    PALETTE_YELLOW,
    PALETTE_PINK,
    PALETTE_ORANGE,
    PALETTE_GREEN,
    PALETTE_PURPLE,
    PALETTE_LIGHT_GRAY,
    PALETTE_WHITE
};

enum CargoType
{
    BLACK_CARGO,
    YELLOW_CARGO,
    PINK_CARGO,
    ORANGE_CARGO,
    GREEN_CARGO,
    PURPLE_CARGO,
    LIGHT_GRAY_CARGO,
    WHITE_CARGO
};

#define EMPTY_CARGO BLACK_CARGO
