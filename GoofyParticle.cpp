//
//  GoofyParticle.cpp
//
//  Created by xxx on 21/09/13.
//
//

#include "GoofyParticle.h"

uint GoofyParticle::count = 0;

GoofyParticle::GoofyParticle(ofVec2f position, float limitVelocity, ofColor color, float spring, float friction, long int life)
{
    velocity.x = velocity.y = 0;
    acceleration.x = acceleration.y = 0;
    this->position = position;
    this->limitVelocity = limitVelocity;
    color = color;
    active = true;
    setBoundingBox(ofRectangle(0,0, ofGetWindowWidth(), ofGetWindowHeight()));
    bCheckBoundingBox = false;
    this->spring =  spring;
    this->friction = friction;
    id = GoofyParticle::count;
    GoofyParticle::count++;
    this->life = life;
    if(life != 0)
        lifeActive = true;
    else
        lifeActive = false;
}

void GoofyParticle::moveWithNoise(GoofyPerlinNoise &goofyPerlinNoise)
{
    float theta = ofMap(goofyPerlinNoise.getValue(position.x, position.y),0,1,0, 2 * TWO_PI);
    ofVec2f newForce = ofVec2f(cos(theta),sin(theta));
    addForce(newForce);
}

void GoofyParticle::follow(GoofyFlowField &flow) {
    ofVec2f desired = flow.lookup(position);
    desired *= 10;
    ofVec2f steer = desired - velocity;
    steer.limit(10);
    addForce(steer);
}

void GoofyParticle::addForce(ofVec2f force)
{
    acceleration += force;    
}

void GoofyParticle::update()
{
    velocity += acceleration;
    velocity.limit(limitVelocity);
    position += velocity;
    acceleration *= 0;
    if(bCheckBoundingBox)
        checkBoundingBox();
    if(lifeActive)
    {
        life--;
        if(life <= 0)
            active = false;        
    }
}

void GoofyParticle::draw()
{
    if(!active) return;
    ofPushStyle();
    ofSetColor(color);
    ofCircle(position, 1);
    ofPopStyle();
    positions.push_back(position);  // Rimuovere questo
}

void GoofyParticle::setBoundingBox(ofRectangle rect)
{
    boundingBox = rect;
}

void GoofyParticle::checkBoundingBox()
{
    if(position.x > boundingBox.x + boundingBox.width || position.x < boundingBox.x)
    {
        active = false;
    }
    if(position.y > boundingBox.y + boundingBox.height || position.y < boundingBox.y)
    {
        active = false;
    }
}

void GoofyParticle::followTarget()
{
    ofPoint distance;
	ofPoint	acceleration(0);
	distance = target - position;
	float angleDirection = atan2( distance.y, distance.x);
	ofPoint tempVector;
	float speed = 10;
    
	tempVector.x = cos(angleDirection) * speed;
	tempVector.y = sin(angleDirection) * speed;
	
    acceleration = tempVector * .16;
	
	velocity += acceleration;
	velocity *= .8;
}

void GoofyParticle::applyRepulsion(GoofyMagneticPoint repeller)
{
    // To fix
    ofPoint distCoord = this->position - repeller.position;
    if(abs(repeller.position.distance(this->position)) > repeller.radius)
        return;
    
    float dist = ofDist(repeller.position.x, repeller.position.y, this->position.x, this->position.y);
    
    float angleDirection = atan2(distCoord.y, distCoord.x);
    dist = 1 - (dist/repeller.radius);
    ofPoint rejectForce;
    rejectForce.x = repeller.force * abs(dist) * cos(angleDirection);
    rejectForce.y =  repeller.force * abs(dist) * sin(angleDirection);
    ofVec2f steer = (rejectForce - this->velocity)*.8;
    //    this->velocity -= steer;
    //this->acceleration -= steer;
    this->acceleration += rejectForce;
}

void GoofyParticle::applyAttraction(GoofyMagneticPoint repeller)
{
    // To fox
    ofPoint distCoord = this->position - repeller.position;
    if(abs(repeller.position.distance(this->position)) > repeller.radius)
        return;
    
    float dist = ofDist(repeller.position.x, repeller.position.y, this->position.x, this->position.y);
    
    dist = 1 - (dist/repeller.radius);
    float angleDirection = atan2(distCoord.y, distCoord.x);
    
    ofPoint rejectForce;
    rejectForce.x = repeller.force * abs(dist) * cos(angleDirection);
    rejectForce.y = repeller.force * abs(dist) * sin(angleDirection);
    
    this->velocity -= rejectForce;
}

void GoofyParticle::followTarget(ofPoint target)
{
    this->target = target;
    followTarget();
}