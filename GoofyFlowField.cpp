//
//  GoofyFlowField.cpp
//  GoofyParticleTest
//
//  Created by xxx on 22/09/13.
//
//

#include "GoofyFlowField.h"

GoofyFlowField::GoofyFlowField()
{
    resX = 0.005;
    resY = 0.005;
    drawFlowGrid = false;
    bResetFlow = false;
    changeFlowSize = 10;
}

void GoofyFlowField::init(int _width, int _height, float _resolution)
{
    this->resolution = _resolution;
    cols = _width/resolution;
    rows = _height/resolution;
    
    ofSeedRandom();
    
    float cont = ofRandom(-10000, 10000);
    float xoff = 0;
    for (int i = 0; i < cols; i++) {
        float yoff = 0;
        vector<ofVec2f> row;
        for (int j = 0; j < rows; j++) {
            float theta = ofMap(ofNoise(xoff,yoff, cont),0,1,0,2*TWO_PI);
            row.push_back(ofVec2f(cos(theta),sin(theta)));
            yoff += resY;
        }
        field.push_back(row);
        xoff += resX;
    }
}

void GoofyFlowField::resetFlow()
{
    ofSeedRandom();
    float cont = ofRandom(-10000, 10000);
    float xoff = 0;
    for (int i = 0; i < cols; i++) {
        float yoff = 0;
        for (int j = 0; j < rows; j++) {
            float theta = ofMap(ofNoise(xoff,yoff, cont),0,1,0,2*TWO_PI);
            field[i][j] = ofVec2f(cos(theta),sin(theta));
            yoff += resY;
        }
        xoff += resX;
    }
}

void GoofyFlowField::draw()
{
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            drawVector(field[i][j],i*resolution,j*resolution,resolution-2);
        }
    }
}

ofVec2f GoofyFlowField::lookup(ofVec2f lookup) {
    int column = int(ofConstrain(lookup.x/resolution,0,cols-1));
    int row = int(ofConstrain(lookup.y/resolution,0,rows-1));
    return field[column][row];
}

int GoofyFlowField::ofConstrain(int amt, int low, int high) {
    return (amt < low) ? low : ((amt > high) ? high : amt);
}

ofRectangle GoofyFlowField::changeFlowField(ofVec2f position, ofVec2f newDir)
{
  ofRectangle rectangle;
  int column = int(ofConstrain(position.x/resolution,0,cols-1));
  int row = int(ofConstrain(position.y/resolution,0,rows-1));
  rectangle.x = (column - floor(changeFlowSize/2)) * resolution;
  rectangle.y = (row - floor(changeFlowSize/2)) * resolution;
  rectangle.width = resolution * changeFlowSize;
  rectangle.height = resolution * changeFlowSize;
  
  int gridSize = changeFlowSize;
  int centerPos = floor(gridSize/2);
  
  for(int x = 0; x < gridSize; x++)
  {
    for(int y = 0; y < gridSize; y++)
    {
      ofVec2f dist;
      dist.x = x - centerPos;
      dist.y = y - centerPos;
      int columnToChange = column + dist.x;
      int rowToChange = row + dist.y;
      columnToChange = ofClamp(columnToChange, 0, cols-1);
      rowToChange = ofClamp(rowToChange, 0, rows-1);
      field[columnToChange][rowToChange] = newDir;
    }
  }
  field[column][row] = newDir;
  return rectangle;
}

void GoofyFlowField::drawVector(ofVec2f position, float x, float y, float r)
{
    float theta = ofRadToDeg(atan2(position.y, position.x));
    ofPushStyle();
    ofNoFill();
    ofPushMatrix();
    ofSetColor(255,50);
    ofRect(x, y, resolution, resolution);
    float greenValue = theta/360*255;
//    ofFill();
//    ofSetColor(0,greenValue,0);
//    ofRect(x, y, resolution, resolution);
    ofPopMatrix();
    ofPushMatrix();
    float arrowsize = resolution*.8;
    ofTranslate(x + resolution*.5,y + resolution*.5);
    ofSetLineWidth(1);
    ofRotate(theta);
    float len = position.length()*r*.6;
    ofSetColor(255,0,0,100);
    ofLine(-resolution*.5,0,len - resolution*.5,0);
    ofCircle(len - resolution*.5,0, resolution*.2*.5);
    //ofLine(len - resolution*.5,0,len - resolution*.5,+arrowsize/2);
    //ofLine(len - resolution*.5,0,len - resolution*.5,-arrowsize/4);
    ofPopMatrix();
    ofPopStyle();
}

ofParameterGroup* GoofyFlowField::getParameterGroup()
{
  if(!flowParams)
  {
    flowParams = new ofParameterGroup();
  }
  if(flowParams->getName() == "")
  {
    flowParams->setName("GoofyFlowField");
    flowParams->add(resX.set("ResX",0.01,.001,.01));
    flowParams->add(resY.set("ResY",0.01,.001,.01));
    flowParams->add(force.set("Force",0.01,.001,1));
    flowParams->add(drawFlowGrid.set("Draw flow", false));
    flowParams->add(bResetFlow.set("Reset flow", false));
//    flowParams->add(bSaveField.set("Save flow", false));
    flowParams->add(changeFlowSize.set("Change flow size",10,1,30));
//    bSaveField.addListener(this, &GoofyFlowField::saveFlowChanged);
  }
  return flowParams;
}
