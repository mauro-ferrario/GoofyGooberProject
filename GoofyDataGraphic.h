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

class GoofyDataGraphic
{
public:
  GoofyDataGraphic();
  void setSize(int width, int height);
  void setMinAndMax(float min, float max);
  float getValue();
  void addValue(float value);
  void  checkSize();
  void setMaxSize(int maxSize);
  void  update();
  void  draw();
  void  draw(int x, int y);
  
private:
  void resetSpace();
  int maxSize;
  int width;
  int height;
  float min;
  float max;
  float space;
  vector<float> values;
  
};

#endif /* defined(__testAccelerometro__GoofyDataGraphic__) */
