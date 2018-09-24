#pragma once

#include "ofMain.h"
#include "GameMap.h"
//#include "ofSerial.h"
#include "ofxNetwork.h"
/** We use this to interface with ofx, and to create the GameThread and the Etherdream
 */
class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		GameMap* Map;

		//ofSerial usb;
		ofxUDPManager* server;
		

		bool lastRestart = false;
		int restartPin = 4;
		
		int updateMillis = 500;

		int currentFrame = 0;
		
		std::vector<Player*> players;
		const int playerCount = 2;

		//const int bytesPerData = 4;
		//int dataScale;

		const ofColor playerColors[4] = {ofColor::green, ofColor::red, ofColor::blue, ofColor::yellow};
		const int playerPins[8] = {2, 3, 17, 18, 22, 23, 10, 24};

		void Restart();
		void CreatePlayers();
		void CreateServer();
		void SendData();
		
};
