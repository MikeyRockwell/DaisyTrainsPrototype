#pragma once

#include "Engine.h"
#include "Grid.h"
#include "Rail.h"

namespace Mines
{
    struct Cargo
    {
        CargoType cargoType;
        i32 count;
    };

    // FOR PICKUPS
    struct Mine
    {
        CargoType cargoType;

        Vector2Int coordinate;
        Vector2 worldPosition;

        Vector2Int stackCoordinate;
        Vector2 stackWorldPosition;
        
        bool vertical = false;
        Rail::RailType compatibleRailType;

        Textures::Sprite sprite;

        i32 capacity = 200;
        i32 count = 200;
    };

    // FOR DELIVERIES
    struct Station
    {
        CargoType cargoType;

        Vector2Int coordinate;
        Vector2 worldPosition;

        Rail::RailType compatibleRailType;

        Textures::Sprite sprite;

        bool vertical = false;
    };

    // FOR CONVERTING
    struct Converter
    {
        Vector2Int inputCoordinate;
        Vector2Int outputCoordinate;
        Vector2Int stackCoordinate;

        Vector2 inputWorldPosition;
        Vector2 outputWorldPosition;
        Vector2 stackWorldPosition;

        CargoType input;
        CargoType output;
        i32 outputCount = 0;
        i32 capacity = 200;
    };

    struct State
    {
        std::unordered_map<Vector2Int, Mine,      Vector2IntHash, Vector2IntEqual> mines;
        std::unordered_map<Vector2Int, Station,   Vector2IntHash, Vector2IntEqual> stations;
        std::unordered_map<Vector2Int, Converter, Vector2IntHash, Vector2IntEqual> converters;
        
    };
    
    struct Resources
    {
        Texture* mineTextures[PALETTE_SIZE];
        Texture* stationTextures[PALETTE_SIZE];
        Texture* converterTextures[PALETTE_SIZE];
        Texture* converterSymbolTexture;
    };
    extern Resources resources;

    void Init();
    void CreateMine     (i32 level, Grid::Cell* cell, i32 capacity, CargoType cargo, bool flipped);
    void CreateStation  (i32 level, Grid::Cell* cell, CargoType cargo, bool flipped);
    void CreateConverter(i32 level, Grid::Cell* cell, CargoType input, CargoType output);
    void Update();
    void Draw();
    
    inline bool IsRailCompatible(Grid::Cell* cell)
    {
        State& state = Game::state.levels[cell->level].mineState;

        if (cell->hasMine)
        {
            Mine* mine = &state.mines[cell->coordinate];
            if (mine->compatibleRailType != Rail::uiState.selectedType)
            {
                return false;
            }
        }
        if (cell->hasStation)
        {
            Station* station = &state.stations[cell->coordinate];
            if (station->compatibleRailType != Rail::uiState.selectedType)
            {
                return false;
            }
        }
        return true;
    }
}