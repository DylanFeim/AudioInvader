#pragma once
#include <vector>
#include "ofMain.h"
#include "AudioAnalyser.h"
#include "Particle.h"
#include "laser.h"
#include "globalVar.h"


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    AudioAnalyser		m_audioAnalyser;

    float           m_circle1Brightness;
    float           m_circle2Brightness;
    float           m_circle3Brightness;
    float           m_circle4Brightness;
    bool            showInterface;

    float           loudHistory[SMOOTH];
    float           skewedLoud;
    int             arrPos;
    float           totalLoud;

    float           soundHistory[SMOOTH];
    float           skewedSound;
    int             arrPos2;
    float           totalSound;


    float           spawnRate;

    float lerp(float start, float end, float percent);
    float dist(ofVec2f start, ofVec2f end);

    float shipAngle;

    ofImage shipImage;
    ofVec2f shipPos;
    ofVec2f shipPosTarget;
    ofVec2f m_mousePos;

    ofColor circleCol;
    ofColor backgroundCol;

    //vector<Particle> particles;
};
