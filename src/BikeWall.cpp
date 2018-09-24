#include "BikeWall.h"

BikeWall::BikeWall()
{
}

BikeWall::~BikeWall()
{
}

void BikeWall::ExtendLast(ofVec2f point)
{
	poly.MoveLastPoint(point);		//Remove the last point
}

void BikeWall::AddNewLine(ofVec2f point)
{
	poly.AddPoint(point);
}

void BikeWall::Clear()
{
	poly.Clear();
}

ofVec2f BikeWall::getClosestPointTo(ofVec2f p, float* outDistance)
{
	ofVec2f ret = poly.GetClosestPoint(p);
	if (outDistance != nullptr)
		*outDistance = ret.distance(p);
	return ret;
}

bool BikeWall::Intersects(ofRectangle shape)
{
	return poly.Intersects(shape);
}
