#include "Grid.h"

namespace Grid
{
    Grid Init(i32 level, i32 width, i32 height, i32 cellSize)
    {
        Grid grid;
        grid.level = level;
        grid.width = width;
        grid.height = height;
        grid.cellSize = cellSize;
        grid.worldPosition = { 0,0 };
        
        Texture2D* texture = Textures::Load("res/sprites/floor_tile_00.png");
        grid.groundSprite[0] = Textures::CreateSprite
        (texture, { 0,0,CELL_SIZE,CELL_SIZE }, { CELL_SIZE / 2, CELL_SIZE / 2 }, 0.0f, 1.0f,WHITE);
        texture = Textures::Load("res/sprites/floor_tile_01.png");
        grid.groundSprite[1] = Textures::CreateSprite
        (texture, { 0,0,CELL_SIZE,CELL_SIZE }, { CELL_SIZE / 2, CELL_SIZE / 2 }, 0.0f, 1.0f, WHITE);
        texture = Textures::Load("res/sprites/floor_tile_02.png");
        grid.groundSprite[2] = Textures::CreateSprite
        (texture, { 0,0,CELL_SIZE,CELL_SIZE }, { CELL_SIZE / 2, CELL_SIZE / 2 }, 0.0f, 1.0f, WHITE);
        texture = Textures::Load("res/sprites/floor_tile_03.png");
        grid.groundSprite[3] = Textures::CreateSprite
        (texture, { 0,0,CELL_SIZE,CELL_SIZE }, { CELL_SIZE / 2, CELL_SIZE / 2 }, 0.0f, 1.0f, WHITE);
        texture = Textures::Load("res/sprites/floor_tile_04.png");
        grid.groundSprite[4] = Textures::CreateSprite
        (texture, { 0,0,CELL_SIZE,CELL_SIZE }, { CELL_SIZE / 2, CELL_SIZE / 2 }, 0.0f, 1.0f, WHITE);

        texture = Textures::Load("res/sprites/obstacle.png");
        grid.obstacleSprite = Textures::CreateSprite
        (
            texture,
            { 0,0,CELL_SIZE,CELL_SIZE },
            { CELL_SIZE / 2, CELL_SIZE / 2 },
            0.0f,
            1.0f,
            WHITE
        );

        for (int y = 0; y < grid.height; y++)
        {
            for (int x = 0; x < grid.width; x++)
            {
                Vector2Int coordinate = { x,y };
                Cell cell =
                {
                    .level = level,
                    .initialized = 1,
                    .coordinate = coordinate,
                    .worldPosition = Vector2Add(GetWorldPosition(coordinate), grid.worldPosition),
                    .groundSpriteIndex = GetRandomValue(0, 4),
                };
                grid.cells[coordinate] = cell;
            }
        }
        grid.cells[{-1, -1}].initialized = false;
        return grid;
    }

    void AddToGrid(Grid& grid, Vector2Int topLeftCoordinate, i32 width, i32 height)
    {
        i32 xMax = topLeftCoordinate.x + width;
        i32 yMax = topLeftCoordinate.y + height;

        for (int y = topLeftCoordinate.y; y < yMax; y++)
        {
            for (int x = topLeftCoordinate.x; x < xMax; x++)
            {
                Vector2Int coordinate = { x, y };
                Cell cell =
                {
                    .level = grid.level,
                    .initialized = 1,
                    .coordinate = coordinate,
                    .worldPosition = Vector2Add(GetWorldPosition(coordinate), grid.worldPosition),
                    .groundSpriteIndex = GetRandomValue(0, 4),
                };
                grid.cells[coordinate] = cell;
            }
        }

        grid.width  = xMax;
        grid.height = yMax;
    }

    void Draw(Grid& grid, GameCamera::Camera& camera)
    {
        for (auto& [coordinate, cell] : grid.cells)
        {
            Rectangle rectangle =
            {
                (float)cell.worldPosition.x,
                (float)cell.worldPosition.y ,
                (float)CELL_SIZE,
                (float)CELL_SIZE
            };

            if (cell.hasObstacle)
            {
                DrawTexturePro
                (
                    *grid.obstacleSprite.texture,
                    { 0, 0, CELL_SIZE, CELL_SIZE },
                    rectangle,
                    { 0,0 },
                    0.0f,
                    WHITE
                );
            }
            else
            {
                DrawTexturePro
                (
                    *grid.groundSprite[cell.groundSpriteIndex].texture,
                    { 0, 0, CELL_SIZE, CELL_SIZE },
                    rectangle,
                    { 0,0 },
                    0.0f,
                    WHITE
                );
            }
        }
    }
}