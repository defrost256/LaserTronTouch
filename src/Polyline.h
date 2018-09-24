#pragma once

#include "ofVec2f.h"
#include "ofRectangle.h"

class PolyLine{

	std::vector<ofVec2f> points;
	int numPts = 0;

public:

	void AddPoint(ofVec2f point);
	void MoveLastPoint(ofVec2f pos);
	bool Intersects(ofRectangle rect);
	ofVec2f GetClosestPoint(ofVec2f p);
	void Clear();
};
