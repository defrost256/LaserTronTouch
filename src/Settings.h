#pragma once

#include <string>

class Settings
{
public:

	static Settings* getSettings();

	int updateMillis = 50;
	float bikeSpeed = 0.005f;
	float crashDist = 0.01f;
	float collisionFwd = 2.5f;
	int inputUpdateMillis = 50;
private:

	static Settings* settings;
	Settings();
	
	void ParseData(std::string token, std::string value);
};
