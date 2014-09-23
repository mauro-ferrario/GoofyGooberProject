//
//  GoofyBullSEye.h
//  testblur
//
//  Created by Mauro on 23/09/14.
//
//

#ifndef __testblur__GoofyBullSEye__
#define __testblur__GoofyBullSEye__

#include "ofMain.h"
#include "GoofyBlurGLSL.h"

class GoofyBullSEye
{
public:
  void          setup(int width, int height);
  void          update();
  void          update(float blur);
  void          draw();
  void          draw(int x, int y);
  void          initCircleEye(ofVec2f pos, float circleSize);
  void          setBlur(float blur);
  ofFbo         fbo;
  float         blur;
  GoofyBlurGLSL goofyBlur;
  int           width;
  int           height;
};

#endif /* defined(__testblur__GoofyBullSEye__) */
