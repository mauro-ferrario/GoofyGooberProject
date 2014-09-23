//
//  GoofyBlurGLSL.h
//  testblur
//
//  Created by Mauro on 23/09/14.
//
//

#ifndef __testblur__GoofyBlurGLSL__
#define __testblur__GoofyBlurGLSL__


#include "ofMain.h"
#include "GoofyPingPongFBO.h"


class GoofyBlurGLSL
{  
public:
  void              setup(int width, int height);
  void              update(ofFbo &fbo, ofVec2f blur);
  void              draw();
  void              draw(int x, int y);
  ofShader          blurShader;
  GoofyPingPongFBO  blurPingPong;
  ofVec2f           dimension;
};

#endif /* defined(__testblur__GoofyBlurGLSL__) */
