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
            GoofyMagneticPoint(ofVec3f position, float radius, float force);
    
    ofVec3f position;
    float   radius;
    float   force;
};

#endif