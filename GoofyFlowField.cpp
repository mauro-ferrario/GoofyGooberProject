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

void GoofyFlowField::drawVector(ofVec2f position, float x, float y, float r)
{
    ofPushStyle();
    ofNoFill();
    ofPushMatrix();
    ofRect(x, y, resolution, resolution);
    ofPopMatrix();
    ofPushMatrix();
    float arrowsize = 4;
    ofTranslate(x + resolution*.5,y + resolution*.5);
    ofSetLineWidth(1);
    float theta = ofRadToDeg(atan2(position.y, position.x));
    ofRotate(theta);
    float len = position.length()*r;
    ofSetColor(255,0,0);
    ofLine(-resolution*.5,0,len - resolution*.5,0);
    ofLine(len - resolution*.5,0,len-arrowsize - resolution*.5,+arrowsize/2);
    ofLine(len - resolution*.5,0,len-arrowsize - resolution*.5,-arrowsize/2);
    ofPopMatrix();
    ofPopStyle();
}
