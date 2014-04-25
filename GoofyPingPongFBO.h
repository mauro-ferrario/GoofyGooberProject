//
//  GoofyPingPongFBO.h
//  ParticlePingPongFbo
//
//  Created by XXX on 09/03/14.
//

// Codice dall'esempio di oF ""gpuParticleSystemExample"

// Struct for doing PingPong quickly and easy
//
// Because on GPU you can´t write over the texture that you are reading we are
// using to pair of ofFbo attached together on what we call pingPongBuffer
// Learn more about Ping-Pong at:
//
// http://www.comp.nus.edu/~ashwinna/docs/PingPong_FBO.pdf
// http://www.seas.upenn.edu/~cis565/fbo.htm#setupgl4
//
//

#ifndef __ParticlePingPongFbo__GoofyPingPongFBO__
#define __ParticlePingPongFbo__GoofyPingPongFBO__

#include "ofMain.h"

class GoofyPingPongFBO
{
public:
  void allocate( int _width, int _height, int _internalformat = GL_RGBA, float _dissipation = 1.0f);
  void swap();
  void clear(ofColor color = ofColor(0,255));
  ofFbo& operator[]( int n );
  ofFbo   *src;       // Source       ->  Ping
  ofFbo   *dst;       // Destination  ->  Pong
private:
  ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s
  int     flag;       // Integer for making a quick swap
};

#endif /* defined(__ParticlePingPongFbo__GoofyPingPongFBO__) */
