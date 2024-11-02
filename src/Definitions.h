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

const Color PALETTE_BLACK        = { 0,   0,   0,   255 };
const Color PALETTE_DARK_GRAY    = { 61,  61,  61, 255 };
const Color PALETTE_PINK         = { 214, 23,  198, 255 };
const Color PALETTE_ORANGE       = { 243, 118,  50,  255 };
const Color PALETTE_GREEN        = { 65, 224, 144,  255 };
const Color PALETTE_BLUE         = { 34,  104, 210,  255 };
const Color PALETTE_LIGHT_GRAY   = { 181, 181, 181, 255 };
const Color PALETTE_WHITE        = { 238, 238, 238, 255 };

const Color PALETTE[8] =
{
    PALETTE_BLACK,
    PALETTE_DARK_GRAY,
    PALETTE_PINK,
    PALETTE_ORANGE,
    PALETTE_GREEN,
    PALETTE_BLUE,
    PALETTE_LIGHT_GRAY,
    PALETTE_WHITE
};

enum CargoType
{
    BLACK_CARGO,
    DARK_GRAY_CARGO,
    PINK_CARGO,
    ORANGE_CARGO,
    GREEN_CARGO,
    BLUE_CARGO,
    LIGHT_GRAY_CARGO,
    WHITE_CARGO
};

