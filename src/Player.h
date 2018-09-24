#pragma once

#include "ofThread.h"
#include "GameMap.h"
#include "Bike.h"
#include "wiringPi.h"

class GameMap;

/**
 * Class representing a player.
 * Binds the bike, and its callers, and handles input
 */
class Player : public ofThread
{
public:
	/**
	 * __PI__
	 * Creates a new player with color and input pins.
	 * Also sets the GPIO pins up.
	 * @param color The color of the wall the player's bike draws
	 * @param leftPin The GPIO pin on which to listen to turn left input
	 * @param rightPin The GPIO pin on which to listen to turn right input
	 */
	Player(int ID, int leftPin, int rightPin);
	/**
	 * Copy constructor for players
	 * @param p the player to copy
	 */
	Player(const Player& p);
	~Player();

	int LeftPin;	///< The GPIO pin corresponding to left turn __PI__
	int RightPin;	///< The GPIO pin corresponding to right turn __PI__

	int ID;

	Bike* bike;		///< Pointer to the bike belonging to the player
	GameMap* Map;	///< Pointer to the map.

	/**
	 * __PI__
	 * Sends the last input to the bike.
	 * Mutex
	 */
	void ExecuteInput();

	ofVec2f GetPosition();
	int GetDirection();
	int GetID();
private:
	bool leftLast = false;	///< True if the state of the left input was PRESSED
	bool rightLast = false;	///< True if the state of the right input was PRESSED
	bool hasInput;			///< True if there was a new input

	/**
	 * Checks the input periferia, and updates the input states of the player accordingly.
	 * Mutex
	 */
	void ProcessInput();
	/**
	 * Loop for processing inputs, runs on a separate thread.
	 */
	void threadedFunction() override;
};
