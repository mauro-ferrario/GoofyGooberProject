//
//  GoofyBlobTracker.h
//  verticalWaves
//
//  Created by Mauro on 22/09/14.
//
//

#ifndef __verticalWaves__GoofyBlobTracker__
#define __verticalWaves__GoofyBlobTracker__

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"


enum inputMode
{
  INPUT_MODE_CAM,
  INPUT_MODE_MOVIE
};


class GoofyBlobTracker
{
  
public:
                            GoofyBlobTracker();
  void                      init(inputMode mode);
  void                      initContourFinder();
  void                      initTrackingColor();
  void                      initMovie(string url);
  void                      initGUI();
  void                      update();
  void                      updateContourFinder();
  void                      updateInput();
  cv::Mat                   gerROIImage();
  void                      drawROI();
  void                      draw(int x, int y, int width, int height);
  void                      draw();
  void                      drawGUI();
  void                      mousePressed(int x, int y, int button);
  void                      currentFrameChanged(int &curFrame);
  ofVideoPlayer             movie;
  ofVideoGrabber            cam;
  ofxCv::ContourFinder      contourFinder;
  ofxCv::TrackingColorMode  trackingColorMode;
  ofParameter<float>        threshold;
  ofParameter<float>        minBlobArea;
  ofParameter<float>        maxBlobArea;
  ofParameter<bool>         bFindHoles;
  ofParameter<bool>         bInvert;
  ofParameter<bool>         bSimplify;
  ofParameter<bool>         seeInput;
  ofParameter<int>          ROIx;
  ofParameter<int>          ROIy;
  ofParameter<int>          ROIwidth;
  ofParameter<int>          ROIheight;
  ofParameter<bool>         useTargetColor;
  ofParameter<int>          actualTrackingColorMode;
  ofParameter<bool>         saveBackground;
  ofParameter<int>          currentFrame;
  ofParameter<bool>         pause;
  ofxPanel                  gui;
  ofParameter<ofColor>      targetColor;
  cv::Mat                   cam_mat;
  cv::Mat                   crop;
  ofxCv::TrackingColorMode  trackingColorModes[4];
  string                    trackingColorModeNames[4];
  float                     inputWidth;
  float                     inputHeight;
};

#endif /* defined(__verticalWaves__GoofyBlobTracker__) */
