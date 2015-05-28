//
//  GoofyPerlinNoise.h
//
//  Created by xxx on 21/09/13.
//
//

#ifndef __GoofyPerlinNoise__
#define __GoofyPerlinNoise__

#include "ofMain.h"
#include "ofxGUI.h"

class GoofyPerlinNoise
{
public:
                      GoofyPerlinNoise(float resX = .01, float resY = .01, float speed = .005);
                      ~GoofyPerlinNoise();
  void                initPerlinImage(int perliImgWidth, int perlinImgHeight, int width, int height);
  float               getValue(int x, int y);
  float               getValue(int x, int y, float tempResX, float tempResY);
  void                update();
  void                draw();
  void                draw(int x, int y, int width, int height);
  void                setPerlinImageRect(ofRectangle rect);
  void                removeNoiseParams();
  ofParameterGroup*   getParameterGroup();
  ofParameterGroup*   noiseParams;
  ofParameter<float>  resX;
  ofParameter<float>  resY;
  ofParameter<float>  speed;
  ofParameter<float>  force;

  bool                drawPerlinImage;
  ofImage             perlinImg;
    
private:
  float               time;
  int                 perlinWidth;
  int                 perlinHeight;
  ofRectangle         perlinImageRect;
};

#endif