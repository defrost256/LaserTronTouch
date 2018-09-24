#pragma once

#include "ofVec2f.h"
#include "BikeWall.h"
#include "ofRectangle.h"

/**
 * Class representing a Player on the playfield. Contains and updates the status, and the transform of the player
 */
class Bike
{
public:

	/**
	 * Cardinal direction
	 */
	enum Base4Direction { Down, Right, Up, Left };

public:
	/**
	 * Creates a new Bike and places it on the playfield
	 * @param wall The wall object the path of the bike is written to
	 * @param startPos The starting position of the bike in normalized space
	 * @param startDir The starting cardinal direction of the bike
	 * @param color The color of the bike (not implemented)
	 */
	Bike(BikeWall* wall, ofVec2f startPos, Base4Direction startDir);
	~Bike();

	/**
	 * Updates the position of the bike based on the velocity of it.
	 * Also extends the wall, and updates the hitbox of the bike.
	 * @param frame (not used)
	 */
	void Update();
	/**
	 * Resets the position, direction, hitbox, and state of the bike.
	 * Clears the walls.
	 * @param startPos The position the bike resets to in normalized space.
	 * @param startDir The cardinal direction the bike resets to.
	 */
	void Respawn(ofVec2f startPos, Base4Direction startDir);

#ifdef PI
	/**
	 * __PI__
	 * Turns the bike to the left or right based on param.
	 * @param left If true, the bike turns to the left, otherwise to the right.
	 */
	void TurnCallback(bool left);
#else
	/**
	 * __WIN__
	 * Turns the bike to the specified direction.
	 * @param idx The direction of the bike (0-3: Up, Left, Down, Right)
	 */
	void TurnCallback(int idx);
#endif

	/**
	 *
	 * @return The normalized position of the bike on the playfield
	 */
	ofVec2f getPosition();
	ofVec2f getNextPosition(float scale);

	int getDirection();
	/**
	 * Returns the rectangle where the collision of the bike is detected as an AABB.
	 * @return The hitbox of the bike
	 */
	ofRectangle getHitBox();

	bool isAlive();

	void Kill();/**< Kills the bike removing it from the playfield. Sets the state of the bike to dead, and clears its walls */

private:

	BikeWall* Wall;				///< The wall object the path of the bike is written to.
	ofVec2f Position;			///< The current position of the bike in normalized coordinates.
	ofVec2f Direction;			///< The current velocity of the bike represented as a vector
	Base4Direction _Direction;	///< The current cardinal direction of the bike

	float Speed = 0.005;		///< The speed of the bike (the velocity is calculated based on this)
	bool alive = false;			///< The state of the bike (alive or dead)

	ofRectangle Hitbox;			///< The hitbox of the bike

	/**
	 * Private member for spawning.
	 * @param startPos
	 * @param startDir
	 */
	void Spawn(ofVec2f startPos, Base4Direction startDir);
	/**
	 * Private member for turning.
	 * @param Left
	 */
	void Turn(bool Left);
	/**
	 * Sets the velocity of the bike based on cardinal directions
	 * @param d The cardinal direction to turn towards.
	 */
	void SetDirection(Base4Direction d);
};

