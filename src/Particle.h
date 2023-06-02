#pragma once
#include "ofMain.h"
#include "ofApp.h"

class Particle {



public:

	Particle();
	~Particle();

	ofVec2f loc;
	float* size;
	bool visible;

	void update();
	void draw();
	float getX();
	float getY();
	ofVec2f getPos();
	
	void avoid(ofVec2f ship);
	void hide();

	float lerp(float start, float end, float percent);
	float dist(ofVec2f start, ofVec2f end);
};