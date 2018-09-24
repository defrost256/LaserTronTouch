#include "Polyline.h"

#include "ofMath.h"
#include "ofLog.h"

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
		float len = p1.distance(p2);
		ofVec2f tmp;
		if(len == 0)
		{
			tmp = p1;
		}
		else
		{
			float t = ofClamp((p - p2).dot(p1 - p2) / (len * len),0.f,1.f);
			tmp = p2 + (t * (p1 - p2));
		}
		float tmpDst = p.squareDistance(tmp);
		if(tmpDst < retDsqr || i == 1)
		{
			retDsqr = tmpDst;
			ret.set(tmp);
		}
	}
	return ret;
}

void PolyLine::Clear()
{
	numPts = 0;
	points.clear();
}
