#pragma once

#include "ofMain.h"
#include "ofxEasyFft.h"

#include "ofxImageSequence.h"

class anim : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	int holdTime;
	// IMAGES STUFF

	string motionStype;
	string name;

	ofxImageSequence sequence;
	//string sequenceNames;
	int sequenceEnding = 0;
	float sequenceTargetFrame = 0;
	int matchPitch = 0;

	int frame = 0;

	int pointReached = 0;
	int pointIndex = 0;
	vector<int> checkPointList;

	bool playing;

};
