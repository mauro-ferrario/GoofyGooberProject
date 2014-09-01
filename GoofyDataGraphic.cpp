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

void GoofyDataGraphic::setColor(string index, ofColor color)
{
  singleDataGraphic[index].color = color;
}

void GoofyDataGraphic::setActive(string index, bool active)
{
  singleDataGraphic[index].active = active;
}

float GoofyDataGraphic::getValue(string index)
{
  return *singleDataGraphic[index].values.end();
}

void GoofyDataGraphic::addValue(string index, float value)
{
  singleDataGraphic[index].values.push_back(value);
  checkSize(index);
}

void GoofyDataGraphic::checkSize(string index)
{
  if(singleDataGraphic[index].values.size() > maxSize)
  {
    singleDataGraphic[index].values.erase(singleDataGraphic[index].values.begin());
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
  ofFill();
  ofSetColor(255);
  ofRect(0, 0, width, height);
  ofNoFill();
  ofPushMatrix();
  ofTranslate(0, height*.5);
  ofSetColor(0);
  ofLine(0,0,width, 0);
  for(tr1::unordered_map<string, SingleDataGraphicInfo >::iterator it = singleDataGraphic.begin(); it != singleDataGraphic.end(); ++it)
  {
    drawSingleData(it->first);
  }

  ofPopMatrix();
  ofPopStyle();
  ofPopMatrix();
}

void GoofyDataGraphic::drawSingleData(string index)
{
  ofPushStyle();
  ofSetColor(singleDataGraphic[index].color);
  for(int i = 1; i < singleDataGraphic[index].values.size(); i++)
  {
    float value1 = ofMap(singleDataGraphic[index].values[i-1], min, max, -height*.5, height*.5);
    float value2 = ofMap(singleDataGraphic[index].values[i], min, max, -height*.5, height*.5);
    ofLine((i-1) * space,value1, i*space, value2);
  }
  ofPopStyle();
}