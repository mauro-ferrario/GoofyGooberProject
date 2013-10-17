//
//  GoofyParticle.h
//
//  Created by xxx on 21/09/13.
//

#ifndef __GoofyParticle__
#define __GoofyParticle__

#include "ofMain.h"
#include "GoofyPerlinNoise.h"
#include "GoofyFlowField.h"
#include "GoofyMagneticPoint.h"

class GoofyParticle
{
public:
                    GoofyParticle(ofVec2f position = ofVec2f(0,0), float limitVelocity = 10, ofColor color = 255, float spring = ofRandom(.01,.5), float friction = ofRandom(.01,.4), long int life = 0);
    void            init(ofVec2f position = ofVec2f(0,0), float limitVelocity = 10, ofColor color = 255, float spring = ofRandom(.01,.5), float friction = ofRandom(.01,.4), long int life = 0);

    void            addForce(ofVec2f force);
    virtual void    update();
    virtual void    draw();
    void            moveWithNoise(GoofyPerlinNoise &goofyPerlinNoise);
    void            moveWithNoise(GoofyPerlinNoise &goofyPerlinNoise, float _velocity);
    void            follow(GoofyFlowField &flow);
    void            setBoundingBox(ofRectangle rect);
    void            checkBoundingBox();
    void            followTarget();
    void            followTarget(ofPoint target);
    void            applyRepulsion(GoofyMagneticPoint* repller);
    void            applyAttraction(GoofyMagneticPoint* repller);
    void            setTarget(ofPoint newTarget);
    
    int             life;
    ofPoint         target;
    ofRectangle     boundingBox;
    ofVec2f         acceleration;
    ofVec2f         position;
    ofVec2f         velocity;
    float           limitVelocity;
    float           originalLimitVelocity;
    float           spring;
    float           friction;
    ofColor         color;
    bool            active;
    bool            bCheckBoundingBox;
    bool            lifeActive;
    int             size;
    uint            id;
    ofPoint         force;
    static uint     count;
};

#endif