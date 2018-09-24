#pragma once

#include <cstdlib>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

struct GPIO_Port
{
	int PortID;
	string DirectionPath;
	string ValuePath;
	string UnexportPath;
	
	GPIO_Port(int id)
	{
		PortID = id;
		string basePath = "/sys/class/gpio/gpio";
		stringstream sstr;
		sstr << basePath << id << "/";
		basePath = sstr.str();
		sstr << "direction";
		DirectionPath = sstr.str();
		sstr.str(std::string());
		sstr << basePath << "value";
		ValuePath = sstr.str();
		sstr.str(std::string());
		sstr << "echo " << PortID;
		sstr << " > " << "/sys/class/gpio/unexport";
		UnexportPath = sstr.str();
	}
	
	GPIO_Port(int id, string path)
	{
		PortID = id;
		stringstream sstr;
		sstr << path << "direction";
		DirectionPath = sstr.str();
		sstr.str(path);
		sstr << "value";
		ValuePath = sstr.str();
		sstr.str(path.substr(0, path.size() - 5));
		sstr << "unexport " << PortID;
		UnexportPath = sstr.str();
	} 
};

class WiringPi
{
	
	
public:
	enum PinMode{ INPUT, OUTPUT };
	enum WireSignal{ HIGH, LOW };
	
	static void pinMode(int pin, PinMode mode);
	static WireSignal digitalRead(int pin);
	static void closePin(int pin);
	static void clear();

	static vector<GPIO_Port> OpenGPIO;
private:

	static bool IsPinOpen(int pin);
	static GPIO_Port PortByID(int pin);
};
