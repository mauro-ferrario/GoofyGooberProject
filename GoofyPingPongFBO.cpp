//
//  GoofyPingPongFBO.cpp
//  ParticlePingPongFbo
//
//  Created by XXX on 09/03/14.
//
//

#include "GoofyPingPongFBO.h"


void GoofyPingPongFBO::allocate( int _width, int _height, int _internalformat, float _dissipation)
{
  for(int i = 0; i < 2; i++){
    FBOs[i].allocate(_width,_height, _internalformat );
    FBOs[i].getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
  }
  
  clear();
  
  flag = 0;
  swap();
  flag = 0;
}

void GoofyPingPongFBO::swap()
{
  src = &(FBOs[(flag)%2]);
  dst = &(FBOs[++(flag)%2]);
}

void GoofyPingPongFBO::clear(ofColor color)
{
  for(int i = 0; i < 2; i++){
    FBOs[i].begin();
    ofClear(color);
    FBOs[i].end();
  }
}

ofFbo& GoofyPingPongFBO::operator[]( int n )
{
  return FBOs[n];
}