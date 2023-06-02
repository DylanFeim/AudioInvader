#pragma once
#include "ofMain.h"
#include "ofApp.h"

class laser {



public:

	laser(ofVec2f ship, float rot);
	~laser();

	ofVec2f loc;
	float angle;
	ofImage laserImg;
	bool visible;

	void update();
	void draw();
	void hide();
	ofVec2f getPos();


};