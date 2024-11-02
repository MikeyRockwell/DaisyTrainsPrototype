#include "Rail.h"
#include "Trains.h"

#include <iostream>
#include "Mines.h"
#include "Game.h"

namespace Rail
{
    RailState railState;
    UIState uiState;

    const i32 UI_BLOCK_SIZE  = 64;

    void Init()
    {
        for (int i = 0; i < RAIL_TYPES; i++)
        {
            i32 screenCenter = GetScreenWidth() / 2;
            i32 x = screenCenter - (UI_BLOCK_SIZE * RAIL_TYPES / 2) + UI_BLOCK_SIZE * i;
            i32 y = GetScreenHeight() - UI_BLOCK_SIZE;
            uiState.buttons[i].bounds = { (float)x, (float)y, (float)UI_BLOCK_SIZE, (float)UI_BLOCK_SIZE };
            uiState.buttons[i].color = DARKGRAY;
            uiState.buttons[i].hovered = false;
            //state.buttons[i].clicked = false;
        }

        // Load the rail sprites - Horizontally packed
        Texture* texture = Textures::Load("res/sprites/tracks_directional.png");

        // VERTICAL
        Rectangle source = { 0, 0, CELL_SIZE, CELL_SIZE };
        Vector2 origin = { CELL_SIZE / 2, CELL_SIZE / 2 };
        uiState.railSpritesClockwise[VERTICAL] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // HORIZONTAL
        source = { CELL_SIZE, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[HORIZONTAL] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // TOP TO LEFT
        source = { CELL_SIZE * 2, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[TOP_TO_LEFT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // TOP TO RIGHT
        source = { CELL_SIZE * 3, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[TOP_TO_RIGHT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // BOTTOM TO RIGHT
        source = { CELL_SIZE * 4, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[BOTTOM_TO_RIGHT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // BOTTOM TO LEFT
        source = { CELL_SIZE * 5, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[BOTTOM_TO_LEFT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);

        // Counter clockwise
        // VERTICAL
        source = { 0, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[VERTICAL] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // HORIZONTAL
        source = { CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[HORIZONTAL] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // TOP TO LEFT
        source = { CELL_SIZE * 2, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[TOP_TO_LEFT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // TOP TO RIGHT
        source = { CELL_SIZE * 3, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[TOP_TO_RIGHT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // BOTTOM TO RIGHT
        source = { CELL_SIZE * 4, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[BOTTOM_TO_RIGHT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // BOTTOM TO LEFT
        source = { CELL_SIZE * 5, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[BOTTOM_TO_LEFT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);

        // Crossing
        texture = Textures::Load("res/sprites/crossing.png");
        source = { 0, 0, CELL_SIZE, CELL_SIZE };
        uiState.crossingSprite = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
    }

    void Update(i32 level)
    {
        Grid::Grid& grid = Game::state.grid;
        Vector2 mousePosition = Game::state.mouseWorldPosition;
        Vector2 mouseScreenPosition = GetMousePosition();
        Grid::Cell* cell = grid.GetCellAtWorldPosition(mousePosition);
        
        Vector2 cellCenter = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y + grid.cellSize / 2 };
        uiState.canBuild = false;
        // Current rail segment type
        RailType type = RailType::NONE;

        /*=========================================================
            RAIL MOUSE OVER GRID
        =========================================================*/

        // CALCULATE THE UI RAIL SEGMENT
        if (uiState.selectedType == RailType::VERTICAL)
        {
            type = RailType::VERTICAL;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y };
            uiState.selectedRail.end = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y + grid.cellSize };
        }
        if (uiState.selectedType == RailType::HORIZONTAL)
        {
            type = RailType::HORIZONTAL;
            uiState.selectedRail.start = { cell->worldPosition.x, cell->worldPosition.y + grid.cellSize / 2 };
            uiState.selectedRail.end = { cell->worldPosition.x + grid.cellSize, cell->worldPosition.y + grid.cellSize / 2 };
        }
        if (uiState.selectedType == RailType::TOP_TO_LEFT)
        {
            type = RailType::TOP_TO_LEFT;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y };
            uiState.selectedRail.end = { cell->worldPosition.x, cell->worldPosition.y + grid.cellSize / 2 };
        }
        if (uiState.selectedType == RailType::TOP_TO_RIGHT)
        {
            type = RailType::TOP_TO_RIGHT;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y };
            uiState.selectedRail.end = { cell->worldPosition.x + grid.cellSize, cell->worldPosition.y + grid.cellSize / 2 };
        }
        if (uiState.selectedType == RailType::BOTTOM_TO_RIGHT)
        {
            type = RailType::BOTTOM_TO_RIGHT;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y + grid.cellSize };
            uiState.selectedRail.end = { cell->worldPosition.x + grid.cellSize, cell->worldPosition.y + grid.cellSize / 2 };
        }
        if (uiState.selectedType == RailType::BOTTOM_TO_LEFT)
        {
            type = RailType::BOTTOM_TO_LEFT;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y + grid.cellSize };
            uiState.selectedRail.end = { cell->worldPosition.x, cell->worldPosition.y + grid.cellSize / 2 };
        }

        /*=========================================================
            CREATE RAIL SEGMENT
        =========================================================*/
        if (Game::GetLevel().railCount == 0 || Game::GetLevel().trainCount == 0)
        {
            uiState.canBuild = false;
        }
        else
        {
            uiState.canBuild = CanBuildRail(cell);
        }
        if (!Mines::IsRailCompatible(cell))
        {
            uiState.canBuild = false;
        }

        if (IsMouseButtonPressed(0) && uiState.canBuild)
        {
            CreateRailSegment(cell, type);
        }

        /*=========================================================
            SCROLL TO CHANGE RAIL SEGMENT
        =========================================================*/
        if (IsKeyPressed(KEY_R))
        {
            uiState.selectedType = (RailType)((uiState.selectedType + 1) % RAIL_TYPES);
        }
        /*if (GetMouseWheelMove() < 0)
        {
            uiState.selectedType = (RailType)((uiState.selectedType - 1) % RAIL_TYPES);
        }*/
        if (uiState.selectedType < 0) uiState.selectedType = (RailType)(RAIL_TYPES - 1);
        if (uiState.selectedType > RAIL_TYPES - 1) uiState.selectedType = (RailType)0;

        /*=========================================================
            DELETE RAIL SEGMENT
        =========================================================*/
        if (IsMouseButtonDown(1) && Game::GetLevel().trainCount > 0)
        {
            if (cell->railType != -1 && cell->railType != CROSSING)
            {
                cell->railType = -1;
                cell->connectionPoints.reset();
                cell->connectionPositions[0] = { -1,-1 };
                cell->connectionPositions[1] = { -1,-1 };
                // Remove the rail from the rail state
                railState.coordinateToRailMap.erase(cell->coordinate);
                Game::GetLevel().railCount++;
            }
        }

        /*========================================================
            FLIP RAIL SEGMENT DIRECTION
        =========================================================*/
        if (IsKeyPressed(KEY_F) && Game::Level().trainCount > 0)
        {
            if (cell->railType != -1)
            {
                cell->clockwise = !cell->clockwise;
            }
            uiState.clockwise = !uiState.clockwise;
        }
    }

    void Draw(i32 level)
    {
        // DRAW THE RAIL SEGMENT AT THE MOUSE POSITION
        Vector2 mouseWorldPosition = Game::state.mouseWorldPosition;
        Grid::Grid& grid = Game::state.grid;
        Grid::Cell* cell = grid.GetCellAtWorldPosition(mouseWorldPosition);

        // DRAW THE RAIL GHOST
        if (UI::state.buildType == UI::RAIL && cell->initialized && uiState.selectedType != NONE)
        {
            Color ghostColor = uiState.canBuild ? PALETTE_GREEN : PALETTE_BLACK;

            if (cell->hasMine || cell->hasStation)
            {
                if (!Mines::IsRailCompatible(cell))
                {
                    ghostColor = PALETTE_BLACK;
                }
                else
                {
                    ghostColor = PALETTE_GREEN;
                }
            }

            Rectangle bounds =
            {
                cell->worldPosition.x,
                cell->worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };
            DrawRectanglePro(bounds, { 0,0 }, 0, ghostColor);
            if (uiState.clockwise)
            {
                DrawTexturePro
                (
                    *uiState.railSpritesClockwise[uiState.selectedType].texture,
                    uiState.railSpritesClockwise[uiState.selectedType].source,
                    bounds,
                    { 0,0 },
                    0.0f,
                    WHITE
                );
            }
            else
            {
                DrawTexturePro
                (
                    *uiState.railSpritesCounterClockwise[uiState.selectedType].texture,
                    uiState.railSpritesCounterClockwise[uiState.selectedType].source,
                    bounds,
                    { 0,0 },
                    0.0f,
                    WHITE
                );
            }
        }

        // DRAW THE RAILS
        for (auto& [coordinate, rail] : railState.coordinateToRailMap)
        {
            Grid::Cell* cell = grid.CoordinateToCell(rail.coordinate);

            Rectangle destination =
            {
                cell->worldPosition.x,
                cell->worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };

            if (cell->hasCrossing)
            {
                DrawTexturePro
                (
                    *uiState.crossingSprite.texture,
                    uiState.crossingSprite.source,
                    destination,
                    { 0,0 },
                    0.0f,
                    uiState.crossingSprite.tint
                );
            }
            else if (cell->clockwise)
            {
                DrawTexturePro
                (
                    *uiState.railSpritesClockwise[rail.type].texture,
                    uiState.railSpritesClockwise[rail.type].source,
                    destination,
                    { 0,0 },
                    0.0f,
                    uiState.railSpritesClockwise[rail.type].tint
                );
            }
            else
            {
                DrawTexturePro
                (
                    *uiState.railSpritesCounterClockwise[rail.type].texture,
                    uiState.railSpritesCounterClockwise[rail.type].source,
                    destination,
                    { 0,0 },
                    0.0f,
                    uiState.railSpritesCounterClockwise[rail.type].tint
                );
            }

            // DEBUG
            //DrawText(std::to_string(rail.trainsOnRail.size()).c_str(), cell->worldPosition.x, cell->worldPosition.y, 10, WHITE);
        }

        // DEBUG

        std::string mouseCoordinate = std::to_string(cell->coordinate.x) + "," + std::to_string(cell->coordinate.y);
        DrawTextEx(GetFontDefault(), mouseCoordinate.c_str(), { mouseWorldPosition.x - 20, mouseWorldPosition.y - 20 }, 20, 0, ORANGE);

        if (cell->connectionPositions[0].x != -1)
        {
            DrawCircleV(cell->connectionPositions[0], 5, RED);
        }
        if (cell->connectionPositions[1].x != -1)
        {
            DrawCircleV(cell->connectionPositions[1], 5, RED);
        }
    }

    Vector2 GetNextDestinationPoint(Grid::Cell* cell, Vector2 worldEntryPoint)
    {
        RailType railPiece = (RailType)cell->railType;

        float halfCellSize = CELL_SIZE / 2.0f;

        Vector2 cellCenter = { cell->worldPosition.x + halfCellSize, cell->worldPosition.y + halfCellSize };
        Vector2 top   = { cellCenter.x, cellCenter.y - halfCellSize };
        Vector2 bot   = { cellCenter.x, cellCenter.y + halfCellSize };
        Vector2 left  = { cellCenter.x - halfCellSize, cellCenter.y };
        Vector2 right = { cellCenter.x + halfCellSize, cellCenter.y };

        if (cell->hasCrossing)
        {
            ConnectionPoint in = WorldToConnectionPoint(cell, worldEntryPoint);
            if (in == NORTH) return bot;
            if (in == SOUTH) return top;
            if (in == WEST)  return right;
            if (in == EAST)  return left;
        }

        // These coordinates are relative to the cell top left corner
        if (cell->clockwise)
        {
            // Clockwise means up and to the left....?
            if (railPiece == VERTICAL)        return top;
            if (railPiece == HORIZONTAL)      return left;
            if (railPiece == TOP_TO_LEFT)     return left;
            if (railPiece == TOP_TO_RIGHT)    return top;
            if (railPiece == BOTTOM_TO_RIGHT) return right;
            if (railPiece == BOTTOM_TO_LEFT)  return bot;
        }
        else
        {
            // Counter clockwise means down and to the right....?
            if (railPiece == VERTICAL)        return bot;
            if (railPiece == HORIZONTAL)      return right;
            if (railPiece == TOP_TO_LEFT)     return top;
            if (railPiece == TOP_TO_RIGHT)    return right;
            if (railPiece == BOTTOM_TO_RIGHT) return bot;
            if (railPiece == BOTTOM_TO_LEFT)  return left;
        }
        
        std::cout << "Error: GetNextDestinationPoint" << std::endl;
    }

    Grid::Cell* GetNextCell(Grid::Cell* cell, Vector2 worldPositionIn)
    {
        Grid::Grid& grid = Game::state.grid;
        RailType railPiece = (RailType)cell->railType;

        if (cell->hasCrossing)
        {
            ConnectionPoint in = WorldToConnectionPoint(cell, worldPositionIn);
            if (in == NORTH) return grid.CoordinateToCell({ cell->coordinate.x, cell->coordinate.y + 1 });
            if (in == WEST)  return grid.CoordinateToCell({ cell->coordinate.x + 1, cell->coordinate.y });
            if (in == SOUTH) return grid.CoordinateToCell({ cell->coordinate.x, cell->coordinate.y - 1 });
            if (in == EAST)  return grid.CoordinateToCell({ cell->coordinate.x - 1, cell->coordinate.y });
        }


        if (cell->clockwise)
        {
            if (railPiece == VERTICAL)        return grid.CoordinateToCell({cell->coordinate.x, cell->coordinate.y - 1});
            if (railPiece == HORIZONTAL)      return grid.CoordinateToCell({cell->coordinate.x - 1, cell->coordinate.y});
            if (railPiece == TOP_TO_LEFT)     return grid.CoordinateToCell({cell->coordinate.x - 1, cell->coordinate.y});
            if (railPiece == TOP_TO_RIGHT)    return grid.CoordinateToCell({cell->coordinate.x, cell->coordinate.y - 1});
            if (railPiece == BOTTOM_TO_RIGHT) return grid.CoordinateToCell({cell->coordinate.x + 1, cell->coordinate.y});
            if (railPiece == BOTTOM_TO_LEFT)  return grid.CoordinateToCell({ cell->coordinate.x, cell->coordinate.y + 1 });
        }
        else
        {
            if (railPiece == VERTICAL)        return grid.CoordinateToCell({cell->coordinate.x, cell->coordinate.y + 1});
            if (railPiece == HORIZONTAL)      return grid.CoordinateToCell({cell->coordinate.x + 1, cell->coordinate.y});
            if (railPiece == TOP_TO_LEFT)     return grid.CoordinateToCell({cell->coordinate.x, cell->coordinate.y - 1});
            if (railPiece == TOP_TO_RIGHT)    return grid.CoordinateToCell({cell->coordinate.x + 1, cell->coordinate.y});
            if (railPiece == BOTTOM_TO_RIGHT) return grid.CoordinateToCell({cell->coordinate.x, cell->coordinate.y + 1});
            if (railPiece == BOTTOM_TO_LEFT)  return grid.CoordinateToCell({ cell->coordinate.x - 1, cell->coordinate.y });
        }
        
        return nullptr;
    }
    void CreateRailSegment(Grid::Cell* cell, RailType type)
    {
        Rail rail{};
        rail.type = uiState.selectedType;
        rail.start = uiState.selectedRail.start;
        rail.end = uiState.selectedRail.end;
        rail.coordinate = cell->coordinate;
        cell->clockwise = uiState.clockwise;
        railState.coordinateToRailMap[cell->coordinate] = rail;
        SetConnectionPoints(cell, type);
        Game::GetLevel().railCount--;
    }
}