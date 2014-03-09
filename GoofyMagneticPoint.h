//
//  GoofyMagneticPoint.h
//
//  Created by Mauro on 23/09/13.
//
//

#ifndef __GoofyMagneticPoint__
#define __GoofyMagneticPoint__

#include "ofMain.h"

class GoofyMagneticPoint
{
public:
            GoofyMagneticPoint();
            GoofyMagneticPoint(ofVec2f position, float radius, float force, bool limitSpeed = true);
    
    ofVec2f position;
    float   radius;
    float   force;
    bool    limitSpeed;
};

#endif