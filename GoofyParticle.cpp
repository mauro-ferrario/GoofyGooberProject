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
    id = GoofyParticle::count;
    GoofyParticle::count++;
    bFollowTarget = false;
    init(position, limitVelocity, color, spring, friction, life);
}

void GoofyParticle::init(ofVec2f position, float limitVelocity, ofColor color, float spring, float friction, long int life)
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
    this->friction = .83;
    this->spring = .183;
    this->life = life;
    size = 1;
    if(life != 0)
        lifeActive = true;
    else
        lifeActive = false;
    force.x = force.y = 0;
}

void GoofyParticle::moveWithNoise(GoofyPerlinNoise &goofyPerlinNoise)
{
    float theta = ofMap(goofyPerlinNoise.getValue(position.x, position.y),0,1,0, 2 * TWO_PI);
    ofVec2f newForce = ofVec2f(cos(theta),sin(theta)) * limitVelocity;
    force += newForce;
}


void GoofyParticle::moveWithNoise(GoofyPerlinNoise &goofyPerlinNoise, float _velocity)
{
    float theta = ofMap(goofyPerlinNoise.getValue(position.x, position.y),0,1,0, 2 * TWO_PI);
    ofVec2f newForce = ofVec2f(cos(theta),sin(theta)) * _velocity;
    force += newForce;
}

void GoofyParticle::follow(GoofyFlowField &flow) {
    ofVec2f desired = flow.lookup(position);
    desired *= limitVelocity;
    ofVec2f steer = desired - limitVelocity;
    steer.limit(limitVelocity);
    force += steer;
}

void GoofyParticle::addForce(ofVec2f _force)
{
    force += _force;
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
  ofPoint distance;
	ofPoint	acceleration(0);
	distance = target - position;
	float angleDirection = atan2( distance.y, distance.x);
	ofPoint tempVector;
  tempVector.x = cos(angleDirection) * limitVelocity;
  tempVector.y = sin(angleDirection) * limitVelocity;
  acceleration = (tempVector) * spring;
  force += acceleration;
}

void GoofyParticle::applyRepulsion(GoofyMagneticPoint* repeller)
{
//  cout << "Repeller pos = " << repeller->position << endl;
  float dist = this->position.distance(repeller->position);
  
  if(dist > repeller->radius)
    return;
  
  ofPoint distCoord = this->position -  repeller->position;
  float angleDirection = atan2(distCoord.y, distCoord.x); //repeller.position.angle(this->position);
  ofPoint rejectForce;
  rejectForce.x = (repeller->force * (abs(distCoord.x)/repeller->radius)) * cos((angleDirection));
  rejectForce.y = (repeller->force * (abs(distCoord.y)/repeller->radius)) * sin((angleDirection));
  
  if(repeller->limitSpeed)
  {
    rejectForce.x = ofClamp(rejectForce.x, -limitVelocity/4, limitVelocity/4);
    rejectForce.y = ofClamp(rejectForce.y, -limitVelocity/4, limitVelocity/4);
  }
  force += rejectForce;
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
  force -= attractionForce;
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