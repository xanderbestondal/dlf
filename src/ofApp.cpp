#include "ofApp.h"
void ofApp::setup() {


	ofSetVerticalSync(true);
	ofSetFrameRate(24);
	fft.setup(16384);


	highestValue = 0.0;
	smoothedPitch = 0.0;
	lowestPitch = 100;
	highestPitch = 850;
	settingRange = false;

	//motionStype = "lowToHigh";
	motionStype = "pitch";

	shapeEnded = true;
	shapeStartFrame = 0;
	
	rebuildCurveCount = 100;

	for (float i = 0; i < rebuildCurveCount; i++) {
		smoothedPeakList.push_back(ofPoint(0, 0));
		//peakListSorted.push_back(ofPoint(0, 0));
	}


	string path = "";
	ofDirectory dir(path);
	//only show png files
	dir.allowExt("png");
	//populate the directory object
	dir.listDir();

	//go through and print out all the paths
	int randomSeed = 0;
	int randomNr = 0;
	for (int i = 0; i < dir.size(); i++) {
		int nr = contains_number(dir.getPath(i));
		string name = dir.getPath(i).substr(0,nr);

		if (!in_array(name, sequenceNames)) {
			sequenceNames.push_back(name);
			int endNr = 0;
			string numberString = "";
			for (int j = 0; j < dir.size(); j++) {
				int nr = contains_number(dir.getPath(j));
				if (nr != -1) {
					string nameSecondaryIter = dir.getPath(j).substr(0, nr);
					if (name == nameSecondaryIter) {
						numberString = dir.getPath(j).substr(nr, dir.getPath(j).rfind(".")- nr);
						endNr = stoi(numberString);
					}
				}
			}
			if (endNr != 0) {
				anim anim;
				anim.sequence = ofxImageSequence();
				anim.name = name;
				anim.sequence.loadSequence(name, "png", 1, endNr, numberString.size());
				anim.sequence.preloadAllFrames();
				for (unsigned i = 0; i<name.length(); ++i)
				{
					char c = name.at(i);
					randomSeed += (int)c;
					ofSeedRandom(randomSeed*10000);
					randomNr = ofRandom(100, 800);
				}
				//matchPitchList.push_back(randomNr);
				animList.push_back(anim);
			}
		}
	}

	animList[0].matchPitch = 400;
	animList[0].checkPointList.push_back(52);
	animList[0].checkPointList.push_back(85);
	animList[0].checkPointList.push_back(124);
	//matchPitchList[0] = 400;

	//playing = true; //controls if playing automatically, or controlled by the mouse

	holdTime = 0;
}
bool ofApp::in_array(const string &value, const vector<string> &array)
{
	return find(array.begin(), array.end(), value) != array.end();
}
int ofApp::contains_number(const std::string &c)
{
	return int(c.find_first_of("0123456789"));
}

bool sortMe(ofPoint & a, ofPoint & b) {
	if (a[1] > b[1]) {
		return true;
	}
	else {
		return false;
	}
}

void ofApp::update() {
	fft.update();
}

void ofApp::draw() {
	//ofBackground(0, 0, 0);
	//sequence.getFrameForTime(smoothedPitch/10)->draw(0, 0);
	
	//sequence.getFrame(ofClamp(smoothedPitch/ 550,0,1)*sequence.getTotalFrames()-1)->draw(300, 200);
	//ofTranslate(-ofGetWidth()/2, -ofGetHeight()/ 2);
	float zoom = 1.5;
	ofScale(zoom, zoom);
	for (int i = 0; i < animList.size(); i++) {

		int xPos = 0;
		int yPos = 0;

		if (motionStype == "lowToHigh") {
			animList[i].sequence.getFrame(ofClamp((smoothedPitch - lowestPitch) / (highestPitch - lowestPitch), 0, 1)*(animList[i].sequence.getTotalFrames() - 1))->draw(xPos, yPos);
		}
		if (motionStype == "pitch") {
			float accurateness = .02;
			float matchPitch = cos(ofClamp((smoothedPitch - animList[i].matchPitch)*accurateness, -PI / 2, PI / 2)); // the closer to 0 on X the closer to 1 as output
			
			//float matchPitch = abs(smoothedPitch/highestPitch - animList[i].matchPitch/highestPitch); // the closer to 0 on X the closer to 1 as output
			
			if (animList[i].sequenceTargetFrame > animList[i].checkPointList[animList[i].pointIndex]) {
				// 
				if (animList[i].frame < animList[i].checkPointList[animList[i].pointIndex]) {
					animList[i].frame += 1;
				}
				if (animList[i].frame == animList[i].checkPointList[animList[i].pointIndex]) {
					// point reached, now go to the next checkpoint.
					animList[i].pointIndex += 1;
				}
			}
			animList[i].sequenceTargetFrame += matchPitch*.5;
			cout << animList[i].pointIndex << endl;
			cout << animList[i].frame << endl;
			//if (animList[i].sequenceTargetFrame < animList[i].checkPointList[animList[i].pointIndex]) {
			//	// stay on the point that is reached
			//	animList[i].sequenceTargetFrame = animList[i].checkPointList[animList[i].pointIndex];
			//}
			//if (animList[i].sequenceTargetFrame > animList[i].sequence.getTotalFrames() - 1) {
			//	animList[i].sequenceTargetFrame = animList[i].sequence.getTotalFrames() - 1;
			//}

			//if (animList[i].sequenceTargetFrame < 10) {
			//	// start triggering
			//	animList[i].sequence.getFrame(animList[i].sequenceTargetFrame)->draw(xPos, yPos);
			//}
			//else {
				// play
				animList[i].sequence.getFrame(animList[i].frame)->draw(xPos, yPos);
			//}
		}
		//if ((animList[i].sequence.getCurrentFrame() + 4) > animList[i].sequence.getTotalFrames()) {
		//	holdTime += 1;
		//}else {
		//	if (holdTime > 800) {
		//		holdTime = 100;
		//	}
		//}
	}


	if (holdTime > 800) {
		//ofDrawBitmapString("____ I like you", 510, 275);
	}

	plot(fft.getBins());
}

void ofApp::keyPressed(int key) {
	if (key == 'r') {
		settingRange = true;
	}
	if (key == 't') {
		settingRange = false;
	}
}
void ofApp::keyReleased(int key) {
}

void ofApp::plot(vector<float>& buffer) {
	ofNoFill();
	int n = MIN(1024, buffer.size());

	highestValue = 1; // store highest volume of recorded pitch
	float highestBoredomValue = 0; // store position of pitch

	peakListSorted.clear();
	//peakList.clear();
	for (int i = lowestPitch; i < highestPitch-lowestPitch; i++) {
		float bufferValue = buffer[i] * 200;
		peakListSorted.push_back(ofPoint(i, bufferValue));
		//peakList.push_back(ofPoint(i, bufferValue));
		//peakListHistory[i] = ofPoint(i, buffer[i] + (peakListHistory[i][1])*.999);
	}
	ofSort(peakListSorted,sortMe);
	float highestPos = peakListSorted[0][0];

	smoothedPitch += (highestPos - smoothedPitch)*.1; // smooth de overgang van de vorige waarde door een gemiddelde te nemen."

	if (settingRange) {
		ofPolyline cp(peakList);
		cp.getResampledByCount(rebuildCurveCount).getSmoothed(5,1);
		for (int i = 0; i < rebuildCurveCount; i++) {
			smoothedPeakList[i] += (cp.getResampledByCount(rebuildCurveCount).getSmoothed(5, 1).getPointAtIndexInterpolated(i) - smoothedPeakList[i])*.1;
		}
		ofPolyline smoothed(smoothedPeakList);
		ofPushMatrix();
		ofTranslate(0, 200);
		ofScale(1, -1);
		smoothed.draw();
		smoothed.getSmoothed(5, 1).draw();
		smoothed.getSmoothed(10, 1).draw();
		//cp.draw();
		ofPopMatrix();


		ofSetColor(255, 0, 0);
		ofDrawBitmapString(smoothedPitch, smoothedPitch, 200 - highestValue * 100);
		ofDrawLine(lowestPitch, 205, highestPitch- lowestPitch, 205);
		ofSetColor(255, 255, 255);

	}


}
