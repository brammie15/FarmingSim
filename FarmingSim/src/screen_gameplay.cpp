#include "raylib.h"
#include "screens.h"
#include "raymath.h"
#include "sprites.h"
#include <string>
#include <vector>

//Should really make these more global ish
static const int screenWidth = 800;
static const int screenHeight = 500;


const char* stateName;
static int framesCounter = 0;
static int finishScreen = 0;
int randomInt = 0;
Texture2D tex;

bool shouldDebug = false;


std::vector<Rectangle> Obstacles;
float gravity = 500.0f;


Player player;
Color playerColor = ORANGE;

void getState() { //function to get the player state as a string
	switch (player.State)
	{
	case PlayerState::Idle:
		stateName = "Idle";
		playerColor = BLUE;
		break;

	case PlayerState::Run:
		stateName = "Running";
		playerColor = RED;
		break;
	case PlayerState::JumpStart:
		stateName = "Jump";
		playerColor = BLACK;
		break;
	case PlayerState::JumpUp:
		stateName = "Flying Up";
		playerColor = GREEN;
		break;
	case PlayerState::FallDown:
		playerColor = PURPLE;
		stateName = "Falling";
		break;
	}

}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
	// TODO: Initialize GAMEPLAY screen variables here!
	framesCounter = 0;
	finishScreen = 0;
	tex = LoadTexture("resources/mecha.png");
	float gravity = 500.0f;
	player.Position = { 0, 0 };
	player.type = WALL;

	Obstacles.push_back(Rectangle{ 100, screenHeight - 100, screenWidth - 200, 20 });
	Obstacles.push_back(Rectangle{ screenWidth / 2, screenHeight / 2 + 50, 100 , 100 });
	Obstacles.push_back(Rectangle{ screenWidth / 2, screenHeight / 2 - 100, 100 , 100 });


}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{


	int runDir = 0;
	if (IsKeyDown(KEY_D))
		runDir += 1;
	if (IsKeyDown(KEY_A))
		runDir += -1;
	bool wantJump = IsKeyDown(KEY_SPACE);

	switch (player.State)
	{
	case PlayerState::Idle:
		if (wantJump)
		{
			player.State = PlayerState::JumpStart;
			player.Velocity.y = player.jumpAcceleration;
			player.Velocity.x = 0;
		}
		else if (runDir != 0)
		{
			player.State = PlayerState::Run;
		}
		else
		{
			player.Velocity.x = 0;
			player.Velocity.y = 0;
		}
		break;

	case PlayerState::Run:
		if (wantJump)
		{
			player.State = PlayerState::JumpStart;
			player.Velocity.y = player.jumpAcceleration;
			player.Velocity.x *= player.jumpVelocityDampen;
		}
		else if (runDir == 0)
		{
			player.State = PlayerState::Idle;
		}
		else
		{
			player.Velocity.x = runDir * player.RunSpeed;
			player.Velocity.y = 0;
		}

		break;

	case PlayerState::JumpStart:
		if (player.Velocity.y >= 0)
			player.State = PlayerState::FallDown;
		else if (true)
			player.State = PlayerState::JumpUp;
		break;

	case PlayerState::JumpUp:
		if (player.Velocity.y >= 0)
			player.State = PlayerState::FallDown;
		player.Velocity.x = runDir * player.RunSpeed;
		break;

	case PlayerState::FallDown:
		player.Velocity.x = runDir * player.RunSpeed;
		break;
	}
	if (IsKeyDown(KEY_G)) {
		shouldDebug = true;
	}
	//Add gravity
	player.Velocity.y += gravity * GetFrameTime();

	//calculate the position the player WILL be at
	Vector2 newPos = Vector2Add(player.Position, Vector2Scale(player.Velocity, GetFrameTime()));

	//Bottom of screen check
	if (newPos.y > screenHeight - player.Size.y)
	{
		newPos.y = screenHeight - player.Size.y;
		player.Velocity.y = 0;
		if (player.State == PlayerState::FallDown)
			player.State = PlayerState::Idle;
	}
	playerColor = ORANGE;
	for (const Rectangle& obstacle : Obstacles)
	{
		if (CheckCollisionRecs(Rectangle{ newPos.x, newPos.y, player.Size.x, player.Size.y }, obstacle))
		{
			float playerX = newPos.x;
			float playerY = newPos.y;
			float playerH = player.Size.y;
			float playerW = player.Size.x;
			Vector2 playerTL = { playerX, playerY };
			Vector2 playerTR = { playerX + playerW, playerY };
			Vector2 playerBL = { playerX, playerY + playerH };
			Vector2 playerBR = { playerX + playerW, playerY + playerH };
			if (shouldDebug) {
				shouldDebug = true;
			}

			randomInt = newPos.x + player.Size.x > obstacle.x;


			/*if (player.Position.x < newPos.x && !(newPos.y >= obstacle.y + obstacle.height) && !(newPos.y + playerH <= obstacle.y) && !(newPos.x + playerW < obstacle.x)) {
				player.Velocity.x = 0;
				newPos.x = obstacle.x - playerW;
			}*/
			//Should handle Left Side Wall Collisions
			if (player.Position.x < newPos.x && newPos.x + playerW > obstacle.x && !(newPos.y + playerH <= obstacle.y) && newPos.y < obstacle.y + obstacle.height ) {
				printf("Left Side wall hit\n");
				player.Velocity.x = 0;
				newPos.x = obstacle.x - playerW;
			}
			

			//Handle top surface collisions (works)
			if (player.Position.y < newPos.y && newPos.y + playerH > obstacle.y && newPos.x + playerW > obstacle.x && newPos.x < obstacle.x + obstacle.width) // 
			{
				printf("Top wall hit\n");
				// we are falling into the block, stop if we were falling
				if (player.State == PlayerState::FallDown)
				{
					player.Velocity.x = 0;
					player.Velocity.y = 0;
					newPos.y = obstacle.y - playerH;
					player.State = PlayerState::Idle;
				}
				else
				{
					// we just know we can't go any further down
					player.Velocity.y = 0;
					newPos.y = obstacle.y - playerH;
				}
			}
			

			////Handle Bottom surface collisions (works)
			if (player.Position.y > newPos.y && newPos.x + playerW > obstacle.x) {
				printf("Bottom wall hit\n");
				if (player.State == PlayerState::JumpUp) {
					player.Velocity.y = 0;
					newPos.y = obstacle.y + obstacle.height;
					player.State = PlayerState::Idle;

				}
				else {
					player.Velocity.y = 0;
					newPos.y = obstacle.y + obstacle.height;
				}
			}






		}
	}






	if (player.State != PlayerState::FallDown && round(player.Velocity.y) > 0)
	{
		player.State = PlayerState::FallDown;
	}
	getState();
	player.Position = newPos;


	// if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
	// {
	//     finishScreen = 1;
	//     PlaySound(fxCoin);
	// }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
	for (Rectangle rect : Obstacles)
	{
		DrawRectangleRec(rect, GRAY);
		DrawRectangle(Obstacles[0].x, Obstacles[0].y, Obstacles[0].width, Obstacles[0].height, GRAY);
		//DrawTextureNPatch(ObstacleTexture, ObstacleNpatch, rect, Vector2Zero(), 0, WHITE);
	}
	DrawPlayer(player, playerColor);
	DrawText(TextFormat("%f", player.Position.y), 10, 30, 10, BLACK);
	DrawText(TextFormat("state:%s", stateName), 10, 50, 20, BLACK);
	DrawText(TextFormat("%i", randomInt), 10, 70, 20, BLACK);


	//DrawTriangle(Vector2{ player.Position.x , player.Position.y }, Vector2{ player.Position.x + (player.Size.x) / 4 , player.Position.y - 20 }, Vector2{ player.Position.x + player.Size.x / 2 , player.Position.y }, ORANGE);
	//DrawTriangle(Vector2{ player.Position.x + player.Size.x / 2 , player.Position.y }, Vector2{ player.Position.x + (player.Size.x) / 4 + player.Size.x / 2, player.Position.y - 20 }, Vector2{ player.Position.x + player.Size.x / 2 + player.Size.x / 2, player.Position.y }, ORANGE);
	DrawCircle(player.Position.x, player.Position.y,10,ORANGE);
	DrawCircle(0, 0, 10, PINK);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
	// TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
	return finishScreen;
}