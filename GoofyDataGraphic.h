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
  float         thresholdMin;
  float         thresholdMax;
  bool          hit;
};

class GoofyDataGraphic
{
public:
            GoofyDataGraphic();
  void      setDefault();
  void      setupGraphic(string index);
  void      setSize(float width, float height);
  void      setMinAndMax(float min, float max);
  float     getValue(string index);
  void      addValue(string index, float value);
  void      checkSize(string index);
  void      setMaxSize(int maxSize);
  void      update(string index, float value);
  void      draw();
  void      draw(float x, float y);
  void      drawSingleData(string index);
  void      setColor(string index, ofColor color);
  void      setThreshold(string index, float min, float max);
  void      setHit(string index, bool hit);
  
private:
  void      resetSpace();
  void      drawBackground();
  void      drawGrid();
  void      drawText(string index, int cont);
  void      drawCharts();
  void      drawBorder();
  void      drawThreshold(string index);
  int       maxSize;
  float     width;
  float     height;
  float     min;
  float     max;
  float     space;
  tr1::unordered_map<string, SingleDataGraphicInfo > singleDataGraphic;
  
};

#endif