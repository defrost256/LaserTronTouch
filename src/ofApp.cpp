#include "ofApp.h"

#include "wiringPi.h"
#include "ofUtils.h"
#include "Settings.h"
#include <sstream>
//--------------------------------------------------------------
void ofApp::setup(){
	updateMillis = Settings::getSettings()->updateMillis;
	ofLogNotice() << "SETUP";
	CreatePlayers();
	Map = new GameMap(players);
	WiringPi::pinMode(restartPin, WiringPi::INPUT);
	CreateServer();
}
//--------------------------------------------------------------
void ofApp::update(){
	bool restart = (WiringPi::digitalRead(restartPin) == WiringPi::HIGH);
	if(restart != lastRestart)
	{
		ofLogNotice() << "Restart change " << restart;
		if(restart)
			Restart();
		lastRestart = restart;
		/*while(usb.available() < 1)
		{
			ofSleepMillis(100);
		}
		usb.readByte();*/
	}
	for(auto it = players.begin(); it != players.end(); it++)
	{
		(*it)->ExecuteInput();
	}
	Map->Update();
	int aliveCnt = Map->CheckForCollision();
	if(aliveCnt == 1 && !hasWinner)
	{
		hasWinner = true;
		SendWinner();
	}
	SendData();
	ofSleepMillis(updateMillis);
}
//--------------------------------------------------------------
void ofApp::draw(){
	//Game->Draw();
	//ofSleepMillis(4);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	WiringPi::clear();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::Restart()
{
	//usb.writeBytes("reset", 5);
	std::string resetMsg = "reset";
	server->Send(resetMsg.c_str(), resetMsg.length());
	char countdownFinishedMsg[10000];
	ofLogNotice() << "Waiting for Restart signal from TD";
	bool done = false;
	while(!done)
	{
		while(server->Receive(countdownFinishedMsg, 10000) < 1)
		{
			ofSleepMillis(100);
		}
		std::string receivedMsg(countdownFinishedMsg);
		ofLogNotice() << "Msg received: " << receivedMsg;
		if(receivedMsg.substr(0, 4) == "done")
		{
			Map->Reset();
			hasWinner = false;

			std::string boxWallsStr = receivedMsg.substr(5);
			std::vector<ofRectangle> boxWalls = ParseBoxWallsFromMessage(boxWallsStr);

			for(ofRectangle rect : boxWalls)
			{
				AddBoxWall(rect);
			}

			done = true;
		}
	}
}

void ofApp::CreatePlayers()
{
	ofLogNotice() << "Creating " << playerCount << " players";
	for(int i = 0; i < playerCount; i++)
	{
		Player* p = new Player(i, playerPins[i * 2], playerPins[i * 2 + 1]);
		players.push_back(p);
	}
}

void ofApp::CreateServer()
{
	server = new ofxUDPManager();
	server->Create();
	server->SetEnableBroadcast(true);
	server->Connect("192.168.1.255", 7000);
	server->Bind(7001);
	server->SetNonBlocking(true);
}
/*
void ofApp::SendData()
{
	for(auto it = players.begin(); it != players.end(); it++)
	{
		ofVec2f p = (*it)->GetPosition();
		std::stringstream dataStr;
		//dataStr << ofToString(p.x, 8) << "," << ofToString(p.y, 8) << "," << (*it)->GetDirection() << "," << (*it)->GetID() << std::endl;
		dataStr << setprecision(8) << p.x << "," << setprecision(8) << p.y << "," << (*it)->GetDirection() << "," << (*it)->GetID() << std::endl;
		std::string dataString = dataStr.str();
		usb.writeBytes(dataString.c_str(), dataString.size());
		ofLogNotice() << "DATA " << dataStr.str() << ", (" << (*it)->GetDirection() << "," << (*it)->GetID() << ")" << dataString.size();
	}
}*/

void ofApp::SendData()
{
	for(auto it = players.begin(); it != players.end(); it++)
	{
		ofVec2f p = (*it)->GetPosition();
		std::stringstream dataStream;
		dataStream << ofToString(p.x, 8) << "," << ofToString(p.y, 8) << "," << (*it)->GetDirection() << "," << (*it)->GetID();
		std::string dataStr = dataStream.str();
		server->Send(dataStr.c_str(), dataStr.length());
	}
}

void ofApp::SendWinner()
{
	for(auto player = players.begin(); player != players.end(); player++)
	{
		if((*player)->bike->isAlive())
		{
			ofLogNotice() << "Winner is Player " << (*player)->GetID() + 1;
			std::stringstream winnerStream;
			winnerStream << "winner " << ofToString((*player)->GetID());
			std::string winnerStr = winnerStream.str();
			server->Send(winnerStr.c_str(), winnerStr.length());
			return;
		}
	}
}

std::vector<ofRectangle> ofApp::ParseBoxWallsFromMessage(std::string msg)
{
	std::vector<ofRectangle> ret;
	std::vector<std::string> rectStrs = ofSplitString(msg, "\t");
	for(std::string rectStr : rectStrs)
	{
		std::vector<std::string> rectAttrs = ofSplitString(rectStr, ",");
		if(rectAttrs.size() == 4)
			ret.push_back(ofRectangle(rectAttrs[0], rectAttrs[1], rectAttrs[2], rectAttrs[3]));
	}
	return ret;
}
