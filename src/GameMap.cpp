#include "GameMap.h"
#include "ofLog.h"

GameMap::GameMap(const vector<Player*> &players)
{
	Players = vector<Player*>(players.begin(), players.end());
	for (auto it = Players.begin(); it != Players.end(); it++)		//For each player
	{
		(*it)->Map = this;											//assign this map
		BikeWall *wall = new BikeWall();				//create a wall
		int SpawnIdx = GetSpawnPointIdx();							//get a spawn point
		Bike *bike = new Bike(wall, SpawnPoints[SpawnIdx], Bike::Base4Direction((SpawnIdx + 1) % 4));		//Create a bike
		(*it)->bike = bike;
		Walls.push_back(wall);
		Bikes.push_back(bike);
	}
}

GameMap::~GameMap()
{
}

int GameMap::CheckForCollision()							///todo refactor loops
{
	for (int i = 0; i < Walls.size(); i++)					//check bike-wall collisions
	{
		for (int j = 0; j < Bikes.size(); j++)
		{
			ofRectangle hitBox = Bikes[j]->getHitBox();
			if (Walls[i]->Intersects(hitBox))
			{
				KillBike(j);
			}
		}
	}
	for (int i = 0; i < Bikes.size(); i++)					//check bike-mapBounds collisions
	{
		ofRectangle hitBox = Bikes[i]->getHitBox();
		if (hitBox.intersects(ofVec2f(0, 0), ofVec2f(0, 1)) ||
			hitBox.intersects(ofVec2f(0, 0), ofVec2f(1, 0)) ||
			hitBox.intersects(ofVec2f(0, 1), ofVec2f(1, 1)) ||
			hitBox.intersects(ofVec2f(1, 0), ofVec2f(1, 1)))
		{
			KillBike(i);
		}
		for (int j = i + 1; j < Bikes.size(); j++)			//check bike-bike collisions
		{
			if (Bikes[j]->getHitBox().intersects(hitBox))
			{
				KillBike(i);
				KillBike(j);
			}
		}
	}
	return LiveBikes;
}

void GameMap::Update()
{
	for (int i = 0; i < Bikes.size(); i++)	//Update bikes
	{
		Bikes[i]->Update();
	}
}

void GameMap::RespawnBike(int idx)
{
	int SpawnIdx = GetSpawnPointIdx();		//Get spawn point
	Bikes[idx]->Respawn(SpawnPoints[SpawnIdx], Bike::Base4Direction((SpawnIdx + 1) % 4));	//respawn bike
	LiveBikes++;
}

void GameMap::Reset()
{
	for(int i = 0; i < Bikes.size(); i++)
	{
		Bikes[i]->Kill();	//Kill bikes (only dead bikes can respawn)
		int SpawnIdx = GetSpawnPointIdx();
		Bikes[i]->Respawn(SpawnPoints[SpawnIdx], Bike::Base4Direction((SpawnIdx + 1) % 4));
	}
}
int GameMap::GetSpawnPointIdx()
{
	if (Bikes.size() == 0 || LiveBikes == 0)	//if this is the first bike
		return rand() % 4;						//choose at random
	int furthestIdx = 0;
	float biggestDst = 0;
	float currentDst = 0;
	for (int i = 0; i < 4; i++)					//check which point is furthest from all the bikes (accumulated distance)
	{
		currentDst = 0;
		for (auto it = Bikes.begin(); it != Bikes.end(); it++)
		{
			currentDst += (*it)->getPosition().distance(SpawnPoints[i]);
		}
		if (currentDst > biggestDst)
		{
			biggestDst = currentDst;
			furthestIdx = i;
		}
	}
	return furthestIdx;
}

void GameMap::KillBike(int idx)
{
	ofLogNotice("Map", "KILL");
	Bikes[idx]->Kill();
	LiveBikes--;
}
