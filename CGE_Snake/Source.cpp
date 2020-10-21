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

	struct coordinate
	{
		int x;
		int y;
	};
	std::list<coordinate> snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15} };
	coordinate nFood = { 30,15 };
	int nScore = 0;
	int nSnakeDirection = 3;
	bool bDead = false;
	float myTime;
	bool notDone = true;

	std::wstring sGameOver;
private:
	bool OnUserCreate()
	{
		sGameOver += L"███▀▀▀██┼███▀▀▀███┼███▀█▄█▀███┼██▀▀▀.";
		sGameOver += L"██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼█┼┼┼██┼██┼┼┼.";
		sGameOver += L"██┼┼┼▄▄▄┼██▄▄▄▄▄██┼██┼┼┼▀┼┼┼██┼██▀▀▀.";
		sGameOver += L"██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██┼┼┼.";
		sGameOver += L"███▄▄▄██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██▄▄▄.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"███▀▀▀███┼▀███┼┼██▀┼██▀▀▀┼██▀▀▀▀██▄┼.";
		sGameOver += L"██┼┼┼┼┼██┼┼┼██┼┼██┼┼██┼┼┼┼██┼┼┼┼┼██┼.";
		sGameOver += L"██┼┼┼┼┼██┼┼┼██┼┼██┼┼██▀▀▀┼██▄▄▄▄▄▀▀┼.";
		sGameOver += L"██┼┼┼┼┼██┼┼┼██┼┼█▀┼┼██┼┼┼┼██┼┼┼┼┼██┼.";
		sGameOver += L"███▄▄▄███┼┼┼─▀█▀┼┼─┼██▄▄▄┼██┼┼┼┼┼██▄.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼████▄┼┼┼▄▄▄▄▄▄▄┼┼┼▄████┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼▀▀█▄█████████▄█▀▀┼┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼█████████████┼┼┼┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼██▀▀▀███▀▀▀██┼┼┼┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼██┼┼┼███┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼█████▀▄▀█████┼┼┼┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼┼┼┼███████████┼┼┼┼┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼▄▄▄██┼┼█▀█▀█┼┼██▄▄▄┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼▀▀██┼┼┼┼┼┼┼┼┼┼┼██▀▀┼┼┼┼┼┼┼┼┼.";
		sGameOver += L"┼┼┼┼┼┼┼┼┼┼▀▀┼┼┼┼┼┼┼┼┼┼┼▀▀┼┼┼┼┼┼┼┼┼┼┼.";

		DrawRect(0, 0, ScreenWidth() - 1, ScreenHeight() - 1, PIXEL_SOLID, FG_GREEN);
		return true;
	}
	bool OnUserUpdate(float fElapsedTime)
	{
		if (GetKey('Q').bPressed)
			notDone = false;

		if (GetKey(VK_LEFT).bPressed)
		{
			nSnakeDirection++;
			if (nSnakeDirection == 4)
				nSnakeDirection = 0;
		}
		if (GetKey(VK_RIGHT).bPressed)
		{
			nSnakeDirection--;
			if (nSnakeDirection == -1)
				nSnakeDirection = 3;
		}

		myTime += fElapsedTime;
		if (1 / myTime <= 10.0f && !bDead)
		{
			myTime = 0.0f;
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
			bool snakeOnFood = true;
			while (snakeOnFood)
			{
				nFood.x = (rand() % (ScreenWidth() - 2)) + 1; // make sure its not on the border
				nFood.y = (rand() % (ScreenHeight() - 2)) + 1;
				snakeOnFood = false;
				for (auto s : snake)
					if (nFood.x == s.x && nFood.y == s.y)
					{
						snakeOnFood = true;
					}
			}
			for (int i = 0; i < 5; i++)
				snake.push_back({ snake.back().x,snake.back().y });
		}

		// Draw Stuff
		Fill(1, 1, ScreenWidth() - 1, ScreenHeight() - 1, PIXEL_SOLID, FG_BLACK);
		// Draw Snake Body
		for (auto s : snake)
			Draw(s.x, s.y, bDead ? 'X' : PIXEL_SOLID, FG_CYAN);

		// Draw Snake Head
		Draw(snake.front().x, snake.front().y, bDead ? 'X' : PIXEL_SOLID, FG_BLUE);

		// Draw Food
		Draw(nFood.x, nFood.y, PIXEL_SOLID, FG_RED);

		DrawString(10, 0, L"Score: " + std::to_wstring(nScore));
		if(bDead)
		{
			int x = 0;
			int y = 0;
			int locationX = ScreenWidth() / 2 - 18;
			int locationY = ScreenHeight() / 2 - 18;
			DrawRect(locationX - 2, locationY -2, 39, 29, PIXEL_SOLID, FG_MAGENTA);
			for (size_t i = 0; i < sGameOver.size(); i++)
			{
				if (sGameOver[i] == L'.')
				{
					y++;
					x = 0;
					i += 1;
				}
				m_bufScreen[(locationY + y) * m_nScreenWidth + locationX + x].Char.UnicodeChar = sGameOver[i];
				m_bufScreen[(locationY + y) * m_nScreenWidth + locationX + x].Attributes = (sGameOver[i] == L'█' || sGameOver[i] == L'▄' || sGameOver[i] == L'▀') ? FG_WHITE : FG_RED;
				x++;
			}
			std::wstring text = L"--== Press <SPACE> to Restart ==--";
			DrawString(ScreenWidth() / 2 - text.size() / 2, locationY + y + 1, text, FG_BLUE);
			text = L"--== Press <Q> to Quit ==--";
			DrawString(ScreenWidth() / 2 - text.size() / 2, locationY + y + 2, text, FG_YELLOW);
			if (GetKey(VK_SPACE).bPressed)
			{
				bDead = false;
				nScore = 0;
				nSnakeDirection = 3;
				snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15} };
				DrawRect(0, 0, ScreenWidth() - 1, ScreenHeight() - 1, PIXEL_SOLID, FG_GREEN);
			}
			Sleep(100);
		}
		return notDone;
	}
};


int main()
{
	Snake game;
	if (game.ConstructConsole(120, 80, 14, 14))
		game.Start();

	return 0;
}