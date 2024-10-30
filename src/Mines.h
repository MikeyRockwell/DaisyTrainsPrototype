#pragma once

#include "Engine.h"
#include "Grid.h"
#include "Rail.h"

namespace Mines
{
    struct Cargo
    {
        Color color;
        i32 count;
    };

    struct Mine
    {
        Vector2Int coordinate;
        Vector2 worldPosition;
        
        bool flipped = false;
        Rail::RailType compatibleRailType;

        Color color;
        Textures::Sprite sprite;

        i32 count = 200;
    };

    struct Station
    {
        Vector2Int coordinate;
        Vector2 worldPosition;

        Rail::RailType compatibleRailType;

        Color color;
        Textures::Sprite sprite;

        bool flipped;
    };

    struct State
    {
        std::unordered_map<Vector2Int, Mine, Vector2IntHash, Vector2IntEqual> mines;
        std::unordered_map<Vector2Int, Station, Vector2IntHash, Vector2IntEqual> stations;
    };
    extern State state;

    void Init(Grid::Grid& grid);
    void CreateMine   (Grid::Cell* cell, i32 capacity, Color color, const char* texturePath, bool flipped);
    void CreateStation(Grid::Cell* cell, Color color, const char* texturePath, bool flipped);
    void Update(Grid::Grid& grid);
    void Draw();
    
    inline bool IsRailCompatible(Grid::Cell* cell)
    {
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