
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
	UI::Init();

    Game::AddLevel(0);

	while(!WindowShouldClose())
	{
        Game::Update(); // Update the camera, and the mouse position
		Rail::Update(0);
		Trains::Update(0);
		Mines::Update(0);
		UI::Update();

		Game::CheckLevelStates();

		BeginDrawing();
        BeginMode2D(Game::state.camera.rlCamera);
		ClearBackground(PALETTE_DARK_GRAY);

		Grid::Draw(Game::state.grid, Game::state.camera);
		Rail::Draw(0);
		Trains::Draw(0);
		Mines::Draw(0);

		UI::DrawWorldSpace();

        EndMode2D();

		Game::Draw();
        UI::DrawScreenSpace();

		EndDrawing();
	}

	Textures::Clean();
	CloseWindow();

	return 0;
}