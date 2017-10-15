#pragma once

#include "ofMain.h"
#include "ofxEasyFft.h"
#include "anim.h"

#include "ofxImageSequence.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);

	int contains_number(const std::string &c);


	bool shapeEnded;
	int shapeStartFrame;

	void plot(vector<float>& buffer);
	bool in_array(const string &value, const vector<string> &array);

	ofxEasyFft fft;

	float highestValue;
	vector<float> highestValueList;
	float smoothedPitch;
	float lowestPitch;
	float highestPitch;
	bool settingRange;

	int rebuildCurveCount;
	vector<ofPoint> peakList;
	vector<ofPoint> peakListSorted;
	vector<ofPoint> smoothedPeakList;
	vector<ofPoint> peakListHistory;
	vector<ofPoint> peakListHistorySorted;
	vector<ofPoint> boredomList; //(like a second history...)
	vector<ofPoint> pointsOfInterest; //(like a second history...)


	int holdTime;
	// IMAGES STUFF

	//anim sequence;

	string motionStype;

	vector<anim> animList;
	vector<string> sequenceNames;

	// old 
	//vector<int> sequenceEndings;
	//vector<float> sequenceTargetFrames;
	//vector<int> matchPitchList;
	//vector<int> matchPitchListCorrected;

	//vector<int> sequenceplayFrames;

	//int pointReached = 0;
	//vector<int> checkPointList;

	
	//bool playing;

};
