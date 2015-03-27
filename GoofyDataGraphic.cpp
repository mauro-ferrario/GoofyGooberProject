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
  setDefault();
}

void GoofyDataGraphic::setDefault()
{
  setSize(300, 80);
  setMinAndMax(0, 1023);
}

void GoofyDataGraphic::setupGraphic(string index)
{
  setColor(index, ofColor(255,0,0));
  setThreshold(index, min, max);
  singleDataGraphic[index].hit = false;
}

void GoofyDataGraphic::setSize(float width, float height)
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

void GoofyDataGraphic::setThreshold(string index, float min, float max)
{
  singleDataGraphic[index].thresholdMin = min;
  singleDataGraphic[index].thresholdMax = max;
}

void GoofyDataGraphic::setHit(string index, bool hit)
{
  singleDataGraphic[index].hit = hit;
}

void GoofyDataGraphic::setColor(string index, ofColor color)
{
  singleDataGraphic[index].color = color;
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

void GoofyDataGraphic::update(string index, float value)
{
  addValue(index, value);
}

void GoofyDataGraphic::draw()
{
  draw(0, 0);
}


void GoofyDataGraphic::drawBackground()
{
  ofPushStyle();
  ofFill();
  ofSetColor(40);
  ofRect(0, 0, width, height);
  ofPopStyle();
}

void GoofyDataGraphic::drawGrid()
{
  ofPushStyle();
  ofSetColor(0,0,255);
  ofPushStyle();
  ofSetColor(20);
  ofSetLineWidth(1);
  int totLines = 10;
  float lineOffsetX = width/totLines;
  float lineOffsetY = height/totLines;
  for(int a = 0; a < totLines; a++)
    ofLine(0, a*lineOffsetY, width, a*lineOffsetY);
  for(int a = 0; a < totLines; a++)
    ofLine(a*lineOffsetX, 0, a*lineOffsetX, height);
  ofPopStyle();
  ofPopStyle();
}

void GoofyDataGraphic::drawText(string index, int cont)
{
  ofPushStyle();
  ofSetColor(70);
  float value =  singleDataGraphic[index].values[singleDataGraphic[index].values.size()-1];
  string text = index + " : " + ofToString(value);
  ofDrawBitmapString(text, ofPoint(10 + (40*cont), (height*.5) - 7));
  ofPopStyle();
}

void GoofyDataGraphic::drawBorder()
{
  ofPushStyle();
  ofNoFill();
  ofSetColor(120);
  ofRect(0,0,width,height);
  ofPopStyle();
}

void GoofyDataGraphic::drawCharts()
{
  ofPushMatrix();
  ofTranslate(0, height*.5);
  int cont = 0;
  for(tr1::unordered_map<string, SingleDataGraphicInfo >::iterator it = singleDataGraphic.begin(); it != singleDataGraphic.end(); ++it)
  {
    drawText(it->first, cont);
    drawSingleData(it->first);
    cont++;
  }
  ofPopMatrix();
}

void GoofyDataGraphic::draw(float x, float y)
{
  ofPushMatrix();
  ofTranslate(x, y);
  drawBackground();
  drawGrid();
  drawCharts();
  drawBorder();
  ofPopMatrix();
}

void GoofyDataGraphic::drawSingleData(string index)
{
  if(singleDataGraphic[index].thresholdMin != min || singleDataGraphic[index].thresholdMax != max)
    drawThreshold(index);
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

void GoofyDataGraphic::drawThreshold(string index)
{
  ofPushStyle();
  ofColor tempColor = singleDataGraphic[index].color;
  ofRectangle rect;
  rect.x = 0;
  rect.width = width;
  rect.y = ofMap(singleDataGraphic[index].thresholdMin, min, max, -height*.5, height*.5);
  rect.height = ofMap(singleDataGraphic[index].thresholdMax, min, max, -height*.5, height*.5) - rect.y;
  if(singleDataGraphic[index].hit)
    tempColor.a = 70;
  else
    tempColor.a = 20;
  ofSetColor(tempColor);
  ofFill();
  ofRect(rect);
  ofPopStyle();
}