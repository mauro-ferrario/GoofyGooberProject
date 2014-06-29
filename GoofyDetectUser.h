//
//  GoofyDetectUser.h
//
//
//  Created by Mauro on 15/10/13.
//
//

#ifndef __Arboretum__ArboretumDetectUser__
#define __Arboretum__ArboretumDetectUser__

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxOpenCv.h"
#include "ofxOpticalFlowFarneback.h"
//#include "ofxOsc.h"

class GoofyDetectUser
{
public:
                          GoofyDetectUser();
  void                    setup(bool _useBodyShape = true, int width = 640, int height = 480);
  void                    draw();
  void                    update();
  void                    updateWithUser();
  void                    updateWithoutUser();
  void                    exit();
  void                    checkBlobPosition(ofxOpenNIUser &user);
  void                    checkBlobPosition();
  void                    drawBorderBox(int x, int y);
  void                    drawContourFinder();
  virtual void            saveImage(string newName = "");
  void                    convertTransparentPixelIntoWhitePixel(ofImage& _img);
  ofxOpenNI               openNIDevice;
  ofImage                 userImage;
  int                     width;
  int                     height;
  
  ofxCvGrayscaleImage     grayImage;
  ofxCvContourFinder      contourFinder;
  ofParameterGroup*       getParameterGroup();
  ofParameterGroup*       arboretumDetectParams;
  ofParameter<bool>       useBodyShape;
  ofParameter<bool>       activeOpenCV;
  ofParameter<bool>       activeDetetion;
  ofParameter<float>      nearThreshold;
  ofParameter<float>      farThreshold;
  ofParameter<bool>       sendImage;
  ofParameter<int>        opticalFlowResolution;
  ofParameter<bool>       activeOpticalFlow;
  ofxOpticalFlowFarneback flowSolver;
  void              initFrameBuffer(int width, int height, ofFbo& fb);
  ofFbo             changedFbo;
  ofFbo             depthStableFbo;
  ofImage           changedImage;
  ofShader            filterDepthShader;
  ofFbo               filterDepthFbo;
  
};

#endif