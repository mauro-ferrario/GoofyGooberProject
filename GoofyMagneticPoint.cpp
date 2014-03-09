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

GoofyMagneticPoint::GoofyMagneticPoint(ofVec2f position, float radius, float force)
{
    this->position = position;
    this->radius = radius;
    this->force = force;
}