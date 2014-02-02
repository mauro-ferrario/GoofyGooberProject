//
//  GoofyMagneticPoint.cpp
//
//  Created by Mauro on 23/09/13.
//
//

#include "GoofyMagneticPoint.h"


GoofyMagneticPoint::GoofyMagneticPoint()
{
    
}

GoofyMagneticPoint::GoofyMagneticPoint(ofVec3f position, float radius, float force)
{
    this->position = position;
    this->radius = radius;
    this->force = force;
}