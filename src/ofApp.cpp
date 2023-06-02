#include "ofApp.h"
#include "Math.h"

vector<Particle> particles;
vector<laser> lasers;

void ofApp::setup() 
{
	ofSetVerticalSync(true);
    ofSetWindowShape(WIDTH, HEIGHT );
    ofSetFrameRate(240);
    ofSetRectMode(OF_RECTMODE_CENTER);

    // jump to a specific time in our song (good for testing)
    // m_audioAnalyser.setPositionMS( 30 * 1000 ); // 30 seconds in (30000 ms)
	// note that this audioAnalyzer uses a modified version of ofFmodSOundPlayer so though it has the same+ functionality it should not be mixed with ofSound / ofFmodSoundPlayer

    // Set up the audio analyser:
    //     int _numBinsPerAverage: size in bins per average number from the full 513 bin spectrum
    //
    //     As an example, if _numBinsPerAverage is set to 10, every 10 bins from the full 513 bin spectrum
    //     will be averaged. You would have 513/10 averages in your set of linear averages.
    //     This would be 52 bins (51 + 1 bins as 10 does not go into 513 evenly).
    //     The reason for doing this is to get a smaller set of frequency ranges to work with. In the full 513
    //     bin spectrum, the each bin is 22,500hz/513 wide -> 43.8 Hz.
    //     If you average them into groups of 10, each bin is 438 Hz wide, which can be easier to work with

    // IMPORTANT TIP: Do not use the 20 that I have here, find a value that works well for your chosen song.
    // You want the lowest value that gives you the ability to clearly isolate the features of your song that
    // are interested in
    m_audioAnalyser.init("song.mp3", 15 );
	m_audioAnalyser.setLoop(true);
	m_audioAnalyser.play();
  
    shipImage.load("ship.png");
    showInterface = false;
    shipPosTarget.set(shipPos.x, shipPos.y);

    for (int i = 0; i < SMOOTH; i++) {
    
        loudHistory[i] = 0.0f;
    }

    for (int i = 0; i < SMOOTH2; i++) {

        soundHistory[i] = 0.0f;
    }

  

    totalLoud = 0;
    skewedLoud = 0;
    spawnRate = 0;
 
    totalSound = 0;
    skewedSound = 0;
  
    ofSetCircleResolution(48);

    circleCol.setHue(75);
    circleCol.setBrightness(255);
    circleCol.setSaturation(180);
    backgroundCol.setHex(0);
    backgroundCol.setSaturation(100);
}

void ofApp::update() 
{
    m_audioAnalyser.update();
    m_mousePos.set(ofGetMouseX(), ofGetMouseY());

    //Ship
    shipPosTarget.set(m_mousePos.x, m_mousePos.y);
    shipPos.x = lerp(shipPos.x, shipPosTarget.x, 0.050);
    shipPos.y = lerp(shipPos.y, shipPosTarget.y, 0.050);

    ofVec2f vecToMouse = m_mousePos - shipPos;
    shipAngle = ofRadToDeg(atan2f(vecToMouse.y, vecToMouse.x));


    //skewedLoud = lerp(40.0f / (1.0f + 300.0f * pow(5.0f, -(m_audioAnalyser.getMixLevel() * 10.0f))), 10, 0.1);

    //Function I came up with playing around on a graphing calculator to skew the low values lower and high values higher
    skewedLoud = 100.0f / (10.0f + 500.0f * pow(5.0f, -(m_audioAnalyser.getMixLevel() * 10.0f)));
    loudHistory[arrPos] = skewedLoud;
    
    //Manage position for speed smooth array
    if (arrPos >= SMOOTH)
        arrPos = 0;
    else
        arrPos++;


    totalLoud = 0;
    for (int i = 0; i < SMOOTH; i++) {

       totalLoud+= loudHistory[i];
    }

    //cout << (totalLoud / SMOOTH * 1.0f) + 0.5f * 1.0f << endl;
    setSpeed((totalLoud/ SMOOTH*1.0f)+0.5f);

    setRadius(ofMap((totalLoud / SMOOTH * 1.0f), 0, 8, 120, 320, true));

    //cout << lerp(40.0f / (1.0f + 300.0f * pow(5.0f, -(m_audioAnalyser.getMixLevel() * 10.0f))), 10, 0.1) << "   -----------   " << m_audioAnalyser.getMixLevel() * 10.0f << endl;

    backgroundCol.setBrightness(ofMap(totalLoud / SMOOTH, 1, 8, 0, 70, true));

    shipAngle = lerp(shipAngle, 0, 0.8);

    spawnRate = (ofMap((totalLoud / SMOOTH * 1.0f), 0, 8, 0, 0.4, true));

    //cout << pow(spawnRate, 1.2)/2 << endl;
    if(ofRandom(0, 1)< pow(spawnRate, 1.2)/2)
    particles.push_back(Particle());


    for (int i = 0; i < particles.size(); i++) {
            particles[i].update();
            particles[i].avoid(shipPos);
    }

    for (int i = 0; i < lasers.size(); i++) {
        lasers[i].update();
    }
    

    for (int i = 0; i < lasers.size(); i++) {
        for (int k = 0; k < particles.size(); k++) {
            if(lasers[i].visible == true && particles[k].visible)
            if (dist(lasers[i].getPos(), particles[k].getPos()) < particleSize) {
                lasers[i].hide();
                particles[k].hide();
            }
        }
    }

}


void ofApp::draw() 
{
    ofBackground(backgroundCol);


    ofSetColor(circleCol);

    //Erase / draw particles
    for (int i = 0; i < particles.size(); i++) {
        if (particles[i].getX() < 0) {
            particles[i].hide();
            particles.erase(particles.begin() + i);
        }
        else
            particles[i].draw();
    }

    for (int i = 0; i < lasers.size(); i++) {
        lasers[i].draw();
    }


    ofSetColor(255);

    ofPushMatrix();
    ofTranslate(shipPos.x, shipPos.y);
    ofRotateDeg(shipAngle);
    ofScale(.25, .25);
    shipImage.draw(0, 0);
    ofRotateDeg(-shipAngle);
    ofPopMatrix();
    ofSetColor(255);


    // Get the decibel levels for the frequency bins we are interested in
    float drums = m_audioAnalyser.getLinearAverage(0);
    // you can add multiple bins together if you notice that two have activity when there's something you would like to track
    float tone1 = m_audioAnalyser.getLinearAverage(3);
    float snare = m_audioAnalyser.getLinearAverage(2);
    // Advanced: can also get a custom average for a frequency range if you know the frequencies (can get them from mousing over the full spectrum)
    // float customAverage = m_audioAnalyser.getAvgForFreqRange( 128.0f, 300.0f );

    // Draw circles to indicate activity in the frequency ranges we are interested in
    // Must be remapped using the ranges of activity that we want
    m_circle1Brightness = ofMap(drums, 143.0f, 200.0f, 0.0f, 1.0f, true);
    m_circle2Brightness = ofMap(tone1, 100.0f, 310.0f, 0.0f, 1.0f, true);
    m_circle4Brightness = ofMap(snare, 16.0f, 80.0f, 0.0f, 1.0f, true);

    //if(m_circle2Brightness > 0.6)
    //    lasers.push_back(laser(shipPos, shipAngle));

    circleCol.setHue(circleCol.getHue()+ m_circle1Brightness);
    backgroundCol.setHue(backgroundCol.getHue() + m_circle2Brightness);



    skewedSound =m_circle4Brightness* 30;

    //Smooth out particle radius
    soundHistory[arrPos2] = skewedSound;

    if (arrPos2 >= SMOOTH2)
        arrPos2 = 0;
    else
        arrPos2++;


    totalSound = 0;
    for (int i = 0; i < SMOOTH2; i++) {

        totalSound += soundHistory[i];
    }

    setSize((totalSound / SMOOTH2 * 1.0f));



    if (showInterface) {
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofBackground(ofColor::black);
        ofSetColor(255);

        // Volume Level
        ofSetColor(ofColor::white);
        ofDrawBitmapString("Volume Level", 140, 50);

        ofDrawCircle(100, 100, m_audioAnalyser.getLeftLevel() * 100.0f);
        ofDrawCircle(200, 100, m_audioAnalyser.getRightLevel() * 100.0f);
        ofDrawCircle(300, 100, m_audioAnalyser.getMixLevel() * 100.0f);

        ofDrawBitmapString("Left", 80, 170);
        ofDrawBitmapString("Right", 180, 170);
        ofDrawBitmapString("Mix", 290, 170);


        // Frequency / FFT information
        m_audioAnalyser.drawWaveform(40, 300, 1200, 90);
        m_audioAnalyser.drawSpectrum(40, 460, 1200, 128);
        m_audioAnalyser.drawLinearAverages(40, 650, 1200, 128);

    

        // Draw circles to indicate activity in the frequency ranges we are interested in
        ofSetColor(ofFloatColor(m_circle1Brightness, 0.0f, 0.0f));
        ofDrawCircle(500, 100, 50);

        ofSetColor(ofFloatColor(0.0f, m_circle2Brightness, 0.0f));
        ofDrawCircle(650, 100, 50);



        ofSetColor(ofFloatColor(m_circle4Brightness, m_circle4Brightness, m_circle4Brightness));
        ofDrawCircle(950, 100, 50);

        ofSetColor(ofColor::white);

        cout << m_circle4Brightness << endl;

        // song time in seconds. Can use m_soundPlayer.setPositionMS( time ) to jump to a specific time
        float songTimeSeconds = m_audioAnalyser.getPositionMS() / 1000.0f;
        ofDrawBitmapString("Song time: " + ofToString(songTimeSeconds), 40, 250);
    }
}

void ofApp::keyPressed(int key) {

    if (key == 32) {
        ofSetRectMode(OF_RECTMODE_CENTER);
        showInterface ^= 1;
    }

	//hmm wonder what kind of useful shortcuts we could add here to go through song more easily ... look at methods of AudioAnalyser ...
}

float ofApp::lerp(float start, float end, float percent)
{
    return (start + percent * (end - start));
}

float ofApp::dist(ofVec2f start, ofVec2f end)
{
    return (sqrt(pow((end.x - start.x), 2) + pow((end.y - start.y), 2)));
}

void ofApp::mousePressed(int x, int y, int button) {
    lasers.push_back(laser(shipPos, shipAngle));
}