#include "Mines.h"

namespace Mines
{
    State state;

    void Init(Grid::Grid& grid)
    {
        /*============================================
            PINK MINE
        ============================================*/
        Vector2Int coordinate = { 12,1 };
        CreateMine(&grid.cells[coordinate], 2000, PALETTE_TEAL, "res/sprites/mine_pink.png", false);

        /*============================================
            PINK STATION
        ============================================*/
        coordinate = { 12, 9 };
        CreateStation(&grid.cells[coordinate], PALETTE_TEAL, "res/sprites/station_pink.png", false);
    }

    void CreateMine(Grid::Cell* cell, i32 capacity, Color color, const char* texturePath, bool flipped)
    {
        Vector2Int coordinate = cell->coordinate;
        state.mines[coordinate] = {};
        Mine& mine = state.mines[coordinate];

        mine.coordinate = coordinate;
        mine.count = capacity;
        mine.flipped = flipped;
        mine.compatibleRailType = flipped ? Rail::RailType::VERTICAL : Rail::RailType::HORIZONTAL;
        mine.worldPosition = { (float)coordinate.x * CELL_SIZE, (float)coordinate.y * CELL_SIZE };
        mine.color = color;

        cell->hasMine = true;
        
        Texture2D* texture = Textures::Load(texturePath);
        mine.sprite = Textures::CreateSprite
        (
            texture,
            { 0,0,CELL_SIZE, CELL_SIZE },
            { CELL_SIZE / 2, CELL_SIZE / 2 },
            0.0f,
            1.0f,
            mine.color
        );
    }

    void CreateStation(Grid::Cell* cell, Color color, const char* texturePath, bool flipped)
    {
        Vector2Int coordinate = cell->coordinate;
        state.stations[coordinate] = {};
        Station& station = state.stations[coordinate];

        station.coordinate = coordinate;
        station.flipped = flipped;
        station.compatibleRailType = flipped ? Rail::RailType::VERTICAL : Rail::RailType::HORIZONTAL;
        station.worldPosition = { (float)coordinate.x * CELL_SIZE, (float)coordinate.y * CELL_SIZE };
        station.color = color;
        cell->hasStation = true;
        Texture* texture = Textures::Load(texturePath);
        station.sprite = Textures::CreateSprite
        (
            texture,
            { 0,0,CELL_SIZE, CELL_SIZE },
            { CELL_SIZE / 2, CELL_SIZE / 2 },
            0.0f,
            1.0f,
            station.color
        );
    }

    void Update(Grid::Grid& grid)
    {

    }

    void Draw()
    {
        for (auto& [coordinate, station] : state.stations)
        {
            Rectangle destination =
            {
                station.flipped ? station.worldPosition.x + CELL_SIZE : station.worldPosition.x,
                station.flipped ? station.worldPosition.y : station.worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };
            DrawTexturePro
            (
                *station.sprite.texture,
                station.sprite.source,
                destination,
                { 0, 0 },
                station.flipped ? 90.0f : 0.0f,
                WHITE
            );
        }

        for (auto& [coordinate, mine] : state.mines)
        {
            Rectangle destination =
            {
                mine.flipped ? mine.worldPosition.x + CELL_SIZE : mine.worldPosition.x,
                mine.flipped ? mine.worldPosition.y : mine.worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };
            DrawTexturePro
            (
                *mine.sprite.texture,
                mine.sprite.source,
                destination,
                { 0, 0 },
                mine.flipped ? 90.0f : 0.0f,
                WHITE
            );
            Vector2 textPosition = 
            { 
                mine.worldPosition.x, 
                mine.worldPosition.y - CELL_SIZE / 2 - 10
            };
            std::string mineCount = std::to_string(mine.count);
            DrawText(mineCount.c_str(), textPosition.x, textPosition.y, 30, PALETTE_TEAL);
        }



    }
}