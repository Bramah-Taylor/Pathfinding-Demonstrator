// Tile.cpp

#include "Tile.h"
#include <string>
#include <math.h>
#include <iostream>

// TODO: Comment everything

Tile::Tile(float x, float y, sf::Font* font, float size)
{

	position.x = x;
	position.y = y;

	// Initialise the sprite representing the tile
	sprite = new sf::RectangleShape(sf::Vector2f(size, size));
	sprite->setFillColor(sf::Color(255, 255, 255));
	sprite->setPosition(position);

	// Initialise the text on the tile
	// This text will represent the gCost and fCost variables on each tile
	textPosition.x = position.x + 1.0f;
	textPosition.y = position.y + ((size / 4) * 3);

	text = new sf::Text(sf::String(), *font, 10);
	text->setFillColor(sf::Color(0, 0, 0));
	text->setPosition(textPosition);

	// Initialise the tile pointers to null
	for (int i = 0; i < 8; i++)
	{

		neighbourhood[i] = NULL;

	}

	parent = NULL;

	// Set variables for A* algorithm to default to false & 0
	isObstacle = false;
	isSelected = false;
	isOpen = false;
	isClosed = false;

	gCost = 0;
	hCost = 0;
	fCost = 0;

}

// Tile cleanup
Tile::~Tile()
{

	// No need to call tile destructors here as they're called in main
	for (int i = 0; i < 8; i++)
	{

		neighbourhood[i] = NULL;

	}

	// Delete the SFML sprite object
	if (sprite)
	{

		delete sprite;
		sprite = NULL;

	}

	// Delete the SFML text object
	if (text)
	{

		delete text;
		text = NULL;

	}

}

// Updates the position of the tile based on player input
void Tile::Update()
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{

		position.x += 1.0f;
		textPosition.x += 1.0f;

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{

		position.x -= 1.0f;
		textPosition.x -= 1.0f;

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{

		position.y -= 1.0f;
		textPosition.y -= 1.0f;

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{

		position.y += 1.0f;
		textPosition.y += 1.0f;

	}

	sprite->setPosition(position);
	text->setPosition(textPosition);

}

// Draw the tile
void Tile::Render(sf::RenderWindow* window)
{

	window->draw(*sprite);
	window->draw(*text);

}

// Get the tile's position
sf::Vector2f Tile::GetPosition()
{

	return position;

}

// Set the tile's position; text position based on trial-and-error values for current tile size
void Tile::SetPosition(float x, float y)
{

	position = sf::Vector2f(x, y);
	textPosition.x = position.x + 1.0f;
	textPosition.y = position.y + 38.0f;

}

// Select the tile and indicate to the user that it's been selected
void Tile::Select(sf::Color colour)
{

	sprite->setFillColor(colour);

	isSelected = true;

}

// Deselet the tile
void Tile::Deselect()
{

	ResetTile();

	isSelected = false;

}

// Set the tile's immediate neighbours
void Tile::SetNeighbours(Tile* left, Tile* right, Tile* up, Tile* down)
{

	neighbourhood[0] = left;
	neighbourhood[1] = right;
	neighbourhood[2] = up;
	neighbourhood[3] = down;

}

// Set the tile's corner neighbours
void Tile::SetCornerNeighbours(Tile* topLeft, Tile* topRight, Tile* bottomLeft, Tile* bottomRight)
{

	neighbourhood[4] = topLeft;
	neighbourhood[5] = topRight;
	neighbourhood[6] = bottomLeft;
	neighbourhood[7] = bottomRight;

}

// Set the tile to be an obstacle and colour it black to indicate this
void Tile::SetObstacle()
{

	sprite->setFillColor(sf::Color(0, 0, 0));

	isObstacle = true;

}

// Reset the tile back to default
void Tile::ResetTile()
{

	sprite->setFillColor(sf::Color(255, 255, 255));

	isObstacle = false;
	isOpen = false;
	isClosed = false;

	gCost = 0;
	hCost = 0;
	fCost = 0;

	parent = NULL;

	text->setString("");

}

// Add the tile to the open set and colour it accordingly
void Tile::SetToOpen()
{

	sprite->setFillColor(sf::Color(100, 255, 10));
	isOpen = true;
	isClosed = false;

}

// Add the tile to the closed set and colour it accordingly
void Tile::SetToClosed()
{

	sprite->setFillColor(sf::Color(10, 125, 255));
	isOpen = false;
	isClosed = true;

}

// Find the costs for this tile and update text values accordingly
void Tile::CalculateCosts(int currentGCost, Tile* endTile)
{

	gCost = currentGCost;
	hCost = Heuristic(this, endTile);
	fCost = gCost + hCost;

	std::string string = std::to_string(gCost) + "    " + std::to_string(fCost);

	text->setString(string);

}

// Search the tile's neighbourhood for tiles that need to be updated
void Tile::SearchNeighbourhood(int currentGCost, Tile* endTile, std::vector<Tile*> *openSet, std::vector<Tile*> *closedSet)
{

	// Loop through the neighbourhood
	for (int i = 0; i < 8; i++)
	{

		// If there's a valid neighbour
		if (neighbourhood[i] != NULL)
		{

			// If it's not in the closed set and isn't an obstacle
			if (neighbourhood[i]->IsObstacle() == false)
			{

				// Get a new g-cost (cost of getting from start node to here)
				int newGCost = currentGCost + DistanceBetween(this, neighbourhood[i]);

				// If it's not in the open set, add it and calculate costs
				if (neighbourhood[i]->IsOpen() == true)
				{

					if (newGCost < neighbourhood[i]->GetGCost())
					{

						neighbourhood[i]->CalculateCosts(newGCost, endTile);
						neighbourhood[i]->SetParentNode(this);

					}

				}
				// Else if the new g-cost is an improvement, calculate costs and update parent node
				else if (neighbourhood[i]->IsClosed() == true)
				{

					if (newGCost < neighbourhood[i]->GetGCost())
					{

						openSet->push_back(neighbourhood[i]);

						neighbourhood[i]->CalculateCosts(newGCost, endTile);
						neighbourhood[i]->SetParentNode(this);

						neighbourhood[i]->SetToOpen();

						closedSet->erase(std::remove(closedSet->begin(), closedSet->end(), neighbourhood[i]));

					}

				}
				// Else add to open set
				else
				{

					openSet->push_back(neighbourhood[i]);

					neighbourhood[i]->CalculateCosts(newGCost, endTile);
					neighbourhood[i]->SetParentNode(this);

					neighbourhood[i]->SetToOpen();

				}

			}

		}

	}

}

// Find the distance between two tiles using Euclidean method
int Tile::DistanceBetween(Tile* tileA, Tile* tileB)
{

	int squareX = (tileB->GetPosition().x - tileA->GetPosition().x) * (tileB->GetPosition().x - tileA->GetPosition().x);
	int squareY = (tileB->GetPosition().y - tileA->GetPosition().y) * (tileB->GetPosition().y - tileA->GetPosition().y);

	return sqrt(squareX + squareY);

}

// Find the distance between two tiles using Euclidean method; functionally identical to DistanceBetween, but syntactically nicer
int Tile::Heuristic(Tile* currentTile, Tile* endTile)
{

	int squareX = (endTile->GetPosition().x - currentTile->GetPosition().x) * (endTile->GetPosition().x - currentTile->GetPosition().x);
	int squareY = (endTile->GetPosition().y - currentTile->GetPosition().y) * (endTile->GetPosition().y - currentTile->GetPosition().y);

	return sqrt(squareX + squareY);

}

// Indicate that this tile is part of the output path
void Tile::SetToPath()
{

	sprite->setFillColor(sf::Color(162, 20, 245));

}