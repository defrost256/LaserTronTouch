#include "wiringPi.h"

vector<GPIO_Port> WiringPi::OpenGPIO = vector<GPIO_Port>();

void WiringPi::pinMode(int pin, PinMode mode)
{
	stringstream sstr;
	if(!IsPinOpen(pin))
	{
		sstr << "echo " << pin << " > /sys/class/gpio/export";
		cout << "Executing1 " << sstr.str() << endl;	//DEBUG
		system(sstr.str().c_str());
		OpenGPIO.push_back(GPIO_Port(pin));
	}
	cout << "Setting direction\n";
	sstr.str(string());
	sstr << "echo ";
	if(mode == PinMode::OUTPUT)
		sstr << "out > ";
	if(mode == PinMode::INPUT)
		sstr << "in > ";
	GPIO_Port currentPort = PortByID(pin);
	sstr << currentPort.DirectionPath;
	cout << "Executing2 " << sstr.str() << endl;		//DEBUG
	system(sstr.str().c_str());
}

WiringPi::WireSignal WiringPi::digitalRead(int pin)
{
	fstream valueFile;
	valueFile.open(PortByID(pin).ValuePath.c_str());
	char c = valueFile.get();
	valueFile.close();
	return c == '0' ? WireSignal::LOW : WireSignal::HIGH;
}

GPIO_Port WiringPi::PortByID(int pin)
{
	for(int i = 0; i < OpenGPIO.size(); i++)
	{
		if(OpenGPIO[i].PortID == pin)
			return OpenGPIO[i];
	}
	return GPIO_Port(-1, "Error - no such GPIO port ");
}

bool WiringPi::IsPinOpen(int pin)
{
	return PortByID(pin).PortID > -1;
}

void WiringPi::closePin(int pin)
{
	cout << "GPIO close returned " << system(PortByID(pin).UnexportPath.c_str()) << "\n";
}

void WiringPi::clear()
{
	for(int i = 0; i < OpenGPIO.size(); i++)
		cout << "GPIO close " << OpenGPIO[i].PortID << " returned " << system(OpenGPIO[i].UnexportPath.c_str()) << "\n";
}
