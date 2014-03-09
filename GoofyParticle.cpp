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
    init(position, limitVelocity, color, spring, friction, life);
}

void GoofyParticle::init(ofVec2f position, float limitVelocity, ofColor color, float spring, float friction, long int life)
{
  //  velocity.x = velocity.y = 0;
   // acceleration.x = acceleration.y = 0;
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
    //addForce(newForce);
}


void GoofyParticle::moveWithNoise(GoofyPerlinNoise &goofyPerlinNoise, float _velocity)
{
    float theta = ofMap(goofyPerlinNoise.getValue(position.x, position.y),0,1,0, 2 * TWO_PI);
    ofVec2f newForce = ofVec2f(cos(theta),sin(theta)) * _velocity;
    force += newForce;
    // addForce(newForce);
}

void GoofyParticle::follow(GoofyFlowField &flow) {
    ofVec2f desired = flow.lookup(position);
    desired *= limitVelocity;
    ofVec2f steer = desired - velocity;
    steer.limit(limitVelocity);
    force += steer;
    //addForce(steer);
}

void GoofyParticle::addForce(ofVec2f _force)
{
    force += _force;
}

void GoofyParticle::update()
{
    if(!active)
        return;
    //acceleration *= .19;
    //velocity += acceleration;
   // position += velocity;
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
    tempVector.x = cos(angleDirection) * abs(distance.x) * 2;// limitVelocity;
    tempVector.y = sin(angleDirection) * abs(distance.y) * 2; // limitVelocity;
    tempVector.x = ofClamp(tempVector.x, -limitVelocity, limitVelocity);
    tempVector.y = ofClamp(tempVector.y, -limitVelocity, limitVelocity);
  
    acceleration = tempVector * spring;
    acceleration *= friction;
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
  
  //    rejectForce *= friction;
  //  rejectForce *= friction;
  
  //    if(rejectForce.x > limitVelocity)
  //      rejectForce.x = limitVelocity;
  //    if(rejectForce.y > limitVelocity)
  //      rejectForce.y = limitVelocity;
  force += rejectForce;
}

void GoofyParticle::applyAttraction(GoofyMagneticPoint* repeller)
{
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
  
//    rejectForce *= friction;
  //  rejectForce *= friction;
  
//    if(rejectForce.x > limitVelocity)
//      rejectForce.x = limitVelocity;
//    if(rejectForce.y > limitVelocity)
//      rejectForce.y = limitVelocity;
    force -= rejectForce;
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