#include <iostream>
#include <string>
#include <algorithm>
#include "olcConsoleGameEngine.h"


class Snake : public olcConsoleGameEngine
{
public:

	void DrawRect(int x, int y, int nxSize, int nySize, short shade, short colour)
	{
		DrawLine(x, y, x + nxSize, y, shade, colour);					// Top
		DrawLine(x, y + nySize, x + nxSize, y + nySize, shade, colour);	// Bottom
		DrawLine(x + nxSize, y, x + nxSize, y + nySize, shade, colour);	// Right
		DrawLine(x, y, x, y + nySize, shade, colour);					// Left
	}

	Snake()
	{
		m_sAppName = L"Snake Pro!";
	}

	enum STATES {
		MAINMENU_STATE,
		BEGIN_STATE,
		RUN_STATE,
		PAUSE_STATE,
		DEAD_STATE,
		DRAW_STATE,
	};
	STATES STATE = MAINMENU_STATE;

	struct coordinate
	{
		int x;
		int y;
	};

	struct GameMenu
	{
		int width;
		int height;
		int size;
		std::wstring sprite;
		GameMenu() : width(0), height(0), size(0), sprite(L"") {}
		GameMenu(std::wstring _sprite, int _width = 0, int _height = 0, int _size = 0) : sprite(_sprite), width(_width), height(_height), size(_size) {}
		GameMenu operator + (const GameMenu& rhs)
		{
			return GameMenu(this->sprite + rhs.sprite);
		}
		GameMenu& operator += (const std::wstring& rhs)
		{
			this->sprite += rhs;
			this->width = rhs.size();
			this->size += rhs.size();
			this->height += 1;
			return *this;
		}
	};
	std::list<coordinate> snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15} };
	coordinate nFood = { 30,15 };
	int nScore;
	int nSnakeDirection;
	int nRequestedDirection;
	bool bDead;
	float fTimePassed;
	bool bNotDone = true;

	GameMenu gmGameOver;
	GameMenu gmPause;
	GameMenu gmMainMenu;

	bool moveFood()
	{
		// Move Food to a random location that is not under the snake. Once done return true.
		// To Use call while(moveFood());

		nFood.x = (rand() % (ScreenWidth() - 2)) + 1; // make sure its not on the border
		nFood.y = (rand() % (ScreenHeight() - 2)) + 1;
		for (auto s : snake)
			if (nFood.x == s.x && nFood.y == s.y)
			{
				return true;
			}
		return false;
	}
	bool DrawMenu(GameMenu gmMenu)
	{
		short color = FG_RED;
		int x = 0;
		int y = 0;
		int locationX = (ScreenWidth() / 2) - (gmMenu.width / 2);
		int locationY = (ScreenHeight() / 2) - (gmMenu.height / 2) - 10;
		DrawRect(locationX - 2, locationY - 2, gmMenu.width + 3, gmMenu.height + 3, PIXEL_SOLID, FG_MAGENTA);
		for (size_t i = 0; i < gmMenu.size; i++)
		{
			if (gmMenu.width == x)
			{
				y++;
				x = 0;
			}
			if (gmMenu.sprite[i] == L'█' || gmMenu.sprite[i] == L'▄' || gmMenu.sprite[i] == L'▀') color = FG_WHITE;
			else if (gmMenu.sprite[gmMenu.width * y] == L'Y') color = FG_YELLOW;
			else if (gmMenu.sprite[gmMenu.width * y] == L'B') color = FG_BLUE;
			else color = FG_RED;
			wchar_t character = gmMenu.sprite[i];
			if (character == 'Y' || character == 'B') character = ' ';
			Draw(locationX + x, locationY + y, character, color);
			x++;
		}
		return true;
	}
private:
	bool OnUserCreate()
	{

		gmPause += L"┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼.";
		gmPause += L"┼██▀▀▀▀██▄┼███▀▀▀███┼██┼┼┼┼┼██┼██▀▀▀▀███┼██▀▀▀▀▀┼██▀▀▀▀██▄┼.";
		gmPause += L"┼██┼┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼┼██┼┼┼┼┼┼██┼┼┼┼┼██┼.";
		gmPause += L"┼██▄▄▄▄▄▀▀┼██▄▄▄▄▄██┼██┼┼┼┼┼██┼██▄▄▄▄▄▄▄┼██▀▀▀▀▀┼██┼┼┼┼┼██┼.";
		gmPause += L"┼██┼┼┼┼┼┼┼┼██┼┼┼┼┼██┼██┼┼┼┼┼██┼┼┼┼┼┼┼┼██┼██┼┼┼┼┼┼██┼┼┼┼┼██┼.";
		gmPause += L"┼██┼┼┼┼┼┼┼┼██┼┼┼┼┼██┼███▄▄▄███┼███▄▄▄▄██┼██▄▄▄▄▄┼██▄▄▄▄██▀┼.";
		gmPause += L"┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼.";

		gmGameOver += L"  ▄████  ▄▄▄       ███▄ ▄███▓▓█████ ";
		gmGameOver += L" ██▒ ▀█▒▒████▄    ▓██▒▀█▀ ██▒▓█   ▀ ";
		gmGameOver += L"▒██░▄▄▄░▒██  ▀█▄  ▓██    ▓██░▒███   ";
		gmGameOver += L"░▓█  ██▓░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄ ";
		gmGameOver += L"░▒▓███▀▒ ▓█   ▓██▒▒██▒   ░██▒░▒████▒";
		gmGameOver += L" ░▒   ▒  ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░";
		gmGameOver += L"  ░   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░";
		gmGameOver += L"░ ░   ░   ░   ▒   ░      ░      ░   ";
		gmGameOver += L"      ░       ░  ░       ░      ░  ░";
		gmGameOver += L"                                    ";
		gmGameOver += L" ▒█████   ██▒   █▓▓█████  ██▀███    ";
		gmGameOver += L"▒██▒  ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒  ";
		gmGameOver += L"▒██░  ██▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒  ";
		gmGameOver += L"▒██   ██░  ▒██ █░░▒▓█  ▄ ▒██▀▀█▄    ";
		gmGameOver += L"░ ████▓▒░   ▒▀█░  ░▒████▒░██▓ ▒██▒  ";
		gmGameOver += L"░ ▒░▒░▒░    ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░  ";
		gmGameOver += L"  ░ ▒ ▒░    ░ ░░   ░ ░  ░  ░▒ ░ ▒░  ";
		gmGameOver += L"░ ░ ░ ▒       ░░     ░     ░░   ░   ";
		gmGameOver += L"    ░ ░        ░     ░  ░   ░       ";
		gmGameOver += L"              ░                     ";
		gmGameOver += L"                                    ";
		gmGameOver += L"        ██               ██         ";
		gmGameOver += L"      ████▄   ▄▄▄▄▄▄▄   ▄████       ";
		gmGameOver += L"         ▀▀█▄█████████▄█▀▀          ";
		gmGameOver += L"           █████████████            ";
		gmGameOver += L"           ██▀▀▀███▀▀▀██            ";
		gmGameOver += L"           ██   ███   ██            ";
		gmGameOver += L"           ██   ███   ██            ";
		gmGameOver += L"           █████▀▄▀█████            ";
		gmGameOver += L"            ███████████             ";
		gmGameOver += L"            ███████████             ";
		gmGameOver += L"        ▄▄▄██  █▀█▀█  ██▄▄▄         ";
		gmGameOver += L"        ▀▀██           ██▀▀         ";
		gmGameOver += L"          ▀▀           ▀▀           ";
		gmGameOver += L"                                    ";
		gmGameOver += L"B--== Press <SPACE> to Restart ==-- ";
		gmGameOver += L"Y   --== Press <Q> to Quit ==--     ";

		//sGameOver += L"███▀▀▀██┼███▀▀▀███┼███▀█▄█▀███┼██▀▀▀";
		//sGameOver += L"██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼█┼┼┼██┼██┼┼┼";
		//sGameOver += L"██┼┼┼▄▄▄┼██▄▄▄▄▄██┼██┼┼┼▀┼┼┼██┼██▀▀▀";
		//sGameOver += L"██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██┼┼┼";
		//sGameOver += L"███▄▄▄██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██▄▄▄";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"███▀▀▀███┼▀███┼┼██▀┼██▀▀▀┼██▀▀▀▀██▄┼";
		//sGameOver += L"██┼┼┼┼┼██┼┼┼██┼┼██┼┼██┼┼┼┼██┼┼┼┼┼██┼";
		//sGameOver += L"██┼┼┼┼┼██┼┼┼██┼┼██┼┼██▀▀▀┼██▄▄▄▄▄▀▀┼";
		//sGameOver += L"██┼┼┼┼┼██┼┼┼██┼┼█▀┼┼██┼┼┼┼██┼┼┼┼┼██┼";
		//sGameOver += L"███▄▄▄███┼┼┼─▀█▀┼┼─┼██▄▄▄┼██┼┼┼┼┼██▄";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼████▄┼┼┼▄▄▄▄▄▄▄┼┼┼▄████┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼▀▀█▄█████████▄█▀▀┼┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼█████████████┼┼┼┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼██▀▀▀███▀▀▀██┼┼┼┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼██┼┼┼███┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼█████▀▄▀█████┼┼┼┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼┼███████████┼┼┼┼┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼▄▄▄██┼┼█▀█▀█┼┼██▄▄▄┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼▀▀██┼┼┼┼┼┼┼┼┼┼┼██▀▀┼┼┼┼┼┼┼┼┼";
		//sGameOver += L"┼┼┼┼┼┼┼┼┼┼▀▀┼┼┼┼┼┼┼┼┼┼┼▀▀┼┼┼┼┼┼┼┼┼┼┼";
		gmMainMenu += L"           /^./^.                                     ";
		gmMainMenu += L"         _|__|  O|                                    ";
		gmMainMenu += L"./     /~     ._/ .    --== Press Space to Start ==-- ";
		gmMainMenu += L" .____|__________/  .                                 ";
		gmMainMenu += L"        ._______      .                               ";
		gmMainMenu += L"                `.     .                 .            ";
		gmMainMenu += L"                  |     |                  .          ";
		gmMainMenu += L"                 /      /                    .        ";
		gmMainMenu += L"                /     /                       ..      ";
		gmMainMenu += L"              /      /                         . .    ";
		gmMainMenu += L"             /     /                            .  .  ";
		gmMainMenu += L"           /     /             _----_            .  . ";
		gmMainMenu += L"          /     /           _-~      ~-_         |   |";
		gmMainMenu += L"         (      (        _-~    _--_    ~-_     _/   |";
		gmMainMenu += L"          .      ~-____-~    _-~    ~-_    ~-_-~    / ";
		gmMainMenu += L"            ~-_           _-~          ~-_       _-~  ";
		gmMainMenu += L"               ~--______-~                ~-___-~     ";
		DrawRect(0, 0, ScreenWidth() - 1, ScreenHeight() - 1, PIXEL_SOLID, FG_GREEN);
		return true;
	}
	bool OnUserUpdate(float fElapsedTime)
	{

		// =========================================== START OF STATES ==============================================

		switch (STATE)
		{
		case MAINMENU_STATE:
			if (GetKey(VK_SPACE).bPressed)
				STATE = BEGIN_STATE;
			DrawMenu(gmMainMenu);
			break;
		case BEGIN_STATE:

			// Reset everything to known state
			bDead = false;
			nScore = 0;
			nSnakeDirection = 3;
			nRequestedDirection = nSnakeDirection;
			fTimePassed = 0.0f;
			snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15} };

			// For Testing
			//for (int i = 0; i < 500; i++)
			//	snake.push_back({ snake.back().x,snake.back().y });
			// nScore = snake.size() / 5 - 6;

			while (moveFood()); //keep moving food to random location until its not under the snake
			DrawRect(0, 0, ScreenWidth() - 1, ScreenHeight() - 1, PIXEL_SOLID, FG_GREEN);
			STATE = RUN_STATE;
			break;

		case RUN_STATE:
			if (GetKey(VK_SPACE).bPressed)
				STATE = PAUSE_STATE;

			// Check for Key presses and dont allow us to move directly back into our body.

			if (GetKey(VK_UP).bPressed && nSnakeDirection != 2)
				nRequestedDirection = 0;
			if (GetKey(VK_RIGHT).bPressed && nSnakeDirection != 3)
				nRequestedDirection = 1;
			if (GetKey(VK_DOWN).bPressed && nSnakeDirection != 0)
				nRequestedDirection = 2;
			if (GetKey(VK_LEFT).bPressed && nSnakeDirection != 1)
				nRequestedDirection = 3;


			//This is for Nokia style snake steering(from snakes perspective, makes it easy to do quick movmenets but the confusion often gets you killed)
			//if (GetKey(VK_LEFT).bPressed)
			//{
			//	nSnakeDirection++;
			//	if (nSnakeDirection == 4)
			//		nSnakeDirection = 0;
			//}
			//if (GetKey(VK_RIGHT).bPressed)
			//{
			//	nSnakeDirection--;
			//	if (nSnakeDirection == -1)
			//		nSnakeDirection = 3;
			//}

			//Only update snake location every <FPS>.0f frames
			fTimePassed += fElapsedTime;
			if (1 / fTimePassed <= 10.0f)
			{
				fTimePassed = 0.0f;
				nSnakeDirection = nRequestedDirection;
				switch (nSnakeDirection)
				{
				case 0: // UP
					snake.push_front({ snake.front().x,snake.front().y - 1 });
					break;

				case 1: //Right
					snake.push_front({ snake.front().x + 1,snake.front().y });
					break;

				case 2: // Down
					snake.push_front({ snake.front().x,snake.front().y + 1 });
					break;

				case 3: // Left
					snake.push_front({ snake.front().x - 1,snake.front().y });
					break;
				}
				//Remove segnment we just replaced
				snake.pop_back();
			}

			// Collision Detection for World
			if (snake.front().x < 1 || snake.front().x >= ScreenWidth() - 1 || snake.front().y < 1 || snake.front().y >= ScreenHeight() - 1)
				bDead = true;

			// Collision Detection for Snake/Tail
			for (std::list<coordinate>::iterator i = snake.begin(); i != snake.end(); i++)
				if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
					bDead = true;


			// Collision Detection for Food
			if (snake.front().x == nFood.x && snake.front().y == nFood.y)
			{
				nScore++;
				while (moveFood()); //keep moving food to random location until its not under the snake
				for (int i = 0; i < 5; i++)
					snake.push_back({ snake.back().x,snake.back().y });
			}

			// Draw Stuff for RUN_STATE
			Fill(1, 1, ScreenWidth() - 1, ScreenHeight() - 1, PIXEL_SOLID, FG_BLACK);
			// Draw Snake Body
			for (auto s : snake)
				Draw(s.x, s.y, bDead ? 'X' : PIXEL_SOLID, FG_CYAN);

			// Draw Snake Head
			Draw(snake.front().x, snake.front().y, bDead ? 'X' : PIXEL_SOLID, FG_BLUE);

			// Draw Food
			Draw(nFood.x, nFood.y, PIXEL_SOLID, FG_RED);

			DrawString(10, 0, L"Score: " + std::to_wstring(nScore));

			// Go To DEAD STATE otherwise stay here
			if (bDead) STATE = DEAD_STATE;
			break;

		case PAUSE_STATE:
			// Check for Key presses
			if (GetKey(VK_SPACE).bPressed)
				STATE = RUN_STATE;
			DrawMenu(gmPause);
			Sleep(10);

			// We are waiting for Space to go to RUN_STATE
			break;

		case DEAD_STATE:

			// Check for Key presses
			if (GetKey('Q').bPressed)
				bNotDone = false;
			if (GetKey(VK_SPACE).bPressed)
				STATE = BEGIN_STATE;

			DrawMenu(gmGameOver);
			Sleep(10);

			// We are waiting for Space to go to BEGIN_STATE
			break;
		}

		// =========================================== END OF STATES ==============================================

		return bNotDone;
	}
};


int main()
{
	Snake game;
	if (game.ConstructConsole(120, 80, 14, 14))
		game.Start();

	return 0;
}