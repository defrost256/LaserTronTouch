#include "Polyline.h"

void PolyLine::AddPoint(ofVec2f p)
{
	points.push_back(p);
	numPts++;
}

void PolyLine::MoveLastPoint(ofVec2f pos)
{
	points.back().set(pos);
}

bool PolyLine::Intersects(ofRectangle rect)
{
	for(int i = 1; i < numPts; i++)
	{
		if(rect.intersects(glm::vec2(points[i-1].x, points[i-1].y), glm::vec2(points[i].x, points[i].y)))
		{
			return true;
		}
	}
	return false;
}

ofVec2f PolyLine::GetClosestPoint(ofVec2f p)
{
	ofVec2f ret;
	float retDsqr;
	for(int i = 1; i < numPts; i++)
	{
		ofVec2f p1 = points[i-1];
		ofVec2f p2 = points[i];
		float a = p1.y - p2.y;
		float b = p2.x - p1.x;
		float c = -(p1.y * b + p1.x * a);
		float s = b*p.x - a*p.y;
		float d = a*a+b*b;
		ofVec2f tmp((b*s-a*c)/d, (-a*s-b*c)/d);
		float tempD = p.squareDistance(tmp); 
		if(i == 1 || tempD < retDsqr)
		{
			ret.set(tmp);
			retDsqr = tempD;
		}
	}

	return ret;
}

void PolyLine::Clear()
{
	numPts = 0;
	points.clear();
}
