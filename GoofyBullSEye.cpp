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
  ofCircle(pos, circleSize);
  fbo.end();
}

void GoofyBullSEye::update()
{
  goofyBlur.update(fbo, ofVec2f(blur,blur));
}

void GoofyBullSEye::update(float blur)
{
  setBlur(blur);
  update();
}

void GoofyBullSEye::draw()
{
  ofPushStyle();
  ofSetColor(255);
  goofyBlur.draw(0,0);
  ofPopStyle();
}

void GoofyBullSEye::draw(int x, int y)
{
  ofPushMatrix();
  ofTranslate(x,y);
  draw();
  ofPopMatrix();
}