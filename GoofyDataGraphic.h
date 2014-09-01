//
//  GoofyDataGraphic.h
//  testAccelerometro
//
//  Created by Mauro on 01/09/14.
//
//

#ifndef __testAccelerometro__GoofyDataGraphic__
#define __testAccelerometro__GoofyDataGraphic__

#include "ofMain.h"
#include<tr1/unordered_map>


class SingleDataGraphicInfo
{
public:
  vector<float> values;
  ofColor       color;
  bool          active;
};

class GoofyDataGraphic
{
public:
  GoofyDataGraphic();
  void setSize(int width, int height);
  void setMinAndMax(float min, float max);
  float getValue(string index);
  void addValue(string index, float value);
  void  checkSize(string index);
  void setMaxSize(int maxSize);
  void  update();
  void  draw();
  void  draw(int x, int y);
  void drawSingleData(string index);
  void setColor(string index, ofColor color);
  void setActive(string index, bool active);
  
private:
  void resetSpace();
  int maxSize;
  int width;
  int height;
  float min;
  float max;
  float space;
  tr1::unordered_map<string, SingleDataGraphicInfo > singleDataGraphic;
  
};

#endif