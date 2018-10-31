// Tile class - handles internal grid tile functionality, such as rendering
// Also handles neighbourhood searching, distance calculation and set management

#ifndef _TILE_H_
#define _TILE_H_

#include <SFML/Graphics.hpp>
#include <vector>

class Tile
{

public:

	// Constructor - pass in initial position and font for text initialisation
	Tile(float x, float y, sf::Font* font, float size);
	~Tile();

	// Update the sprite and text
	void Update();
	// Render the sprite and text
	void Render(sf::RenderWindow* window);

	// Get the position of the sprite
	sf::Vector2f GetPosition();
	// Get whether the tile is an obstacle
	bool IsObstacle() { return isObstacle; }
	bool IsSelected() { return isSelected; }
	bool IsOpen() { return isOpen; }
	bool IsClosed() { return isClosed; }
	int GetGCost() { return gCost; }
	Tile* GetParentTile() { return parent; }

	// Set the position of the sprite
	void SetPosition(float x, float y);
	// Set the neighbouring tiles for the tile
	// Tiles with missing neighbours can safely use null pointers as parameters
	void SetNeighbours(Tile* left, Tile* right, Tile* up, Tile* down);
	// Set the neighbouring corner tiles for the tile
	// Tiles with missing neighbours can safely use null pointers as parameters
	void SetCornerNeighbours(Tile* topLeft, Tile* topRight, Tile* bottomLeft, Tile* bottomRight);
	// Set the tile to be an obstacle
	void SetObstacle();
	// Reset the tile to blank
	void ResetTile();
	// Add to the open set
	void SetToOpen();
	// Add to the closed set
	void SetToClosed();
	void SetParentNode(Tile* tile) { parent = tile; }
	// Add to the final path
	void SetToPath();

	// Select function to display user selected tiles for A* algorithm and obstacle creation
	void Select(sf::Color colour);
	// Deselect a tile that has been selected by the user
	void Deselect();
	// Find the costs for this tile and update text values accordingly
	void CalculateCosts(int currentGCost, Tile* endTile);
	// Search the tile's neighbourhood for tiles that need to be updated
	void SearchNeighbourhood(int currentGCost, Tile* endTile, std::vector<Tile*> *openSet, std::vector<Tile*> *closedSet);

	// Find the distance between two tiles using Euclidean method
	int DistanceBetween(Tile* tileA, Tile* tileB);
	// Find the distance between two tiles using Euclidean method; functionally identical to DistanceBetween, but syntactically nicer
	int Heuristic(Tile* currentTile, Tile* endTile);

	// Distance from starting tile to current tile
	int gCost;
	// Estimated distance from current tile to end tile
	int hCost;
	// Summed distance
	int fCost;

private:

	// SFML objects used for grid square rendering
	sf::RectangleShape* sprite;
	sf::Text* text;

	// SFML objects used for positioning rendered objects
	sf::Vector2f position;
	sf::Vector2f textPosition;

	// Tile pointers for use in the A* algorithm
	Tile* neighbourhood[8];
	Tile* parent;

	// Boolean values for determining what set the tile's currently in
	bool isObstacle;
	bool isSelected;
	bool isOpen;
	bool isClosed;

};

#endif