//
//  GoofyBullSEye.cpp
//  testblur
//
//  Created by Mauro on 23/09/14.
//
//

#include "GoofyBullSEye.h"


void GoofyBullSEye::setup(int width, int height)
{
  goofyBlur.setup(width, height);
  fbo.allocate(width, height, GL_RGBA32F_ARB);
  initCircleEye(ofVec2f(width*.5, height*.5), 150);
  this->width = width;
  this->height = height;
  blur = 10;
}

void GoofyBullSEye::setBlur(float blur)
{
  this->blur = blur;
}

void GoofyBullSEye::initCircleEye(ofVec2f pos, float circleSize)
{
  fbo.begin();
  ofClear(255,0);
  ofSetColor(255);
  ofSetCircleResolution(150);
  ofCircle(pos, circleSize);
  fbo.end();
}

void GoofyBullSEye::update()
{
  goofyBlur.update(fbo, ofVec2f(blur,blur));
}

void GoofyBullSEye::update(float blur)
{
  if(opacity == 0)
    return;
  setBlur(blur);
  update();
}

void GoofyBullSEye::draw()
{
  if(opacity == 0)
    return;
  ofPushStyle();
  ofSetColor(255, opacity);
  ofPushMatrix();
  ofTranslate(goofyBlur.dimension.x*.5, goofyBlur.dimension.y*.5);
  ofScale(scale, scale);
  ofTranslate(-goofyBlur.dimension.x*.5, -goofyBlur.dimension.y*.5);
  goofyBlur.draw(0,0);
  ofPopMatrix();
  ofPopStyle();
}

void GoofyBullSEye::draw(int x, int y)
{
  ofPushMatrix();
  ofTranslate(x,y);
  draw();
  ofPopMatrix();
}

ofParameterGroup* GoofyBullSEye::getParameterGroup()
{
  if(!params)
  {
    params = new ofParameterGroup();
  }
  if(params->getName() == "")
  {
    params->setName("Bull's Eye");
    params->add(opacity.set("Opacity", 255, 0, 255));
    params->add(blur.set("Blur", 10, 0, 50));
    params->add(scale.set("Scale", 1, 0, 5));
  }
  return params;
}