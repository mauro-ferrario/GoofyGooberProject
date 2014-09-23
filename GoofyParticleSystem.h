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
    virtual void                    init();
    virtual GoofyParticle*          addParticle(ofVec3f newPosition);
    virtual GoofyParticle*          addParticle(ofVec3f newPosition, float maxVelocity, long int life);
    virtual void                    draw();
    virtual void                    update();
    virtual void                    updateAndDraw();
    virtual void                    lastActionInsideUpdateLoop(GoofyParticle* particle);
    virtual ofParameterGroup*       getParameterGroup();
    void                            initGoofyNoise();
    void                            initGoofyNoise(GoofyPerlinNoise &externalGoofyPerlinNoise);
    void                            initGoofyFlowField();
    void                            initGoofyFlowField(GoofyFlowField &externaGoofyFlowField);
    void                            setBoundingBox(ofRectangle rect);
    void                            applyRepulsions(GoofyParticle* particle);
    void                            addRepeller(GoofyMagneticPoint* repeller);
    void                            addAttractor(GoofyMagneticPoint* attractor);
    void                            applyAttraction(GoofyParticle* particle);
    void                            changeVelocityFromOfParams(float &perc);
    void                            changeVelocity(float perc);
    void                            removeLastRepeller();
    void                            removeRepellers();
    bool                            moveNoise;
    bool                            followFlow;
    bool                            bFollowTarget;
    ofRectangle                     boundingBox;
  
    ofParameterGroup*               goofyParticleSystemParams;
    ofParameter<float>              percParticleSpeed;
    ofParameter<float>              goofyPerlinNoiseForce;

    GoofyPerlinNoise                goofyPerlinNoise;
    GoofyFlowField                  goofyFlowField;
    vector<GoofyParticle*>          particles;
    vector<GoofyMagneticPoint*>     repellers;
    vector<GoofyMagneticPoint*>     attractors;

};

#endif