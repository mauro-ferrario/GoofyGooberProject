//
//  GoofyDataGraphic.cpp
//  testAccelerometro
//
//  Created by Mauro on 01/09/14.
//
//

#include "GoofyDataGraphic.h"


GoofyDataGraphic::GoofyDataGraphic()
{
  maxSize = 200;
  resetSpace();
}

void GoofyDataGraphic::setSize(int width, int height)
{
  this->width = width;
  this->height = height;
  resetSpace();
}

void GoofyDataGraphic::resetSpace()
{
  space = width/maxSize;
}


void GoofyDataGraphic::setMinAndMax(float min, float max)
{
  this->min = min;
  this->max = max;
}


float GoofyDataGraphic::getValue()
{
  return *values.end();
}

void GoofyDataGraphic::addValue(float value)
{
  values.push_back(value);
  checkSize();
}

void GoofyDataGraphic::checkSize()
{
  if(values.size() > maxSize)
  {
    values.erase(values.begin());
  }
}

void GoofyDataGraphic::setMaxSize(int maxSize)
{
  this->maxSize = maxSize;
}

void GoofyDataGraphic::update()
{
  
}

void GoofyDataGraphic::draw()
{
  draw(0, 0);
}

void GoofyDataGraphic::draw(int x, int y)
{
  ofPushMatrix();
  ofTranslate(x, y);
  ofPushStyle();
  ofSetColor(0,0,255);
  ofNoFill();
  ofRect(0, 0, width, height);
  ofPushMatrix();
  ofTranslate(0, height*.5);
  ofLine(0,0,width, 0);
  for(int i = 1; i < values.size(); i++)
  {
    float value1 = ofMap(values[i-1], min, max, -height*.5, height*.5);
    float value2 = ofMap(values[i], min, max, -height*.5, height*.5);
    ofLine((i-1) * space,value1, i*space, value2);
  }
  ofPopMatrix();
  ofPopStyle();
  ofPopMatrix();
}