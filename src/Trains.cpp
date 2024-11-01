#include "Curves.h"
#include "Game.h"
#include "Mines.h"
#include "Rail.h"
#include "Trains.h"

#include <string>
#include <iostream>
#include <cassert>

namespace Trains
{
    TrainState state;
    TrainResources resources;

	void Init()
	{
        Texture* texture = Textures::Load("res/sprites/train_engine.png");
        i32 width = texture->width;
        i32 height = texture->height;
        resources.trainSprite = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)width, (float)height },
            { (float)width / 2, (float)height / 2 },
            0.0f,
            1.0f,
            WHITE
        );

        texture = Textures::Load("res/sprites/train_car.png");
        width = texture->width;
        height = texture->height;
        resources.carSprite = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)width, (float)height },
            { (float)width / 2, (float)height / 2 },
            0.0f,
            1.0f,
            WHITE
        );
	}

	void Update(i32 level)
	{
        Grid::Grid& grid = Game::state.grid;

        /*===============================================
            CREATE TRAIN
        ===============================================*/
		if (UI::state.buildType == UI::BuildType::TRAIN)
		{
			Rail::uiState.selectedType = Rail::RailType::NONE;
            
            Vector2 mousePosition = Game::state.mouseWorldPosition;
            Grid::Cell* cell = grid.GetCellAtWorldPosition(mousePosition);

            if (cell->railType != -1)
            {
                state.trainPlacementPosition = Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, CELL_SIZE / 2.0f });

                if (IsMouseButtonPressed(0) && state.trainsAvailable > 0)
                {
                    TrainEngine& train = state.trains[state.trainCount++];

                    train.transform.worldPosition = state.trainPlacementPosition;
                    train.transform.currentCell = cell;
                    train.acceleration = 50.0f;
                    train.maxSpeed = 170.0f;

                    train.transform.entryPosition  = train.transform.worldPosition;
                    train.transform.targetPosition = Rail::GetNextDestinationPoint(cell);
                    train.transform.distanceToNextCell = Vector2Distance(train.transform.worldPosition, train.transform.targetPosition);
                    if (cell->railType != Rail::RailType::VERTICAL && cell->railType != Rail::RailType::HORIZONTAL)
                    {
                        train.transform.distanceToNextCell *= 1.5f;
                    }
                    train.transform.distanceTravelled = 0.0f;
                    train.transform.nextCell = Rail::GetNextCell(cell);

                    train.carCount = 8;
                    for (int i = 0; i < train.carCount; i++)
                    {
                        Car& car = train.cars[i];
                        car.transform.worldPosition = train.transform.worldPosition;
                        car.transform.currentCell = cell;
                        car.transform.entryPosition = car.transform.worldPosition;
                        car.transform.targetPosition = train.transform.targetPosition;
                        car.transform.distanceToNextCell = train.transform.distanceToNextCell;
                        car.transform.distanceTravelled = 0.0f;
                        car.transform.nextCell = train.transform.nextCell;
                    }
                }
            }
		}

        for (int i = 0; i < state.trainCount; i++)
        {
            TrainEngine& train = state.trains[i];
            UpdateTransform(level, train, train.transform);

            for (int j = 0; j < train.carCount; j++)
            {
                Car& car = train.cars[j];

                TrainTransform& parentTransform = j == 0 ? train.transform : train.cars[j - 1].transform;
                UpdateCarTransform(level, car, parentTransform, train.transform.speed);

                // LOADING AND UNLOADING
                Grid::Cell* cell = grid.GetCellAtWorldPosition(car.transform.worldPosition);
                if (cell->hasMine && !car.loaded)
                {
                    Mines::Mine* mine = &Mines::state.mines[cell->coordinate];
                    if (mine->stack->count > 0)
                    {
                        i32 amount = 1;
                        mine->stack->count -= amount;
                        car.loaded = true;
                        car.cargoType = mine->cargoType;
                    }
                }

                if (cell->hasStation && car.loaded)
                {
                    // Unloading at STATION
                    Mines::Station* station = &Mines::state.stations[cell->coordinate];
                    if (station->cargoType == car.cargoType)
                    {
                        station->stack->count++;
                        car.loaded = false;
                    }
                }
            }
        }
	}

    void UpdateTransform(i32 level, TrainEngine& engine, TrainTransform& transform)
    {
        Grid::Grid& grid = Game::state.grid;

        // Update the distance travelled
        transform.distanceTravelled += transform.speed * Game::state.clock.deltaTime;
        transform.tValue = transform.distanceTravelled / transform.distanceToNextCell;
        if (transform.tValue > 1.0f)
        {
            transform.tValue = 1.0f;
        }
        Vector2 nextPosition = Vector2Lerp(transform.entryPosition, transform.targetPosition, transform.tValue);

        // CURVED TRACK HANDLING
        if (transform.currentCell->railType != Rail::RailType::VERTICAL && 
            transform.currentCell->railType != Rail::RailType::HORIZONTAL)
        {
            // Calculate the center of the current cell
            Vector2 cellCenter = {
                transform.currentCell->worldPosition.x + grid.cellSize / 2.0f,
                transform.currentCell->worldPosition.y + grid.cellSize / 2.0f
            };

            // Offset the midpoint to create a control point for the curve
            Vector2 direction = Vector2Subtract(transform.targetPosition, transform.entryPosition);
            Vector2 perpendicular = { -direction.y, direction.x }; // Perpendicular vector
            perpendicular = Vector2Normalize(perpendicular);

            float offset = 1.0f; // Adjust this value to control the curvature
            Vector2 controlPoint = Vector2Add(cellCenter, Vector2Scale(perpendicular, offset));
            nextPosition = GetSplinePointBezierQuad(transform.entryPosition, controlPoint, transform.targetPosition, transform.tValue);
        }
        
        Vector2 facingDirection = Vector2Subtract(nextPosition, transform.worldPosition);
        facingDirection = Vector2Scale(Vector2Normalize(facingDirection), 100.0f);

        transform.worldPosition = nextPosition;
        transform.rotation = atan2(facingDirection.y, facingDirection.x) * RAD2DEG;
        transform.direction = facingDirection;
        transform.speed += engine.acceleration * Game::state.clock.deltaTime;
        transform.tValue = Clamp(transform.tValue, 0.0f, 1.0f);

        if (transform.speed > engine.maxSpeed) transform.speed = engine.maxSpeed;

        if (transform.tValue == 1.0f) // We have reached the next cell
        {
            if (transform.nextCell->railType != -1) // Does this cell have rail
            {
                EnterCell(transform);
            }
            else
            {
                // We have reached the final destination
                transform.speed = 0.0f;
            }
        }
    }

    void UpdateCarTransform(i32 level, Car& car, TrainTransform& parentTransform, float speed)
    {
        Grid::Grid& grid = Game::state.grid;
        TrainTransform& transform = car.transform;
        if (transform.currentCell == parentTransform.currentCell &&
            abs(transform.tValue - parentTransform.tValue) < 0.15f)
        {
            // Just offset the t value
            car.transform.tValue = parentTransform.tValue - 0.53f;
            transform.distanceTravelled = transform.distanceToNextCell * car.transform.tValue;
        }
        else
        { 
            // Update the distance travelled
            transform.distanceTravelled += speed * Game::state.clock.deltaTime;
            transform.tValue = transform.distanceTravelled / transform.distanceToNextCell;
        }
        transform.tValue = Clamp(transform.tValue, 0.0f, 1.0f);

        Vector2 nextPosition = Vector2Lerp(transform.entryPosition, transform.targetPosition, transform.tValue);

        // CURVED TRACK HANDLING
        if (transform.currentCell->railType != Rail::RailType::VERTICAL &&
            transform.currentCell->railType != Rail::RailType::HORIZONTAL)
        {
            // Calculate the center of the current cell
            Vector2 cellCenter = {
                transform.currentCell->worldPosition.x + grid.cellSize / 2.0f,
                transform.currentCell->worldPosition.y + grid.cellSize / 2.0f
            };

            // Offset the midpoint to create a control point for the curve
            Vector2 direction = Vector2Subtract(transform.targetPosition, transform.entryPosition);
            Vector2 perpendicular = { -direction.y, direction.x }; // Perpendicular vector
            perpendicular = Vector2Normalize(perpendicular);

            float offset = 1.0f; // Adjust this value to control the curvature
            Vector2 controlPoint = Vector2Add(cellCenter, Vector2Scale(perpendicular, offset));
            nextPosition = GetSplinePointBezierQuad(transform.entryPosition, controlPoint, transform.targetPosition, transform.tValue);
        }
        Vector2 facingDirection = Vector2Subtract(nextPosition, transform.worldPosition);
        facingDirection = Vector2Scale(Vector2Normalize(facingDirection), 100.0f);
        
        transform.worldPosition = nextPosition;
        transform.rotation = atan2(facingDirection.y, facingDirection.x) * RAD2DEG;
        transform.direction = facingDirection;
        transform.speed += speed;
        transform.tValue = Clamp(transform.tValue, 0.0f, 1.0f);

        if (transform.tValue == 1.0f) // We have reached the next cell
        {
            if (transform.nextCell->railType != -1) // Does this cell have rail
            {
                EnterCell(transform);
            }
            else
            {
                // We have reached the final destination
                transform.speed = 0.0f;
            }
        }
    }

    void EnterCell(TrainTransform& transform)
    {
        transform.currentCell = transform.nextCell;
        transform.worldPosition = transform.targetPosition;
        transform.entryPosition = transform.worldPosition;
        transform.targetPosition = Rail::GetNextDestinationPoint(transform.nextCell);
        transform.distanceToNextCell = Vector2Distance(transform.worldPosition, transform.targetPosition);
        transform.distanceTravelled = 0.0f;
        transform.nextCell = Rail::GetNextCell(transform.nextCell);
    }

	void Draw(i32 level)
    {
        Grid::Grid& grid  = Game::state.grid;

		if (UI::state.buildType == UI::BuildType::RAIL)
		{
			Vector2 mousePosition = GetMousePosition();
			Grid::Cell* cell = grid.GetCellAtWorldPosition(mousePosition);

			if (cell->railType != -1)
			{
                DrawCircleV(state.trainPlacementPosition, 8, YELLOW);
			}
		}
        for (int i = 0; i < state.trainCount; i++)
        {
            TrainEngine& train = state.trains[i];
            Color color = WHITE;
            Rectangle destination =
            {
                train.transform.worldPosition.x,
                train.transform.worldPosition.y,
                (float)resources.trainSprite.texture->width,
                (float)resources.trainSprite.texture->height
            };
            DrawTexturePro
            (
                *resources.trainSprite.texture,
                resources.trainSprite.source,
                destination,
                { resources.trainSprite.origin.x, resources.trainSprite.origin.y },
                train.transform.rotation,
                color
            );

            for (int j = 0; j < train.carCount; j++)
            {
                Car& car = train.cars[j];
                Color color = car.loaded ? PALETTE[car.cargoType] : PALETTE_LIGHT_GRAY;
                destination =
                {
                    car.transform.worldPosition.x,
                    car.transform.worldPosition.y,
                    (float)resources.carSprite.texture->width,
                    (float)resources.carSprite.texture->height
                };
                DrawTexturePro
                (
                    *resources.carSprite.texture,
                    resources.carSprite.source,
                    destination,
                    { resources.carSprite.origin.x, resources.carSprite.origin.y },
                    car.transform.rotation,
                    color
                );
            }


            // DEBUG
            // 
            //DrawPoly(train.worldPosition, 3, 12, train.rotation, color);
            //DrawCircleV(train.targetPosition, 5, GREEN);
            //DrawCircleV(train.entryPosition, 5, YELLOW);
            //DrawCircleV(train.controlPoint, 5, MAGENTA);
            // Current cell debug
            //Grid::Cell* cell = &grid.cells[train.currentCell->coordinate];
            //Rectangle rectangle =
            //{
            //    (float)cell->worldPosition.x,
            //    (float)cell->worldPosition.y ,
            //    (float)grid.cellSize,
            //    (float)grid.cellSize
            //};
            //DrawRectangleRec(rectangle, Fade(YELLOW, 0.5f));

            //// Next cell debug
            //if (train.nextCell)
            //{
            //    cell = &grid.cells[train.nextCell->coordinate];
            //    rectangle =
            //    {
            //        (float)cell->worldPosition.x,
            //        (float)cell->worldPosition.y ,
            //        (float)grid.cellSize,
            //        (float)grid.cellSize
            //    };
            //    DrawRectangleRec(rectangle, Fade(GREEN, 0.5f));
            //}

        }

	}
}
