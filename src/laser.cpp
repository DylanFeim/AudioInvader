#include "laser.h"


laser::laser(ofVec2f ship, float rot)
{

	loc.x = ship.x;
	loc.y = ship.y;
	angle = rot;

	laserImg.load("laser.png");
	visible = true;


}

laser::~laser()
{
}

void laser::update()
{
	if (visible) {
		loc.x = loc.x + (20 * cos(ofDegToRad(angle)));
		loc.y = loc.y + (20 * sin(ofDegToRad(angle)));
	}
}

void laser::draw()
{

	if (visible) {
		ofSetRectMode(OF_RECTMODE_CENTER);

		ofPushMatrix();
		//ofSetColor(255);
		ofTranslate(loc.x, loc.y);
		ofScale(.25, .25);
		ofRotateDeg(angle);
		laserImg.draw(0, 0);

		ofPopMatrix();

	}
}

void laser::hide() {
	visible = false;
}

ofVec2f laser::getPos() {
	return loc;
}

