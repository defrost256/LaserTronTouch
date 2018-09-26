
#pragma once

#include "Bike.h"
#include "BikeWall.h"
#include "Player.h"
#include "ofVec2f.h"

class Player;

/**
 * Class representing the playfield.
 * Responsible for updating the state of the map.
 */
class GameMap
{
public:
	/**
	 * Creates a new map for the players specified by the params
	 * @param players The list players based on which the map is initialized
	 */
	GameMap(const std::vector<Player*> &players);
	~GameMap();

	/**
	 * Checks the map to detect collisions
	 * Kills any bike colliding with a wall, or the edge of the map, or any two bikes colliding with each other
	 * @return The number of bikes still alive after the collisions
	 */
	int CheckForCollision();

	/**
	 * Updates the elements on the map
	 * @param frame The Ilda queue to draw any map elements to
	 * @param soundShift Value produced by the sound engine to modulate the graphics
	 */
	void Update();
	/**
	 * Respawns the bike on the given spawn point
	 * @param idx The idx of the spawn point to respawn to
	 */
	void RespawnBike(int idx);
	/**
	 * Resets the map.
	 * Respawns the bikes.
	 */
	void Reset();

private:

	/**
	 * Returns a spawn point on the map.
	 * Tries return a point which is furthest from any living bike on the map
	 * @return The index of the selected spawn point
	 */
	int GetSpawnPointIdx();
	/**
	 * Kills the bike with the specified idx.
	 * @param idx The idx of the bike to kill
	 */
	void KillBike(int idx);

	Bike::Base4Direction GetStartDirection(int spawnIdx);

	std::vector<Bike*> Bikes;		///< List of the bikes on the map
	std::vector<BikeWall*> Walls;	///< List of the walls on the map
	std::vector<Player*> Players;	///< List of the players currently playing
	ofVec2f SpawnPoints[4] = { ofVec2f(0.05, 0.05), ofVec2f(0.95, 0.95),  ofVec2f(0.05, 0.95), ofVec2f(0.95, 0.05) };	///< List of the coordinates of possible spawn point
	float crashDistance = 0.01f;
	int LiveBikes = 0;			///< The number of bikes alive on the map
	float collisionFwd = 2.5f;
	bool positiveDir = true;
};
