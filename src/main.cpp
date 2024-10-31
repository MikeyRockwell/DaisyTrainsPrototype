#include "Engine.h"

#include "Game.h"
#include "Grid.h"
#include "Rail.h"
#include "Trains.h"
#include "Mines.h"

#include <iostream>

const i32 screenWidth = 2560;
const i32 screenHeight = 1440;

int main()
{
	std::cout << "Hello, World!" << std::endl;

	InitWindow(screenWidth, screenHeight, "Connex");

    SetTargetFPS(60);

	
	Game::Init();

    Rail::Init();
	Mines::Init();
	Trains::Init();

	while(!WindowShouldClose())
	{
        Game::Update(); // Update the camera, and the mouse position

		for (int i = 0; i < Game::LEVEL_COUNT; i++)
		{
			Rail::Update();
			Trains::Update(i);
		}
		Mines::Update();

		BeginDrawing();
        BeginMode2D(Game::state.camera.rlCamera);
		ClearBackground(PALETTE_DARK_GRAY);

        for (int i = 0; i < Game::LEVEL_COUNT; i++)
        {
            Game::Level& level = Game::GetLevel(i);
            if (level.unlocked == false) continue;
			level.grid.Draw(Game::state.camera);
			Trains::Draw(i);
        }
		Rail::Draw();
        Mines::Draw();

        EndMode2D();

		Game::Draw();

		EndDrawing();
	}

	Textures::Clean();
	CloseWindow();

	return 0;
}