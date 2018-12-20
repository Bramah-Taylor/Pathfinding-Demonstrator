# Pathfinding Demonstrator

This is a simple application designed to demonstrate the A* pathfinding algorithm on a 2 dimensional grid. This implementation of the algorithm traverses each cell's Moore neighbourhood and uses Euclidean distances as a heuristic. This application is **not** intended to demonstrate best practices for implementating the A* algorithm.

## Requirements

This application was built using SFML 2.5.1, and consequently requires the SFML binaries to run and SFML libraries to compile. SFML 2.5.1 can be downloaded [here](https://www.sfml-dev.org/download/sfml/2.5.1/).

## Instructions For Use

The code provided in this repository does not come with project files, so project setup is left at the user's discretion. It is advised that project setup follows the practices demonstrated in the tutorials on the SFML websites (accessible [here](https://www.sfml-dev.org/tutorials/2.5/)).

Once running, the application has the following controls:
 - Escape key to terminate the application
 - WASD keys to move the grid about the window
 - O key to enter obstacle mode, where users can click on tiles to convert them to obstacles
 - L key to leave obstacle mode
 - C key to clear the grid and reset it
 - R key to run a step of the A* algorithm
 - Left click to set the algorithm's start tile
 - Right click to set the algorithm's end tile (the tile it's trying to reach)

The algorithm runs one iteration at a time, waiting for the user to press the R key to run repeated iterations. The grid can be reset at any time.
