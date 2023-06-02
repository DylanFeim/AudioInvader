#include "Particle.h"


Particle::Particle()
{
	
	loc.x = WIDTH;
	loc.y = ofRandom(0, HEIGHT);
	
	size = &particleSize;
	visible = true;
}

Particle::~Particle()
{
}

void Particle::update()
{
	if (visible)
	loc.x -= particleSpeed;

}

void Particle::draw()
{
	if(visible)
		ofDrawCircle(loc, *size);
}
float Particle::getX()
{
	return loc.x;
}

float Particle::getY()
{
	return loc.y;
}

void Particle::avoid(ofVec2f ship) {
	if(visible)
	if (dist(loc, ship) < radius) {
		if(loc.y>ship.y)
			loc.y = lerp(loc.y, ship.y + radius, 0.5);
		else
			loc.y = lerp(loc.y, ship.y - radius, 0.5);

	}
}

float Particle::lerp(float start, float end, float percent)
{
	return (start + percent * (end - start));
}

float Particle::dist(ofVec2f start, ofVec2f end)
{
	return (sqrt(pow((end.x - start.x), 2) + pow((end.y - start.y), 2)));
}

void Particle::hide() {
	visible = false;
}

ofVec2f Particle::getPos() {
	return loc;
}

