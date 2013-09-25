//
//  GoofyParticleSystem.h
//
//  Created by xxx on 21/09/13.
//
//

#ifndef __GoofyParticleSystem__
#define __GoofyParticleSystem__

#include "ofMain.h"
#include "GoofyParticle.h"
#include "GoofyPerlinNoise.h"
#include "GoofyFlowField.h"
#include "GoofyMagneticPoint.h"

class GoofyParticleSystem
{
public:
                                    GoofyParticleSystem();
    virtual void                    init(uint totParticle);
    virtual void                    addParticle(ofVec2f newPosition);
    virtual void                    addParticle(ofVec2f newPosition, float maxVelocity, long int life);
    virtual void                    draw();
    virtual void                    update();
    virtual void                    updateAndDraw();
    void                            initGoofyNoise();
    void                            initGoofyNoise(GoofyPerlinNoise &externalGoofyPerlinNoise);
    void                            initGoofyFlowField();
    void                            initGoofyFlowField(GoofyFlowField &externaGoofyFlowField);
    void                            setBoundingBox(ofRectangle rect);
    void                            removeNonActiveParticles();
    void                            applyRepulsions(GoofyParticle* particle);
    void                            addRepeller(GoofyMagneticPoint repeller);
    void                            addAttractor(GoofyMagneticPoint attractor);
    void                            applyAttraction(GoofyParticle* particle);
    virtual void                    lastActionInsideUpdateLoop(GoofyParticle* particle);
    
    bool                            moveNoise;
    bool                            followFlow;
    ofRectangle                     boundingBox;
    
    GoofyPerlinNoise                goofyPerlinNoise;
    GoofyFlowField                  goofyFlowField;
    vector<GoofyParticle*>          particles;
    vector<GoofyMagneticPoint>      repellers;
    vector<GoofyMagneticPoint>      attractors;
};

#endif