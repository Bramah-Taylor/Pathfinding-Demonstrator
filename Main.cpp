// Basic C++ application demonstrating the A* pathfinding algorithm in a two dimensional grid
// Uses Moore neighbourhood and Euclidean distances for pathfinding
// Works step-by-step, showing the algorithm progressing towards the end goal
// Uses SFML's graphics library to do window handling, sprite and text rendering
// SFML (Simple and Fast Multimedia Library) - Copyright (c) Laurent Gomila
// Available at: https://www.sfml-dev.org/index.php

#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include "Tile.h"

// Simple rounding function used to find the tile that mouse clicks happen within
int RoundDown(int i, int n)
{
	return (i / n) * n;

}

// Find the tile with the lowest F-cost
Tile* findTile(std::vector<Tile*> *openSet, int currentGCost, Tile* endTile)
{

	int newFCost = openSet->front()->fCost;
	Tile* tileToReturn = openSet->front();

	for (Tile* it : *openSet)
	{

		if (it->fCost < newFCost)
		{

			newFCost = it->fCost;

			tileToReturn = it;

		}

	}

	return tileToReturn;

}

int main()
{

	// Initialise the SFML render window to a 16:9 resolution running at 60 FPS
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Pathfinding Algorithm Application");
	window.setFramerateLimit(60);

	// Initialise the font we need for the text displayed on each grid tile
	sf::Font calibri;

	// Quit the program if the text can't be found - no point running it if we can't see the text
	if (!calibri.loadFromFile("calibri.ttf"))
	{

		return 0;

	}

	int mode = 0; // Default mode, obstacle placement mode = 1

	// Initialise the tiles
	const int GRID_DIMS_X = 32;
	const int GRID_DIMS_Y = 18;
	const float TILE_OFFSET = 50.0f;
	int tileOffset = TILE_OFFSET;
	int tileOffsetX = 0;
	int tileOffsetY = 0;

	Tile* tileGrid[GRID_DIMS_X * GRID_DIMS_Y];

	for (int y = 0; y < GRID_DIMS_Y; y++)
	{

		for (int x = 0; x < GRID_DIMS_X; x++)
		{

			tileGrid[(GRID_DIMS_X * y) + x] = new Tile(x * TILE_OFFSET, y * TILE_OFFSET, &calibri, TILE_OFFSET - 1.0f);

		}

	}

	// Set the tile neighbours
	for (int y = 0; y < GRID_DIMS_Y; y++)
	{

		for (int x = 0; x < GRID_DIMS_X; x++)
		{

			// Top left corner
			if (x == 0 && y == 0)
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(NULL,
					tileGrid[(GRID_DIMS_X * y) + x + 1],
					NULL,
					tileGrid[(GRID_DIMS_X * (y + 1)) + x]);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(NULL,
					NULL,
					NULL,
					tileGrid[(GRID_DIMS_X * (y + 1)) + x + 1]);

			}
			// Bottom right corner
			else if (x == GRID_DIMS_X - 1 && y == GRID_DIMS_Y - 1)
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(tileGrid[(GRID_DIMS_X * y) + x - 1],
					NULL,
					tileGrid[(GRID_DIMS_X * (y - 1)) + x],
					NULL);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(tileGrid[(GRID_DIMS_X * (y - 1)) + x - 1],
					NULL,
					NULL,
					NULL);

			}
			// Bottom left corner
			else if (x == 0 && y == GRID_DIMS_Y - 1)
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(NULL,
					tileGrid[(GRID_DIMS_X * y) + x + 1],
					tileGrid[(GRID_DIMS_X * (y - 1)) + x],
					NULL);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(NULL,
					tileGrid[(GRID_DIMS_X * (y - 1)) + x + 1],
					NULL,
					NULL);

			}
			// Top right corner
			else if (x == GRID_DIMS_X - 1 && y == 0)
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(tileGrid[(GRID_DIMS_X * y) + x - 1],
					NULL,
					NULL,
					tileGrid[(GRID_DIMS_X * (y + 1)) + x]);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(NULL,
					NULL,
					tileGrid[(GRID_DIMS_X * (y + 1)) + x - 1],
					NULL);

			}
			// Top edge
			else if (y == 0)
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(tileGrid[(GRID_DIMS_X * y) + x - 1],
					tileGrid[(GRID_DIMS_X * y) + x + 1],
					NULL,
					tileGrid[(GRID_DIMS_X * (y + 1)) + x]);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(NULL,
					NULL,
					tileGrid[(GRID_DIMS_X * (y + 1)) + x - 1],
					tileGrid[(GRID_DIMS_X * (y + 1)) + x + 1]);

			}
			// Bottom edge
			else if (y == GRID_DIMS_Y - 1)
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(tileGrid[(GRID_DIMS_X * y) + x - 1],
					tileGrid[(GRID_DIMS_X * y) + x + 1],
					tileGrid[(GRID_DIMS_X * (y - 1)) + x],
					NULL);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(tileGrid[(GRID_DIMS_X * (y - 1)) + x - 1],
					tileGrid[(GRID_DIMS_X * (y - 1)) + x + 1],
					NULL,
					NULL);

			}
			// Left edge
			else if (x == 0)
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(NULL,
					tileGrid[(GRID_DIMS_X * y) + x + 1],
					tileGrid[(GRID_DIMS_X * (y - 1)) + x],
					tileGrid[(GRID_DIMS_X * (y + 1)) + x]);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(NULL,
					tileGrid[(GRID_DIMS_X * (y - 1)) + x + 1],
					NULL,
					tileGrid[(GRID_DIMS_X * (y + 1)) + x + 1]);

			}
			// Right edge
			else if (x == GRID_DIMS_X - 1)
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(tileGrid[(GRID_DIMS_X * y) + x - 1],
					NULL,
					tileGrid[(GRID_DIMS_X * (y - 1)) + x],
					tileGrid[(GRID_DIMS_X * (y + 1)) + x]);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(tileGrid[(GRID_DIMS_X * (y - 1)) + x - 1],
					NULL,
					tileGrid[(GRID_DIMS_X * (y + 1)) + x - 1],
					NULL);

			}
			else
			{

				tileGrid[(GRID_DIMS_X * y) + x]->SetNeighbours(tileGrid[(GRID_DIMS_X * y) + x - 1],
					tileGrid[(GRID_DIMS_X * y) + x + 1],
					tileGrid[(GRID_DIMS_X * (y - 1)) + x],
					tileGrid[(GRID_DIMS_X * (y + 1)) + x]);
				tileGrid[(GRID_DIMS_X * y) + x]->SetCornerNeighbours(tileGrid[(GRID_DIMS_X * (y - 1)) + x - 1],
					tileGrid[(GRID_DIMS_X * (y - 1)) + x + 1],
					tileGrid[(GRID_DIMS_X * (y + 1)) + x - 1],
					tileGrid[(GRID_DIMS_X * (y + 1)) + x + 1]);

			}

		}

	}

	// Initialise values for start/end tile selection
	int lastSelectedStartTile = 0;
	int lastSelectedEndTile = 0;

	// Create the open & closed sets, and the path
	std::vector<Tile*> openSet;
	std::vector<Tile*> closedSet;

	bool isRunning = false;
	bool foundPath = false;
	int currentGCost = 0;
	Tile* currentTile = NULL;

	// Main program loop
	while (window.isOpen())
	{

		// SFML event handling
		sf::Event event;

		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
			{

				window.close();

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{

				window.close();

			}

			// Update tiles here
			for (int i = 0; i < GRID_DIMS_Y*GRID_DIMS_X; i++)
			{

				tileGrid[i]->Update();

			}

			// Update tile offsets when the grid moves
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{

				tileOffsetX += 1;

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{

				tileOffsetX -= 1;

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{

				tileOffsetY -= 1;

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{

				tileOffsetY += 1;

			}

			if (mode == 0)
			{

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
				{

					mode = 1;

				}

				// Select tile when the user clicks on it
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{

					// Deselect the last tile
					tileGrid[lastSelectedStartTile]->Deselect();

					// Get the mouse position and round down
					// This will be used to find the tile that the button press happened within
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

					int xRounded = RoundDown(mousePosition.x - tileOffsetX, tileOffset) + tileOffsetX;
					int yRounded = RoundDown(mousePosition.y - tileOffsetY, tileOffset) + tileOffsetY;

					// Loop to find the tile (could be done using [])
					for (int i = 0; i < GRID_DIMS_X * GRID_DIMS_Y; i++)
					{

						if (tileGrid[i]->GetPosition().x == xRounded && tileGrid[i]->GetPosition().y == yRounded)
						{

							if (!tileGrid[i]->IsObstacle())
							{

								// Select the tile
								tileGrid[i]->Select(sf::Color(0, 255, 0));

								// Update this to be the last selected tile
								lastSelectedStartTile = i;

							}

						}

					}

				}

				// Select tile when the user clicks on it
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
				{

					// Deselect the last tile
					tileGrid[lastSelectedEndTile]->Deselect();

					// Get the mouse position and round down
					// This will be used to find the tile that the button press happened within
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

					int xRounded = RoundDown(mousePosition.x - tileOffsetX, tileOffset) + tileOffsetX;
					int yRounded = RoundDown(mousePosition.y - tileOffsetY, tileOffset) + tileOffsetY;

					// Loop to find the tile (could be done using [])
					for (int i = 0; i < GRID_DIMS_X * GRID_DIMS_Y; i++)
					{

						if (tileGrid[i]->GetPosition().x == xRounded && tileGrid[i]->GetPosition().y == yRounded)
						{

							if (!tileGrid[i]->IsObstacle())
							{

								// Select the tile
								tileGrid[i]->Select(sf::Color(255, 0, 0));

								// Update this to be the last selected tile
								lastSelectedEndTile = i;

							}

						}

					}

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{

					if (tileGrid[lastSelectedStartTile]->IsSelected() && tileGrid[lastSelectedEndTile]->IsSelected())
					{

						// Run A*
						isRunning = true;

						// Start with the start tile
						tileGrid[lastSelectedStartTile]->Deselect();

						tileGrid[lastSelectedStartTile]->CalculateCosts(0, tileGrid[lastSelectedEndTile]);

						currentGCost = tileGrid[lastSelectedStartTile]->GetGCost();

						currentTile = tileGrid[lastSelectedStartTile];

						openSet.push_back(currentTile);

						currentTile->SetToOpen();

					}

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{

					for (int i = 0; i < GRID_DIMS_X * GRID_DIMS_Y; i++)
					{

						
						// Reset the tile
						tileGrid[i]->ResetTile();

					}

				}

			}

			else if (mode == 1)
			{

				// Switch back to default mode when L is pressed (needs to be different key because of SFML's input
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
				{

					mode = 0;

				}

				// Reset obstacle tiles
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{

					for (int i = 0; i < GRID_DIMS_X * GRID_DIMS_Y; i++)
					{

						if (tileGrid[i]->IsObstacle())
						{

							// Reset the tile
							tileGrid[i]->ResetTile();

						}

					}

				}

				// Select tile when the user clicks on it
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{

					// Get the mouse position and round down
					// This will be used to find the tile that the button press happened within
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

					int xRounded = RoundDown(mousePosition.x - tileOffsetX, tileOffset) + tileOffsetX;
					int yRounded = RoundDown(mousePosition.y - tileOffsetY, tileOffset) + tileOffsetY;

					// Loop to find the tile (could be done using [])
					for (int i = 0; i < GRID_DIMS_X * GRID_DIMS_Y; i++)
					{

						if (tileGrid[i]->GetPosition().x == xRounded && tileGrid[i]->GetPosition().y == yRounded)
						{

							// Select the tile
							tileGrid[i]->SetObstacle();

						}

					}

				}

			}

			// Running the A* algorithm
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				if (!openSet.empty())
				{

					// Find the next tile in the open set with the lowest cost
					currentTile = findTile(&openSet, currentGCost, tileGrid[lastSelectedEndTile]);

					currentGCost = currentTile->GetGCost();

					// Check if the current tile is the end tile
					if (currentTile == tileGrid[lastSelectedEndTile])
					{

						// If it is, we've found the desired path
						foundPath = true;

					}

					// Search the tile's neighbourhood
					currentTile->SearchNeighbourhood(currentGCost, tileGrid[lastSelectedEndTile], &openSet, &closedSet);

					// Remove the current tile from the open set
					openSet.erase(std::remove(openSet.begin(), openSet.end(), currentTile));

					// Add it to the closed set
					closedSet.push_back(currentTile);

					currentTile->SetToClosed();

					// Once a path has been found, trace back to find the path
					while (foundPath)
					{

						// Get the end tile and its parent tile
						tileGrid[lastSelectedEndTile]->SetToPath();
						Tile* nextTile = tileGrid[lastSelectedEndTile]->GetParentTile();

						// Repeat process until the start tile is found
						while (nextTile != tileGrid[lastSelectedStartTile])
						{

							nextTile->SetToPath();
							nextTile = nextTile->GetParentTile();

						}

						// Add the start tile to the path
						tileGrid[lastSelectedStartTile]->SetToPath();

						// Reset simulation values
						foundPath = false;
						lastSelectedStartTile = 0;
						lastSelectedEndTile = 0;
						currentTile = NULL;

						// Clear the vectors
						openSet.clear();
						closedSet.clear();

					}

					std::this_thread::sleep_for(std::chrono::milliseconds(100));

				}

			}

		}

		// Render tiles here
		window.clear();

		for (int i = 0; i < GRID_DIMS_Y*GRID_DIMS_X; i++)
		{

			tileGrid[i]->Render(&window);

		}

		window.display();

	}

	// Clear the tile objects from memory now the application is finished
	for (int i = 0; i < GRID_DIMS_X * GRID_DIMS_Y; i++)
	{

		delete tileGrid[i];
		tileGrid[i] = NULL;

	}

	return 0;

}