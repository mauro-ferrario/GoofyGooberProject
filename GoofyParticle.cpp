//
//  GoofyParticle.cpp
//
//  Created by xxx on 21/09/13.
//
//

#include "GoofyParticle.h"

uint GoofyParticle::count = 0;

GoofyParticle::GoofyParticle()
{
    id = GoofyParticle::count;
    GoofyParticle::count++;
}

void GoofyParticle::init(ofVec3f position, float limitVelocity, ofColor color, float spring, float friction, long int life)
{
    this->position = position;
    this->limitVelocity = limitVelocity;
    this->originalLimitVelocity = limitVelocity;
    color = color;
    active = true;
    setBoundingBox(ofRectangle(0,0, ofGetWindowWidth(), ofGetWindowHeight()));
    bCheckBoundingBox = false;
    this->spring =  spring;
    this->friction = friction;
//    this->friction = .83;
//    this->spring = .183;
    this->life = life;
  cout << "LIFE = " << this->life << endl;
    size = 1;
    if(life != 0)
        lifeActive = true;
    else
        lifeActive = false;
    force.x = force.y = 0;
    pointerToTarget = &target;
}

void GoofyParticle::moveWithNoise(GoofyPerlinNoise &goofyPerlinNoise)
{
    float theta = ofMap(goofyPerlinNoise.getValue(position.x, position.y),0,1,0, 2 * TWO_PI);
    ofVec3f newForce = ofVec3f(cos(theta),sin(theta)) * limitVelocity;
    addForce(newForce);
}


void GoofyParticle::moveWithNoise(GoofyPerlinNoise &goofyPerlinNoise, float _velocity)
{
    float theta = ofMap(goofyPerlinNoise.getValue(position.x, position.y),0,1,0, 2 * TWO_PI);
    ofVec3f newForce = ofVec3f(cos(theta),sin(theta),0 ) * _velocity;
    addForce(newForce);
}

void GoofyParticle::follow(GoofyFlowField &flow) {
    ofVec3f desired = flow.lookup(position);
    desired *= flow.force * limitVelocity ; //ofGetMouseX()/float(ofGetWindowWidth())*10 ;//limitVelocity;
    addForce(desired);
}

void GoofyParticle::addForce(ofVec3f _force)
{
    force += _force;
}

void GoofyParticle::addWind(ofVec3f _force)
{
  addForce(_force);
}

void GoofyParticle::update()
{
    if(!active)
        return;
    position += force;
    force *= friction;
    if(bCheckBoundingBox)
        checkBoundingBox();
    if(lifeActive)
    {
        life--;
        if(life <= 0 && active)
        {
            active = false;
        }
    }
}

void GoofyParticle::draw()
{
    if(!active) return;
    ofPushStyle();
    ofSetColor(color);
    ofCircle(position, size);
    ofPopStyle();
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
  if(target.x == NULL)
    return;
    ofPoint distance;
	ofPoint	acceleration(0);
	distance = target - position;
	float angleDirection = atan2( distance.y, distance.x);
	ofPoint tempVector;
    tempVector.x = cos(angleDirection) * abs(distance.x) * 2;// limitVelocity;
    tempVector.y = sin(angleDirection) * abs(distance.y) * 2; // limitVelocity;
    tempVector.x = ofClamp(tempVector.x, -limitVelocity, limitVelocity);
    tempVector.y = ofClamp(tempVector.y, -limitVelocity, limitVelocity);
    acceleration = tempVector * spring;
    acceleration *= friction;
    force += acceleration;
}

void GoofyParticle::applyRepulsion(GoofyMagneticPoint* repeller, bool dependFromDistance)
{
  float dist = this->position.distance(repeller->position);
  
  if(dist > repeller->radius)
    return;
  
  ofPoint distCoord = this->position -  repeller->position;
  float angleDirection = atan2(distCoord.y, distCoord.x); //repeller.position.angle(this->position);
  ofPoint rejectForce;
  if(dependFromDistance)
  {
    rejectForce.x = (repeller->force * (abs(distCoord.x)/repeller->radius)) * cos((angleDirection));
    rejectForce.y = (repeller->force * (abs(distCoord.y)/repeller->radius)) * sin((angleDirection));
  }
  else
  {
    rejectForce.x = (repeller->force * cos(angleDirection));
    rejectForce.y = (repeller->force * sin(angleDirection));
  }
  
  if(repeller->limitSpeed)
  {
    rejectForce.x = ofClamp(rejectForce.x, -limitVelocity/4, limitVelocity/4);
    rejectForce.y = ofClamp(rejectForce.y, -limitVelocity/4, limitVelocity/4);
  }
  addForce(rejectForce);
  repeller = NULL;
}

void GoofyParticle::applyAttraction(GoofyMagneticPoint* attractor)
{
  float dist = this->position.distance(attractor->position);
  
  if(dist > attractor->radius)
      return;
  
  ofPoint distCoord = this->position -  attractor->position;
  float angleDirection = atan2(distCoord.y, distCoord.x); //repeller.position.angle(this->position);
  ofPoint attractionForce;
  attractionForce.x = (attractor->force * (abs(distCoord.x)/attractor->radius)) * cos((angleDirection));
  attractionForce.y = (attractor->force * (abs(distCoord.y)/attractor->radius)) * sin((angleDirection));
  attractionForce /= 10;
  addForce(-attractionForce);
  attractor = NULL;
}

void GoofyParticle::followTarget(ofPoint target)
{
    this->target = target;
    followTarget();
}

void GoofyParticle::setTarget(ofPoint newTarget)
{
    this->target = newTarget;
}