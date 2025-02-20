#include "Curves.h"
#include "Game.h"
#include "Mines.h"
#include "Rail.h"
#include "Trains.h"

#include <string>
#include <iostream>
#include <cassert>
#include "Audio.h"

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

        texture = Textures::Load("res/sprites/train_car_orange.png");
        width = texture->width;
        height = texture->height;
        resources.carSprites[ORANGE_CARGO] = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)width, (float)height },
            { (float)width / 2, (float)height / 2 },
            0.0f,
            1.0f,
            WHITE
        );
        texture = Textures::Load("res/sprites/train_car_yellow.png");
        resources.carSprites[YELLOW_CARGO] = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)width, (float)height },
            { (float)width / 2, (float)height / 2 },
            0.0f,
            1.0f,
            WHITE
        );
        texture = Textures::Load("res/sprites/train_car_purple.png");
        resources.carSprites[PURPLE_CARGO] = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)width, (float)height },
            { (float)width / 2, (float)height / 2 },
            0.0f,
            1.0f,
            WHITE
        );
        texture = Textures::Load("res/sprites/train_car_pink.png");
        resources.carSprites[PINK_CARGO] = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)width, (float)height },
            { (float)width / 2, (float)height / 2 },
            0.0f,
            1.0f,
            WHITE
        );
        texture = Textures::Load("res/sprites/train_car_empty.png");
        resources.carSprites[BLACK_CARGO] = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)width, (float)height },
            { (float)width / 2, (float)height / 2 },
            0.0f,
            1.0f,
            WHITE
        );

        texture = Textures::Load("res/sprites/remove_train.png");
        resources.removeTrain = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)texture->width, (float)texture->height },
            { (float)texture->width / 2, (float)texture->height / 2 },
            0.0f,
            1.0f,
            WHITE
        );
	}

	void Update()
	{
        Grid::Grid& grid = Game::state.grid;
        /*===============================================
            TRAIN AUDIO
        ===============================================*/
        for (int i = 0; i < Game::GetLevel().trainCount; i++)
        {
            TrainEngine& train = Game::GetLevel().trains[i];
            float distance = Vector2Distance(train.transform.worldPosition, Game::state.camera.target);
            distance *= 1.9;
            distance /= (Game::state.camera.rlCamera.zoom * 0.5);
            float volume = 1.0f - distance / 1000.0f;
            if (volume < 0.0f)
            {
                volume = 0.0f;
            }
            volume *= 3.5f;
            SetSoundVolume(Audio::resources.train_running.sound, volume);
            if (!IsSoundPlaying(Audio::resources.train_running.sound))
            {
                PlaySound(Audio::resources.train_running.sound);
            }
        }

        /*===============================================
            CREATE TRAIN
        ===============================================*/
        state.trainPlacementValid = false;

		if (UI::state.buildType == UI::BuildType::TRAIN && !UI::state.mouseOverUI)
		{
			Rail::uiState.selectedType = Rail::RailType::NONE;
            
            Vector2 mousePosition = Game::state.mouseWorldPosition;
            Grid::Cell* cell = grid.GetCellAtWorldPosition(mousePosition);

            if (cell->railType != -1)
            {
                state.trainPlacementPosition = Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, CELL_SIZE / 2.0f });

                bool tooClose = false;
                for (int i = 0; i < Game::GetLevel().trainCount; i++)
                {
                    if (Vector2Distance(state.trainPlacementPosition, Game::GetLevel().trains[i].transform.worldPosition) < MIN_TRAIN_SPAWN_DISTANCE)
                    {
                        tooClose = true;
                    }
                    else
                    {
                        state.trainPlacementValid = true;
                    }
                }

                if (IsMouseButtonPressed(0) && Game::GetLevel().trainsAvailable > 0 && !tooClose)
                {
                    Game::Level& level = Game::GetLevel();
                    level.trainDeployed = true;
                    TrainEngine& train = level.trains[level.trainCount++];
                    train.index = level.trainCount - 1;

                    train.transform.worldPosition = state.trainPlacementPosition;
                    train.transform.currentCell = cell;
                    train.acceleration = 50.0f;
                    train.transform.speed = 0.0f;
                    train.maxSpeed = 170.0f;

                    train.transform.entryPosition  = train.transform.worldPosition;
                    train.transform.targetPosition = Rail::GetNextDestinationPoint(cell, train.transform.worldPosition);
                    train.transform.distanceToNextCell = Vector2Distance(train.transform.worldPosition, train.transform.targetPosition);
                    if (cell->railType != Rail::RailType::VERTICAL && cell->railType != Rail::RailType::HORIZONTAL)
                    {
                        train.transform.distanceToNextCell *= 1.5f;
                    }
                    train.transform.distanceTravelled = 0.0f;
                    train.transform.nextCell = Rail::GetNextCell(cell, train.transform.worldPosition);

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
                        car.transform.speed = train.transform.speed;
                    }

                    Game::GetLevel().trainsAvailable--;

                    PlaySound(Audio::resources.place_train.sound);
                }
            }
		}

        /*==============================================
            REMOVE TRAIN
        ==============================================*/

        if (UI::state.buildType == UI::BuildType::REMOVE_TRAIN && !UI::state.mouseOverUI)
        {
            Vector2 mousePosition = Game::state.mouseWorldPosition;
            
            Game::Level& level = Game::GetLevel();
            for (int i = 0; i < level.trainCount; i++)
            {
                TrainEngine& train = level.trains[i];
                if (CheckCollisionPointCircle(mousePosition, train.transform.worldPosition, 50.0f))
                {
                    if (IsMouseButtonDown(0))
                    {
                        // Remove the train
                        RemoveTrain(train.index);
                        break;
                    }
                }
            }
        }


        for (int x = 0; x < Game::LEVEL_COUNT; x++)
        {
            Game::Level& level = Game::state.levels[x];
            for (int i = 0; i < level.trainCount; i++)
            {
                TrainEngine& train = level.trains[i];
                UpdateTransform(train, train.transform);

                for (int j = 0; j < train.carCount; j++)
                {
                    Car& car = train.cars[j];

                    TrainTransform& parentTransform = j == 0 ? train.transform : train.cars[j - 1].transform;
                    UpdateCarTransform(car, parentTransform, train.transform.speed);

                    // LOADING AND UNLOADING
                    Grid::Cell* cell = grid.GetCellAtWorldPosition(car.transform.worldPosition);
                    /*if (cell->hasMine && !car.loaded)
                    {
                        Mines::Mine* mine = &Mines::state.mines[cell->coordinate];
                        if (mine->stack->count > 0)
                        {
                            i32 amount = 1;
                            mine->stack->count -= amount;
                            car.loaded = true;
                            car.cargoType = mine->cargoType;
                        }
                    }*/

                    // Unloading at STATION
                    //if (cell->hasStation && car.loaded)
                    if (cell->hasStation)
                    {
                        Mines::Station* station = &Mines::state.stations[cell->coordinate];
                        if (!car.loaded && station->pickup && station->stack->count > 0)
                        {
                            i32 amount = 1;
                            station->stack->count -= amount;
                            car.loaded = true;
                            car.cargoType = station->cargoType;
                        }
                        else if (car.loaded && !station->pickup)
                        {
                            if (station->cargoType == car.cargoType && station->stack->count != station->stack->capacity)
                            {
                                station->stack->count++;
                                car.loaded = false;
                            }
                        }
                    }
                }
            }
        }
	}

    void UpdateTransform(TrainEngine& engine, TrainTransform& transform)
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

    void UpdateCarTransform(Car& car, TrainTransform& parentTransform, float speed)
    {
        Grid::Grid& grid = Game::state.grid;
        TrainTransform& transform = car.transform;
        if (transform.currentCell == parentTransform.currentCell &&
            abs(transform.tValue - parentTransform.tValue) < 0.15f)
        {
            // Just offset the t value
            car.transform.tValue = parentTransform.tValue - 0.75f;
            transform.distanceTravelled = transform.distanceToNextCell * car.transform.tValue;
        }
        else
        { 
            // Update the distance travelled
            transform.distanceTravelled += (speed * Game::state.clock.deltaTime);
            transform.tValue = transform.distanceTravelled / transform.distanceToNextCell;
        }

        if (Vector2Distance(transform.worldPosition, parentTransform.worldPosition) > 50.0f)
        {
            transform.currentCell = parentTransform.currentCell;
            transform.entryPosition = parentTransform.entryPosition;
            transform.targetPosition = parentTransform.targetPosition;
            transform.distanceToNextCell = parentTransform.distanceToNextCell;
            transform.distanceTravelled = parentTransform.distanceTravelled;
            transform.nextCell = parentTransform.nextCell;
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
        transform.targetPosition = Rail::GetNextDestinationPoint(transform.nextCell, transform.worldPosition);
        transform.distanceToNextCell = Vector2Distance(transform.worldPosition, transform.targetPosition);
        transform.distanceTravelled = 0.0f;
        transform.nextCell = Rail::GetNextCell(transform.nextCell, transform.worldPosition);
    }

    void RemoveTrain(i32 index)
    {
        Game::Level& level = Game::GetLevel();
        level.trains[index] = level.trains[level.trainCount - 1];
        level.trainCount--;
        level.trains[index].index = index;
        if (level.trainCount == 0)
        {
            level.trainDeployed = false;
        }
        level.trainsAvailable++;

        PlaySound(Audio::resources.remove_train.sound);
    }

	void Draw()
    {
        Grid::Grid& grid  = Game::state.grid;

		if (UI::state.buildType == UI::BuildType::TRAIN && Game::GetLevel().trainCount >= 1 && Game::GetLevel().trainsAvailable > 0)
		{
			Vector2 mousePosition = Game::state.mouseWorldPosition;
            float size = MIN_TRAIN_SPAWN_DISTANCE;
            Rectangle destination = { mousePosition.x - size / 2, mousePosition.y - size / 2, size , size };
            if (state.trainPlacementValid)
            {
                DrawRectangleRoundedLines(destination, 1.0f, 4, 4.0f / Game::state.camera.rlCamera.zoom, PALETTE_YELLOW);
            }
            else
            {
                DrawRectangleRoundedLines(destination, 1.0f, 4, 4.0f / Game::state.camera.rlCamera.zoom, PALETTE_ORANGE);
            }

		}
        for (int x = 0; x < Game::LEVEL_COUNT; x++)
        {
            Game::Level& level = Game::state.levels[x];
            for (int i = 0; i < level.trainCount; i++)
            {
                TrainEngine& train = level.trains[i];
            
                Color color = WHITE;
                Rectangle destination =
                {
                    train.transform.worldPosition.x,
                    train.transform.worldPosition.y,
                    (float)resources.trainSprite.texture->width,
                    (float)resources.trainSprite.texture->height
                };
                if (train.transform.currentCell->hasCrossing &&
                    abs(train.transform.entryPosition.x - train.transform.targetPosition.x) > 0)
                {
                    // THE TRAIN IS MOVING HORIZONTALLY THROUGH A TUNNEL
                    // dont draw the train
                }
                else
                {
                    DrawTexturePro
                    (
                        *resources.trainSprite.texture,
                        resources.trainSprite.source,
                        destination,
                        { resources.trainSprite.origin.x, resources.trainSprite.origin.y },
                        train.transform.rotation,
                        color
                    );
                }

                for (int j = 0; j < train.carCount; j++)
                {
                    Car& car = train.cars[j];
                    if (car.transform.currentCell->hasCrossing &&
                        abs(car.transform.entryPosition.x - car.transform.targetPosition.x) > 0)
                    {
                        // THE CAR IS MOVING HORIZONTALLY THROUGH A TUNNEL
                        // dont draw the CAR
                    }
                    else
                    {
                        Textures::Sprite sprite = resources.carSprites[BLACK_CARGO];
                        if (car.loaded)
                        {
                            sprite = resources.carSprites[car.cargoType];
                        }
                        destination =
                        {
                            car.transform.worldPosition.x,
                            car.transform.worldPosition.y,
                            (float)sprite.texture->width,
                            (float)sprite.texture->height
                        };
                        DrawTexturePro
                        (
                            *sprite.texture,
                            sprite.source,
                            destination,
                            { sprite.origin.x, sprite.origin.y},
                            car.transform.rotation,
                            color
                        );
                    }
                }
            }
        }
	}
}
