//
//  GoofyParticleSystem.cpp
//  GoofyParticleTest
//
//  Created by xxx on 21/09/13.
//
//

#include "GoofyParticleSystem.h"


GoofyParticleSystem::GoofyParticleSystem()
{
    init();
}

void GoofyParticleSystem::init()
{
    moveNoise = false;
    followFlow = false;
    bFollowTarget = true;
    percParticleSpeed = 1;
    goofyPerlinNoiseForce = 1;
}

ofParameterGroup* GoofyParticleSystem::getParameterGroup()
{
  if(!goofyParticleSystemParams)
  {
    goofyParticleSystemParams = new ofParameterGroup();
  }
  if(goofyParticleSystemParams->getName() == "")
  {
    goofyParticleSystemParams->setName("GoofyParticleSystem");
    goofyParticleSystemParams->add(percParticleSpeed.set("Particlespeed", 1, 0, 10));
    goofyParticleSystemParams->add(goofyPerlinNoiseForce.set("Goofyperlinforce", 1, 0, 10));
    percParticleSpeed.addListener(this, &GoofyParticleSystem::changeVelocityFromOfParams);
  }
  return goofyParticleSystemParams;
}

void GoofyParticleSystem::initGoofyNoise()
{
    goofyPerlinNoise.initPerlinImage(ofGetWindowWidth()/4, ofGetWindowHeight()/4, ofGetWindowWidth(), ofGetWindowHeight());
}

void GoofyParticleSystem::initGoofyNoise(GoofyPerlinNoise &externalGoofyPerlinNoise)
{
    goofyPerlinNoise = externalGoofyPerlinNoise;
}

void GoofyParticleSystem::initGoofyFlowField()
{
    goofyFlowField.init(ofGetWindowWidth(), ofGetWindowHeight(), 20);
}

void GoofyParticleSystem::initGoofyFlowField(GoofyFlowField &externaGoofyFlowField)
{
    goofyFlowField = externaGoofyFlowField;
}

void GoofyParticleSystem::addParticle(ofVec2f newPosition)
{
    addParticle(newPosition, ofRandom(8,10),0);
}

void GoofyParticleSystem::addParticle(ofVec2f newPosition, float maxVelocity, long int life)
{
    GoofyParticle* particle = new GoofyParticle(newPosition, maxVelocity * percParticleSpeed);
    particle->position = newPosition;
    particle->target = newPosition;
    particle->setBoundingBox(boundingBox);
    if(life != 0)
    {
        particle->life = life;
        particle->lifeActive = true;
    }
    particles.push_back(particle);
}

void GoofyParticleSystem::draw()
{
    vector<GoofyParticle*>::iterator vItr = particles.begin();
    while ( vItr != particles.end() )
    {
        if((*vItr)->active)
        {
            (*vItr)->draw();
        }
        vItr++;
    }

    if(goofyPerlinNoise.drawPerlinImage && moveNoise)
    {
        goofyPerlinNoise.draw();
    }
    if(goofyFlowField.drawFlowGrid && followFlow)
    {
        goofyFlowField.draw();
    }
}

void GoofyParticleSystem::updateAndDraw()
{
    if(moveNoise)
        goofyPerlinNoise.update();

    vector<GoofyParticle*>::iterator vItr = particles.begin();
    while ( vItr != particles.end() )
    {
//        if (!(*vItr)->active)
//        {
//            delete * vItr;
//            vItr = particles.erase( vItr );
//            break;
//        }
//        else
//        {
        //    (*vItr)->followTarget(ofPoint(ofGetMouseX(),ofGetMouseY()));

            if(bFollowTarget)
                (*vItr)->followTarget();
           if(moveNoise)
                (*vItr)->moveWithNoise(goofyPerlinNoise);;
            if(followFlow)
                (*vItr)->follow(goofyFlowField);
            applyRepulsions((*vItr));
            applyAttraction((*vItr));
            (*vItr)->update();
            lastActionInsideUpdateLoop((*vItr));
            (*vItr)->draw();
            vItr++;
    //    }
    }
    removeNonActiveParticles();
}

void GoofyParticleSystem::removeLastRepeller()
{
  GoofyMagneticPoint* tempRepeller = repellers.back();
  repellers.pop_back();
  tempRepeller = NULL;
  delete tempRepeller;
}

void GoofyParticleSystem::lastActionInsideUpdateLoop(GoofyParticle* particle)
{

}

void GoofyParticleSystem::update()
{
    if(moveNoise)
        goofyPerlinNoise.update();
    vector<GoofyParticle*>::iterator vItr = particles.begin();
    while ( vItr != particles.end() )
    {
//        if (!(*vItr)->active)
//        {
//            vItr = particles.erase( vItr );
//            break;
//        }
//        else
//        {
    //        (*vItr)->followTarget();

        if(bFollowTarget)
             (*vItr)->followTarget();
       // if(moveNoise)
       //     (*vItr)->moveWithNoise(goofyPerlinNoise);;
        if(followFlow)
            (*vItr)->follow(goofyFlowField);

        applyRepulsions((*vItr));
        applyAttraction((*vItr));
        (*vItr)->update();
        lastActionInsideUpdateLoop((*vItr));
        vItr++;
//        }
    }
    removeNonActiveParticles();
}

void GoofyParticleSystem::addRepeller(GoofyMagneticPoint* repeller)
{
    repellers.push_back(repeller);
}

void GoofyParticleSystem::addAttractor(GoofyMagneticPoint* attractor)
{
    attractors.push_back(attractor);
}

void GoofyParticleSystem::applyRepulsions(GoofyParticle* particle)
{
    vector<GoofyMagneticPoint*>::iterator vItr = repellers.begin();
    while ( vItr != repellers.end() )
    {
        GoofyMagneticPoint* tempPointer = (*vItr);
        particle->applyRepulsion(tempPointer);
        vItr++;
    }
}

void GoofyParticleSystem::applyAttraction(GoofyParticle* particle)
{
    vector<GoofyMagneticPoint*>::iterator vItr = attractors.begin();
    while ( vItr != attractors.end() )
    {
        particle->applyAttraction((*vItr));
        vItr++;
    }
}

void GoofyParticleSystem::removeNonActiveParticles()
{
    vector<GoofyParticle*>::iterator vItr = particles.begin();
    while ( vItr != particles.end() )
    {
        if (!(*vItr)->active)
        {
            delete * vItr;
            vItr = particles.erase( vItr );
            break;
        }
        else
        {
            vItr++;
        }
    }
}

void GoofyParticleSystem::setBoundingBox(ofRectangle rect)
{
    boundingBox = rect;
    vector<GoofyParticle*>::iterator vItr = particles.begin();
    while ( vItr != particles.end() )
    {
        (*vItr)->setBoundingBox(rect);
        vItr++;
    }
}

void GoofyParticleSystem::changeVelocityFromOfParams(float &perc)
{
    percParticleSpeed = perc;
    vector<GoofyParticle*>::iterator vItr = particles.begin();
    while ( vItr != particles.end() )
    {
        float newVel = (*vItr)->originalLimitVelocity * perc;
        (*vItr)->limitVelocity = newVel;
        vItr++;
    }
}

void GoofyParticleSystem::changeVelocity(float perc)
{
    percParticleSpeed = perc;
    vector<GoofyParticle*>::iterator vItr = particles.begin();
    while ( vItr != particles.end() )
    {
        float newVel = (*vItr)->originalLimitVelocity * perc;
        (*vItr)->limitVelocity = newVel;
        vItr++;
    }
}