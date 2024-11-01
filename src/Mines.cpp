#include "Mines.h"
#include "Game.h"

namespace Mines
{
    MineState state;
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

        resources.mineGhostTexture = Textures::Load("res/sprites/mine_ghost.png");
        resources.mineGhostSprite = Textures::CreateSprite
        (
            resources.mineGhostTexture, 
            { 0,0,CELL_SIZE, CELL_SIZE }, 
            { CELL_SIZE / 2, CELL_SIZE / 2 }, 
            0.0f, 1.0f, WHITE
        );
        resources.stationGhostTexture = Textures::Load("res/sprites/station_ghost.png");
        resources.stationGhostSprite = Textures::CreateSprite
        (
            resources.stationGhostTexture,
            { 0,0,CELL_SIZE, CELL_SIZE },
            { CELL_SIZE / 2, CELL_SIZE / 2 },
            0.0f, 1.0f, WHITE
        );

        // level 0

        i32 levelNumber = 0;
        Grid::Grid& grid   = Game::state.grid;


    }

    Mine* CreateMine(i32 levelNumber, Grid::Cell* cell, i32 capacity, CargoType cargoType, bool vertical)
    {
        Vector2Int coordinate = cell->coordinate;
        state.mines[coordinate] = {};
        Mine& mine = state.mines[coordinate];
        mine.coordinate = coordinate;
        mine.vertical = vertical;
        mine.compatibleRailType = vertical ? Rail::RailType::VERTICAL : Rail::RailType::HORIZONTAL;
        mine.worldPosition = { (float)coordinate.x * CELL_SIZE, (float)coordinate.y * CELL_SIZE };
        mine.cargoType = cargoType;

        cell->hasMine = true;
        mine.sprite = Textures::CreateSprite
        (
            resources.mineTextures[cargoType],
            { 0,0,CELL_SIZE, CELL_SIZE },
            { CELL_SIZE / 2, CELL_SIZE / 2 },
            0.0f,
            1.0f,
            PALETTE[mine.cargoType]
        );

        return &mine;
    }

    Station* CreateStation(i32 level, Grid::Cell* cell, CargoType cargo, bool flipped)
    {
        Vector2Int coordinate = cell->coordinate;
        state.stations[coordinate] = {};
        Station& station = state.stations[coordinate];

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

        return &station;
    }

    Stack* CreateStack(Vector2Int coordinate, CargoType cargo, i32 capacity, i32 count)
    {
        Stack& stack = state.stacks[coordinate];
        stack.cargoType = cargo;
        stack.capacity = capacity;
        stack.count = count;
        stack.coordinate = coordinate;
        stack.worldPosition = { (float)stack.coordinate.x * CELL_SIZE, (float)stack.coordinate.y * CELL_SIZE };

        return &stack;
    }

    void Update(i32 level)
    {
        Vector2 mouseWorldPosition = Game::state.mouseWorldPosition;
        Grid::Grid& grid = Game::state.grid;
        Grid::Cell* cell = grid.GetCellAtWorldPosition(mouseWorldPosition);
        state.buildableCell = false;

        if (GetMouseWheelMove() != 0)
        {
            state.flipped = !state.flipped;
        }

        if (UI::state.buildType == UI::BuildType::MINE)
        {
            if ( cell->initialized && 
                !cell->hasMine && 
                 cell->buildable &&
                Mines::IsRailCompatible())
            {
                Stack* stack = nullptr;
                for (int x = -1; x < 2; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        Vector2Int coordinate = { cell->coordinate.x + x, cell->coordinate.y + y };
                        if (state.stacks.find(coordinate) != state.stacks.end())
                        {
                            stack = &state.stacks[coordinate];
                            state.buildableCell = true;
                            break;
                        }
                    }
                }

                if (IsMouseButtonPressed(0) && stack != nullptr && state.minesAvailable > 0)
                {
                    Mine* mine = CreateMine(level, cell, 200, stack->cargoType, state.flipped);
                    mine->stack = stack;
                    state.minesAvailable--;
                    cell->buildable = false;
                    cell->hasMine = true;
                }
            }
            else if (cell->hasMine && IsMouseButtonPressed(1))
            {
                state.mines.erase(cell->coordinate);
                cell->hasMine= false;
                if (cell->railType == -1) cell->buildable = true;
                state.minesAvailable++;
            }
        }

        if (UI::state.buildType == UI::BuildType::STATION)
        {
            if (cell->initialized && !cell->hasStation && cell->buildable)
            {
                Stack* stack = nullptr;
                for (int x = -1; x < 2; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        Vector2Int coordinate = { cell->coordinate.x + x, cell->coordinate.y + y };
                        if (state.stacks.find(coordinate) != state.stacks.end())
                        {
                            stack = &state.stacks[coordinate];
                            state.buildableCell = true;
                            break;
                        }
                    }
                }
                if (IsMouseButtonPressed(0) && stack != nullptr && state.stationsAvailable > 0)
                {
                    Station* station = CreateStation(level, cell, stack->cargoType, state.flipped);
                    station->stack = stack;
                    state.stationsAvailable--;
                    cell->buildable = false;
                    cell->hasStation = true;
                }
            }
            else if (cell->hasStation && IsMouseButtonPressed(1))
            {
                state.stations.erase(cell->coordinate);
                cell->hasStation = false;
                if (cell->railType == -1) cell->buildable = true;
                state.stationsAvailable++;
            }
        }
    }

    bool IsRailCompatible(Grid::Cell* cell)
    {
        if (cell->initialized == false)
        {
            return false;
        }
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

    void Draw(i32 level)
    {
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

            /*if (station.hasStack)
            {
                DrawStack(*station.stack);
            }*/
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
                mine.stack->worldPosition.x,
                mine.stack->worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };
        }

        // DRAW STACKS
        for (auto& [coordinate, stack] : state.stacks)
        {
            DrawStack(stack);
        }

        // BUILDING GHOST
        Vector2 mouseWorldPosition = Game::state.mouseWorldPosition;
        Grid::Cell* cell = Game::state.grid.GetCellAtWorldPosition(mouseWorldPosition);

        if (UI::state.buildType == UI::BuildType::MINE)
        {
            if (cell->initialized && !cell->hasMine && cell->buildable)
            {
                Color ghostColor = state.buildableCell ? PALETTE_GREEN : PALETTE_ORANGE ;
                Rectangle bounds =
                {
                    state.flipped ? cell->worldPosition.x + CELL_SIZE : cell->worldPosition.x,
                    state.flipped ? cell->worldPosition.y : cell->worldPosition.y,
                    CELL_SIZE,
                    CELL_SIZE
                };
                DrawTexturePro
                (
                    *resources.mineGhostTexture,
                    { 0,0,CELL_SIZE, CELL_SIZE },
                    bounds,
                    { 0,0 },
                    state.flipped ? 90.0f : 0.0f,
                    ghostColor
                );
            }
        }

        if (UI::state.buildType == UI::BuildType::STATION)
        {
            if (cell->initialized && !cell->hasStation && cell->buildable)
            {
                Color ghostColor = state.buildableCell ? PALETTE_GREEN : PALETTE_ORANGE;
                Rectangle bounds =
                {
                    state.flipped ? cell->worldPosition.x + CELL_SIZE : cell->worldPosition.x,
                    state.flipped ? cell->worldPosition.y : cell->worldPosition.y,
                    CELL_SIZE,
                    CELL_SIZE
                };
                DrawTexturePro
                (
                    *resources.stationGhostTexture,
                    { 0,0,CELL_SIZE, CELL_SIZE },
                    bounds,
                    { 0,0 },
                    state.flipped ? 90.0f : 0.0f,
                    ghostColor
                );
            }
        }
    }

    void DrawStack(Stack& stack)
    {
        // 64 is the number of rectangles to draw in the mine visual (for a full mine)
        // get the percentage of the mine that is full
        float percentage = (float)stack.count / (float)stack.capacity;
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
                    stack.worldPosition.x + (CELL_SIZE - 8) - x * 8,
                    stack.worldPosition.y + y * 8,
                    8,
                    8
                };
                DrawRectangleRec(rect, PALETTE[stack.cargoType]);
                DrawRectangleLinesEx(rect, 1, BLACK);
                rectanglesToDraw--;
            }
            if (rectanglesToDraw <= 0) break;
        }

        DrawRectangleLinesEx({ stack.worldPosition.x, stack.worldPosition.y, CELL_SIZE, CELL_SIZE }, 2.0f / Game::state.camera.rlCamera.zoom, PALETTE[stack.cargoType]);
    }
}