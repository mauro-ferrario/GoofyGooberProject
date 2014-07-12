//
//  GoofyCamBackgroundSubstraction.cpp
//  LaLinea2
//
//  Created by XXX on 27/06/14.
//
//

#include "GoofyCamBackgroundSubstraction.h"



GoofyCamBackgroundSubstraction::GoofyCamBackgroundSubstraction()
{
  
}

void GoofyCamBackgroundSubstraction::setup(int width, int height, bool useOpticalFlow)
{
  cam.setDeviceID(0);
  cam.setDesiredFrameRate(60);
  cam.initGrabber(width,height);
  
  this->width = width;
  this->height = height;
  this->activeOpticalFlow = useOpticalFlow;
  initFrameBuffer(width, height, backgroundFbo);
  initFrameBuffer(width, height, substractionFbo);
  
  substractionShader.load("backgroundSubstraciton.vert","backgroundSubstraciton.frag");
}

void GoofyCamBackgroundSubstraction::update()
{
  cam.update();
  if (cam.isFrameNew())
  {
    if(activeOpticalFlow)
    {
      // Bisogna aggiungere il codice per usare l'optical flow
    }
    substractionShader.load("backgroundSubstraciton.vert","backgroundSubstraciton.frag");
    substractionFbo.begin();
      ofClear(0,0,0,255);
    substractionShader.begin();
    substractionShader.setUniformTexture("background", backgroundFbo.getTextureReference(), 0);
    substractionShader.setUniformTexture("original", cam.getTextureReference(), 1);
    backgroundFbo.draw(0,0);
    substractionShader.end();
    substractionFbo.end();
  }
}

void GoofyCamBackgroundSubstraction::draw()
{
  ofSetColor(255);
  cam.draw(0,0, width, height);
  substractionFbo.draw(0,height,width,height);
}

void GoofyCamBackgroundSubstraction::setBackground()
{
  backgroundFbo.begin();
  cam.draw(0,0, width, height);
  backgroundFbo.end();
}

void GoofyCamBackgroundSubstraction::initFrameBuffer(int width, int height, ofFbo& fb)
{
  fb.allocate(width, height, GL_RGBA32F);
  fb.begin();
  ofClear(0, 0, 0 ,255);
  fb.end();
}