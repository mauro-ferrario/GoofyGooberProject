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
    applyWind             = true;
    wind                  = ofVec2f(0,10);
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
    goofyParticleSystemParams->add(goofyPerlinNoiseForce.set("Goofyperlinforce", 1, 0, 1));
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

GoofyParticle* GoofyParticleSystem::addParticle(ofVec3f newPosition)
{
    return addParticle(newPosition, ofRandom(8,10),0);
}

GoofyParticle* GoofyParticleSystem::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  GoofyParticle* particle = new GoofyParticle();
  particle->init(newPosition, maxVelocity * percParticleSpeed);
  particle->target = newPosition;
  particle->setBoundingBox(boundingBox);
  particles.push_back(particle);
  return particle;
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
   // if(moveNoise)
     //   goofyPerlinNoise.update();

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
                (*vItr)->moveWithNoise(goofyPerlinNoise, goofyPerlinNoiseForce * 20);;
            if(followFlow)
                (*vItr)->follow(goofyFlowField);
            applyRepulsions((*vItr));
            applyAttraction((*vItr));
      
      
      
      
            // Ciclo per il contollo dello stormo. Se gli element sono troppo vicini, si allontanano, se sono troppo lontanti fra di loro,
            // si avvicinano
            vector<GoofyParticle*>::iterator vItr2 = vItr;
    
            vItr2++;
//            while ( vItr2 != particles.end() )
//            {
//              ofVec3f dir = (*vItr)->position - (*vItr2)->position;
//              float distSqrd = dir.lengthSquared();
//              float zoneRadiusSqrd = 10000;
//              float percent = distSqrd/zoneRadiusSqrd;
//              if( distSqrd <= zoneRadiusSqrd ) {   // SEPARATION
//                float F = ( zoneRadiusSqrd/distSqrd - 1.0f ) * 0.01f;
//                dir = dir.normalized() * F;
//                (*vItr)->addForce(dir);
//                (*vItr2)->addForce(-dir);
//              }
//              else { // ... else attract
//                float thresh = .184;
//                float threshDelta = 1.0f - thresh;
//                float adjustedPercent = ( percent - thresh )/threshDelta;
//                float F = ( 1.0 - ( cos( adjustedPercent * M_PI*2.0f ) * -0.5f + 0.5f ) ) * 0.04f;
//                dir = dir.normalized() * F;
//                (*vItr)->addForce(-dir);
//                (*vItr2)->addForce(dir);
//              }
//              vItr2++;
//            }
//      
      
            ofPoint center(ofGetWidth()*.5, ofGetHeight()*.5);
            ofPoint distance = (*vItr)->position - center;
      
            // Questo serve per fare in modo che le particelle non si muovino tropo dal centro
      
//            if(distance.length() > 300)
//            {
//              ofVec3f centerForce;
//              centerForce = -distance.normalize() * 3;
//              (*vItr)->addForce(centerForce);
//            }
      
      
            (*vItr)->update();
            lastActionInsideUpdateLoop((*vItr));
      
            if (!(*vItr)->active)
            {
              delete * vItr;
              vItr = particles.erase( vItr );
            }
            else
            {
              (*vItr)->draw();
              vItr++;
            }
    //    }
    }
}

void GoofyParticleSystem::removeRepellers()
{
  vector<GoofyMagneticPoint*>::iterator vItr = repellers.begin();
  while ( vItr != repellers.end() )
  {
    GoofyMagneticPoint* tempPointer = (*vItr);
    repellers.pop_back();
    tempPointer = NULL;
    delete tempPointer;
  }
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
      
        if (!(*vItr)->active)
        {
          delete * vItr;
          vItr = particles.erase( vItr );
        }
        else
        {
          vItr++;
        }
    }
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