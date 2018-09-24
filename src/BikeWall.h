#pragma once

#include "ofVec2f.h"
#include "Polyline.h"
#include "ofRectangle.h"

/**
 * Class representing a wall drawn by bikes as a series of line segments.
 */
class BikeWall
{
public:
	/**
	 * Creates a new wall with the specified color.
	 * @param c The color of the line.
	 */
	BikeWall();
	~BikeWall();

	/**
	 * Moves the last point to a new position.
	 * @param point The normalized position of the new point
	 */
	void ExtendLast(ofVec2f point);
	/**
	 * Adds a new point to the wall (thus adding a new line segment)
	 * @param point The normalized position of the new point
	 */
	void AddNewLine(ofVec2f point);

	/**
	 * Deletes the line segments
	 */
	void Clear();

	/**
	 * Returns the closest point on the wall to a specified point (in normalized coordinates)
	 * @param p The point
	 * @param outDistance Pointer to the distance, if needed (can be null)
	 * @return The Point on the wall closest to the specified point.
	 */
	ofVec2f getClosestPointTo(ofVec2f p, float* outDistance = nullptr);
	/**
	 * Checks if a rectangle intersects the wall.
	 * @param shape The rectangle
	 * @return True, if the rectangle intersects the wall.
	 */
	bool Intersects(ofRectangle shape);

	int BikeIdx; ///< The index of the bike this wall belongs to.
private:

	PolyLine poly; ///< The underlying polyline
};
