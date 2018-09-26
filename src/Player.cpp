#include "Player.h"
#include "ofUtils.h"
#include "ofLog.h"
#include "Settings.h"

Player::Player(int id, int leftPin, int rightPin)
{
	ID = id;
	LeftPin = leftPin;
	RightPin = rightPin;
	WiringPi::pinMode(leftPin, WiringPi::INPUT);	//Set up pinModes
	WiringPi::pinMode(rightPin, WiringPi::INPUT);
	inputUpdateMillis = Settings::getSettings()->inputUpdateMillis;
	ofLogNotice() << "Player " << id << " created with pins(" << leftPin << "," << rightPin << ")";
	startThread();
}

Player::Player(const Player & p)
{
	ID = p.ID;
	bike = p.bike;
	LeftPin = p.LeftPin;
	RightPin = p.RightPin;
}

Player::~Player()
{
}

void Player::ExecuteInput()
{
	lock();
	if(hasInput)																//if there is new input
	{
		if(leftLast)															//check left or right
			bike->TurnCallback(true);
		else if(rightLast)
			bike->TurnCallback(false);
		hasInput = false;				//input was processed
	}
	unlock();
}

ofVec2f Player::GetPosition()
{
	return bike->getPosition();
}

int Player::GetDirection()
{
	return bike->getDirection();
}

int Player::GetID()
{
	return ID;
}

void Player::ProcessInput()
{
	if(hasInput)
		return;
	bool leftCurrent, rightCurrent;
	leftCurrent = (WiringPi::digitalRead(LeftPin) == WiringPi::HIGH);			//Check pins
	rightCurrent = (WiringPi::digitalRead(RightPin) == WiringPi::HIGH);
	//ofLogNotice() << ID << ":" << leftCurrent << "," << rightCurrent;
 	if(leftCurrent != leftLast)	//if there was change
	{
		if(leftCurrent && !hasInput)			//and the button is pressed
		{
			ofLogNotice("Player", "TURN LEFT");
			hasInput = true;	//set input state
		}
		leftLast = leftCurrent;
	}
	else if(rightCurrent != rightLast)
	{
		if(rightCurrent && !hasInput)
		{
			ofLogNotice("Player", "TURN RIGHT");
			hasInput = true;
		}
		rightLast = rightCurrent;
	}
}

void Player::threadedFunction()
{
	while (isThreadRunning())
	{
		lock();
		ProcessInput();		//process input
		unlock();
		ofSleepMillis(inputUpdateMillis);	//wait
	}
}
