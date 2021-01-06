#include <iostream>
#include <chrono>
#include <thread>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Add LEFT key to slow down
// RIGHT key to speed up

// Override base class with your custom functionality
class GameOfLife : public olc::PixelGameEngine
{
public:
	GameOfLife()
	{
		sAppName = "Game Of Life";
	}

private:
	int* m_output;
	int* m_state;
	float fAccTime;
	float interval = 0.01f;

public:
	bool OnUserCreate() override
	{
		m_output = new int[ScreenWidth() * ScreenHeight()];
		m_state = new int[ScreenWidth() * ScreenHeight()];

		memset(m_output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
		memset(m_state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

		for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
			m_state[i] = rand() % 2;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Draw Border
		for (int x = 0; x < ScreenWidth(); x++)
		{
			Draw(x, 0, olc::BLUE);
			Draw(x, ScreenHeight()-1, olc::BLUE);
		}
		for (int y = 0; y < ScreenHeight(); y++)
		{
			Draw(0, y, olc::BLUE);
			Draw(ScreenWidth()-1, y, olc::BLUE);
		}

		// Get Keyboard Inputs
		if (GetKey(olc::Key::SPACE).bHeld)
			return true;
		if (GetKey(olc::Key::RIGHT).bHeld)
			interval = 0.001f;
		else
			interval = 0.1f;
		
		fAccTime += fElapsedTime;
		if (fAccTime > interval) {
			fAccTime -= interval;
			// Store output state
			for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
				m_output[i] = m_state[i];

			auto cell = [&](int x, int y)
			{
				return m_output[y * ScreenWidth() + x];
			};
			int border = 1;
			for (int x = 1; x < ScreenWidth() - border; x++)
				for (int y = 1; y < ScreenHeight() - border; y++)
				{
					int nNeighbours = cell(x - 1, y - 1) + cell(x - 0, y - 1) + cell(x + 1, y - 1) +
						cell(x - 1, y + 0) + 0 + cell(x + 1, y + 0) +
						cell(x - 1, y + 1) + cell(x + 0, y + 1) + cell(x + 1, y + 1);

					if (cell(x, y) == 1)
					{

						m_state[y * ScreenWidth() + x] = nNeighbours == 2 || nNeighbours == 3;
						Draw(x, y, olc::WHITE);
					}

					else
					{
						m_state[y * ScreenWidth() + x] = nNeighbours == 3;
						Draw(x, y, olc::BLACK);
					}
				}
		}

		return true;
	}
};


int main()
{
	// Seed random number generator
	srand(clock());

	// Use olcConsoleGameEngine derived app
	GameOfLife game;

	if (game.Construct(100, 100, 20, 20))
		game.Start();


	return 0;
}