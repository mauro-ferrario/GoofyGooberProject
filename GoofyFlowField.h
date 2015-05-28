//
//  GoofyFlowField.h
//  Code from "Nature Of Code" by Shiffman
//
//  Created by xxx on 22/09/13.
//
//

#ifndef __GoofyFlowField__
#define __GoofyFlowField__

#include "ofMain.h"

class GoofyFlowField
{
public:
                                        GoofyFlowField();
    void                                init(int _width, int _height, float _resolution);
    void                                drawVector(ofVec2f position, float x, float y, float r);
    int                                 ofConstrain(int amt, int low, int high);
    void                                resetFlow();
    void                                draw();
    ofVec2f                             lookup(ofVec2f lookup);
    vector< vector<ofVec2f> >           field;
    vector< vector< vector<ofVec2f> > >  savedField;
    float                               cols;
    float                               rows;
    float                               resolution;
    ofParameterGroup*                   getParameterGroup();
    ofParameterGroup*                   flowParams;
    ofParameter<float>                  resX;
    ofParameter<float>                  resY;
    ofParameter<float>                  force;
    ofParameter<bool>                   drawFlowGrid;
    ofParameter<bool>                   bResetFlow;
//    ofParameter<bool>                   bSaveField;
    ofRectangle                         changeFlowField(ofVec2f position, ofVec2f newDir);
    ofParameter<int>                    changeFlowSize;
};

#endif