#include "Settings.h"

#include "ofFileUtils.h"
#include "ofUtils.h"
#include "ofLog.h"

Settings* Settings::settings = nullptr;

Settings::Settings()
{
	ofBuffer file = ofBufferFromFile(ofFilePath::join(ofFilePath::getCurrentExeDir(), "settings.cfg"));
	ofBuffer::Lines lines = file.getLines();
	for(auto line = lines.begin(); line != lines.end(); line++)
	{
		std::vector<std::string> data = ofSplitString(*line, " ", true, true);
		if(data.size() == 2)
		{
			ParseData(data[0], data[1]);
			ofLogNotice() << "Parsed " << data[0] << " value: " << data[1];
		}
	}
}

Settings* Settings::getSettings()
{
	if(settings == nullptr)
	{
		settings = new Settings();
	}
	return settings;
}

void Settings::ParseData(std::string token, std::string value)
{
	if(token == "updateMillis")
		updateMillis = ofToInt(value);
	else if(token == "bikeSpeed")
		bikeSpeed = ofToFloat(value);
	else if(token == "crashDist")
		crashDist = ofToFloat(value);
	else if(token == "collisionFwd")
		collisionFwd = ofToFloat(value);
	else if(token == "inputUpdateMillis")
		inputUpdateMillis = ofToInt(value);
	else return;
}
