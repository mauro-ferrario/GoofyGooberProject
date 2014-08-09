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
#if !defined USE_CAM
  pausedTime = 0;
#endif
}


ofParameterGroup* GoofyCamBackgroundSubstraction::getParameterGroup()
{
  if(!goofyCamBackgroundSubstractionGroup)
    goofyCamBackgroundSubstractionGroup = new ofParameterGroup();
  if(goofyCamBackgroundSubstractionGroup->getName() == "")
  {
    goofyCamBackgroundSubstractionGroup->setName("Background Substraction");
    goofyCamBackgroundSubstractionGroup->add(thresholdSensitivity.set("thresholdSensitivity", 0.0754, 0.0000, 1.0000));
    goofyCamBackgroundSubstractionGroup->add(smoothing.set("smoothing", 0.0850, 0.0000, 1.0000));
    goofyCamBackgroundSubstractionGroup->add(background.set("background", ofColor(0,0,0), ofColor(0,0,0), ofColor(255,255,255)));
#if !defined USE_CAM
    goofyCamBackgroundSubstractionGroup->add(bPause.set("pause",false));
#endif
  }
  
  return goofyCamBackgroundSubstractionGroup;
}

void GoofyCamBackgroundSubstraction::setup(int width, int height, bool useOpticalFlow)
{
#if defined USE_CAM
  cam.setDeviceID(0);
  cam.setDesiredFrameRate(60);
  cam.initGrabber(width,height);
#else
  movie.loadMovie("chromaKeyTest.mov");
  movie.play();
#endif
  this->width = width;
  this->height = height;
  this->activeOpticalFlow = useOpticalFlow;
  initFrameBuffer(width, height, backgroundFbo);
  initFrameBuffer(width, height, substractionFbo);
  substractionShader.load("backgroundSubstraciton.vert","backgroundSubstraciton.frag");
}

void GoofyCamBackgroundSubstraction::update()
{
#if defined USE_CAM
  cam.update();
  if(cam.isFrameNew())
#else
    if(bPause)
    {
      bPause = false;
      if(movie.isPlaying())
       {
         movie.stop();
         pausedTime = movie.getCurrentFrame();
       }
      else
      {
        movie.setFrame(pausedTime);
        movie.play();
      }
    }
  if(movie.isPlaying())
    movie.update();
  if(movie.isFrameNew())
#endif
  {
    if(activeOpticalFlow)
    {
      // Bisogna aggiungere il codice per usare l'optical flow
    }
    substractionShader.load("backgroundSubstraciton.vert","backgroundSubstraciton.frag");
    substractionFbo.begin();
      ofClear(0,0,0,255);
    substractionShader.begin();
    substractionShader.setUniform1f("thresholdSensitivity", thresholdSensitivity);
    substractionShader.setUniform1f("smoothing", smoothing);
    substractionShader.setUniform3f("chromakeyColor", background->r, background->g, background->b);
    substractionShader.setUniformTexture("background", backgroundFbo.getTextureReference(), 0);
    substractionShader.setUniformTexture("original", movie.getTextureReference(), 1);
    backgroundFbo.draw(0,0);
    substractionShader.end();
    substractionFbo.end();
  }
}

void GoofyCamBackgroundSubstraction::draw()
{
  ofSetColor(255);
#if defined USE_CAM
  cam.draw(0,0, width, height);
#else
  movie.draw(0,0, width, height);
#endif
  substractionFbo.draw(0,height,width,height);
}

void GoofyCamBackgroundSubstraction::setBackground()
{
  backgroundFbo.begin();
#if defined USE_CAM
  cam.draw(0,0, width, height);
#else
  movie.draw(0,0, width, height);
#endif
  backgroundFbo.end();
}

void GoofyCamBackgroundSubstraction::initFrameBuffer(int width, int height, ofFbo& fb)
{
  fb.allocate(width, height, GL_RGBA32F);
  fb.begin();
  ofClear(0, 0, 0 ,255);
  fb.end();
}