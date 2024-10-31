#include "Mines.h"
#include "Game.h"

namespace Mines
{
    Resources resources;

    void Init()
    {
        
        // MINES
        Texture* texture = Textures::Load("res/sprites/mine_pink.png");
        resources.mineTextures[PINK_CARGO] = texture;
        texture = Textures::Load("res/sprites/mine_orange.png");
        resources.mineTextures[ORANGE_CARGO] = texture;

        // STATIONS
        texture = Textures::Load("res/sprites/station_pink.png");
        resources.stationTextures[PINK_CARGO] = texture;
        texture = Textures::Load("res/sprites/station_orange.png");
        resources.stationTextures[ORANGE_CARGO] = texture;

        // CONVERTERS
        texture = Textures::Load("res/sprites/converter_orange.png");
        resources.converterTextures[ORANGE_CARGO] = texture;
        texture = Textures::Load("res/sprites/converter_pink.png");
        resources.converterTextures[PINK_CARGO] = texture;

        texture = Textures::Load("res/sprites/converter_symbol.png");
        resources.converterSymbolTexture = texture;

        // level 0

        i32 levelNumber = 0;
        Game::Level& level = Game::state.levels[levelNumber];

        /*============================================
            PINK MINE AND STATION
        ============================================*/
        Vector2Int coordinate = { 2,1 };
        CreateMine(levelNumber, &level.grid.cells[coordinate], 200, PINK_CARGO, false);
        coordinate = { 2, 4 };
        CreateStation(levelNumber, &level.grid.cells[coordinate], PINK_CARGO, false);
    }

    void CreateMine(i32 levelNumber, Grid::Cell* cell, i32 capacity, CargoType cargoType, bool vertical)
    {
        State& state = Game::state.levels[levelNumber].mineState;
        Vector2Int coordinate = cell->coordinate;
        state.mines[coordinate] = {};
        Mine& mine = state.mines[coordinate];
        mine.capacity = capacity;
        mine.count = capacity;
        mine.coordinate = coordinate;
        mine.vertical = vertical;
        mine.compatibleRailType = vertical ? Rail::RailType::VERTICAL : Rail::RailType::HORIZONTAL;
        mine.worldPosition = { (float)coordinate.x * CELL_SIZE, (float)coordinate.y * CELL_SIZE };
        mine.cargoType = cargoType;

        cell->hasMine = true;

        mine.stackCoordinate = vertical ? 
            Vector2Int{ coordinate.x + 1, coordinate.y } : 
            Vector2Int{ coordinate.x, coordinate.y - 1 };
        mine.stackWorldPosition = { (float)mine.stackCoordinate.x * CELL_SIZE, (float)mine.stackCoordinate.y * CELL_SIZE };

        Grid::Cell* visualCell = Game::state.levels[levelNumber].grid.CoordinateToCell(mine.stackCoordinate);
        visualCell->buildable = false;
        
        mine.sprite = Textures::CreateSprite
        (
            resources.mineTextures[cargoType],
            { 0,0,CELL_SIZE, CELL_SIZE },
            { CELL_SIZE / 2, CELL_SIZE / 2 },
            0.0f,
            1.0f,
            PALETTE[mine.cargoType]
        );
    }

    void CreateStation(i32 level, Grid::Cell* cell, CargoType cargo, bool flipped)
    {
        Vector2Int coordinate = cell->coordinate;
        Game::state.levels[level].mineState.stations[coordinate] = {};
        Station& station = Game::state.levels[level].mineState.stations[coordinate];

        station.coordinate = coordinate;
        station.vertical = flipped;
        station.compatibleRailType = flipped ? Rail::RailType::VERTICAL : Rail::RailType::HORIZONTAL;
        station.worldPosition = { (float)coordinate.x * CELL_SIZE, (float)coordinate.y * CELL_SIZE };
        station.cargoType = cargo;
        cell->hasStation = true;
        
        station.sprite = Textures::CreateSprite
        (
            resources.stationTextures[cargo],
            { 0,0,CELL_SIZE, CELL_SIZE },
            { CELL_SIZE / 2, CELL_SIZE / 2 },
            0.0f,
            1.0f,
            PALETTE[station.cargoType]
        );
    }

    void CreateConverter(i32 level, Grid::Cell* cell, CargoType input, CargoType output)
    {
        Vector2Int coordinate = cell->coordinate;
        Game::state.levels[level].mineState.converters[coordinate] = {};
        Converter& converter = Game::state.levels[level].mineState.converters[coordinate];

        converter.input = input;
        converter.output = output;
        converter.inputCoordinate = coordinate;
        converter.outputCoordinate = { coordinate.x, coordinate.y - 1 };
        converter.stackCoordinate = { coordinate.x, coordinate.y - 2 };
        converter.inputWorldPosition = { (float)coordinate.x * CELL_SIZE, (float)coordinate.y * CELL_SIZE };
        converter.outputWorldPosition = 
        { 
            (float)converter.outputCoordinate.x * CELL_SIZE, 
            (float)converter.outputCoordinate.y * CELL_SIZE 
        };
        converter.stackWorldPosition =
        {
            (float)converter.stackCoordinate.x * CELL_SIZE,
            (float)converter.stackCoordinate.y * CELL_SIZE
        };

        cell->hasConverter = true;
        // TODO: handle builability for the extra cells...
    }

    void Update()
    {

    }

    void Draw()
    {
        State state = Game::GetCurrentLevel().mineState;

        // DRAW MINES
        for (auto& [coordinate, station] : state.stations)
        {
            Rectangle destination =
            {
                station.vertical ? station.worldPosition.x + CELL_SIZE : station.worldPosition.x,
                station.vertical ? station.worldPosition.y : station.worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };
            DrawTexturePro
            (
                *station.sprite.texture,
                station.sprite.source,
                destination,
                { 0, 0 },
                station.vertical ? 90.0f : 0.0f,
                WHITE
            );
        }
        // DRAW STATIONS
        for (auto& [coordinate, mine] : state.mines)
        {
            Rectangle destination =
            {
                mine.vertical ? mine.worldPosition.x + CELL_SIZE : mine.worldPosition.x,
                mine.vertical ? mine.worldPosition.y : mine.worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };
            DrawTexturePro
            (
                *mine.sprite.texture,
                mine.sprite.source,
                destination,
                { 0, 0 },
                mine.vertical ? 90.0f : 0.0f,
                WHITE
            );
            Rectangle visualDestination =
            {
                mine.stackWorldPosition.x,
                mine.stackWorldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };

            // 64 is the number of rectangles to draw in the mine visual (for a full mine)
            // get the percentage of the mine that is full
            float percentage = (float)mine.count / (float)mine.capacity;
            // get how many rectangles to draw to represent the mine with 64 / 8 rectangles per row and column
            i32 rectanglesToDraw = (i32)(64 * percentage);
            // draw the rectangles from the top right corner
            for (int y = 8 - 1; y >= 0; y--)
            {
                for (int x = 0; x < 8; x++)
                {
                    if (rectanglesToDraw <= 0) break;

                    Rectangle rect =
                    {
                        visualDestination.x + (CELL_SIZE - 8) - x * 8,
                        visualDestination.y + y * 8,
                        8,
                        8
                    };
                    DrawRectangleRec(rect, PALETTE[mine.cargoType]);
                    DrawRectangleLinesEx(rect, 1, BLACK);
                    rectanglesToDraw--;
                }
                if (rectanglesToDraw <= 0) break;
            }

            //DrawRectangleRec(visualDestination, PALETTE[mine.cargoType]);
            /*Vector2 textPosition = 
            { 
                mine.visualWorldPosition.x,
                mine.visualWorldPosition.y + CELL_SIZE - 20
            };
            std::string mineCount = std::to_string(mine.count);
            DrawText(mineCount.c_str(), textPosition.x, textPosition.y, 20, BLACK);*/

            // DRAW CONVERTERS
            for (auto& [coordinate, converter] : state.converters)
            {
                Rectangle destination =
                {
                    converter.inputWorldPosition.x,
                    converter.inputWorldPosition.y,
                    CELL_SIZE,
                    CELL_SIZE
                };
                DrawTexturePro
                (
                    *resources.converterTextures[converter.input],
                    { 0,0,CELL_SIZE,CELL_SIZE },
                    destination,
                    { 0,0 },
                    0.0f,
                    WHITE
                );
                destination =
                {
                    converter.outputWorldPosition.x,
                    converter.outputWorldPosition.y,
                    CELL_SIZE,
                    CELL_SIZE
                };
                DrawTexturePro
                (
                    *resources.converterSymbolTexture,
                    { 0,0,CELL_SIZE,CELL_SIZE },
                    destination,
                    { 0,0 },
                    0.0f,
                    PALETTE[converter.output]
                );
                
                // 64 is the number of rectangles to draw in the mine visual (for a full mine)
                // get the percentage of the mine that is full
                float percentage = (float)converter.outputCount / (float)converter.capacity;
                // get how many rectangles to draw to represent the mine with 64 / 8 rectangles per row and column
                i32 rectanglesToDraw = (i32)(64 * percentage);
                // draw the rectangles from the top right corner
                for (int y = 8 - 1; y >= 0; y--)
                {
                    for (int x = 0; x < 8; x++)
                    {
                        if (rectanglesToDraw <= 0) break;

                        Rectangle rect =
                        {
                            converter.stackWorldPosition.x + (CELL_SIZE - 8) - x * 8,
                            converter.stackWorldPosition.y + y * 8,
                            8,
                            8
                        };
                        DrawRectangleRec(rect, PALETTE[mine.cargoType]);
                        DrawRectangleLinesEx(rect, 1, BLACK);
                        rectanglesToDraw--;
                    }
                    if (rectanglesToDraw <= 0) break;
                }
            }
        }
    }
}