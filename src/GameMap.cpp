#include "GameMap.h"
#include "ofLog.h"
#include "Settings.h"

GameMap::GameMap(const vector<Player*> &players)
{
	Players = vector<Player*>(players.begin(), players.end());
	for (auto it = Players.begin(); it != Players.end(); it++)		//For each player
	{
		(*it)->Map = this;											//assign this map
		BikeWall *wall = new BikeWall();				//create a wall
		//int SpawnIdx = GetSpawnPointIdx();							//get a spawn point
		int SpawnIdx = Bikes.size();
		Bike *bike = new Bike(wall, SpawnPoints[SpawnIdx], GetStartDirection(SpawnIdx));		//Create a bike
		(*it)->bike = bike;
		Walls.push_back(wall);
		Bikes.push_back(bike);
	}
	LiveBikes = Bikes.size();
	crashDistance = Settings::getSettings()->crashDist;
	collisionFwd = Settings::getSettings()->collisionFwd;
	positiveDir = Settings::getSettings()->positiveDir;
}

GameMap::~GameMap()
{
}

int GameMap::CheckForCollision()							///todo refactor loops
{
	for (int i = 0; i < Bikes.size(); i++)					//check bike-mapBounds collisions
	{
		Bike* currentBike = Bikes[i];
		if(!currentBike->isAlive())
			continue;
		ofVec2f nextP = currentBike->getNextPosition(collisionFwd);
		for(int j = 0; j < Walls.size(); j++)
		{
			float dst = 0;
			ofVec2f cp = Walls[j]->getClosestPointTo(nextP, &dst);
			if(dst <= crashDistance)
			{
				KillBike(i);
				ofLogNotice() << "Killed by wall";
			}
		}
		if(!currentBike->isAlive())
			continue;
		if (nextP.x <= crashDistance || nextP.x >= (1 - crashDistance) || nextP.y <= crashDistance || nextP.y >= (1 - crashDistance))
		{
			ofLogNotice() << "Killed by map";
			KillBike(i);
		}
		if(!currentBike->isAlive())
			continue;
		nextP.set(currentBike->getPosition());
		for (int j = i + 1; j < Bikes.size(); j++)			//check bike-bike collisions
		{
			if(nextP.distance(Bikes[j]->getPosition()) <= crashDistance * 2)
			{
				ofLogNotice() << "Killed by other bike";
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
	//int SpawnIdx = GetSpawnPointIdx();		//Get spawn point
	int SpawnIdx = idx;
	Bikes[idx]->Respawn(SpawnPoints[SpawnIdx], GetStartDirection(SpawnIdx));	//respawn bike
	LiveBikes++;
}

void GameMap::Reset()
{
	for(int i = 0; i < Bikes.size(); i++)
	{
		Bikes[i]->Kill();	//Kill bikes (only dead bikes can respawn)
		//int SpawnIdx = GetSpawnPointIdx();
		int SpawnIdx = i;
		Bikes[i]->Respawn(SpawnPoints[SpawnIdx], GetStartDirection(SpawnIdx));
	}
	LiveBikes = Bikes.size();
}

void GameMap::AddBoxWall(ofRectangle rect)
{
	BoxWalls.push_back(rect);
}

void GameMap::ClearBoxWalls()
{
	BoxWalls.clear();
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

Bike::Base4Direction GameMap::GetStartDirection(int spawnIdx)
{
	//return Bike::Base4Direction((spawnIdx + 1) % 4);
	if(positiveDir)
	{
		if(spawnIdx == 0 || spawnIdx == 3)
			return Bike::Base4Direction(spawnIdx);
		if(spawnIdx == 2)
			return Bike::Base4Direction(1);
		return Bike::Base4Direction(2);
	}
	else
	{
		if(spawnIdx == 0 || spawnIdx == 3)
			return Bike::Base4Direction((spawnIdx + 2) % 4);
		if(spawnIdx == 1)
			return Bike::Base4Direction(0);
		return Bike::Base4Direction(3);
	}
}
