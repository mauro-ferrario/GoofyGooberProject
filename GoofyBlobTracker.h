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
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

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
  void                      setName(string name);
  void                      initContourFinder();
  void                      initTrackingColor();
  void                      init(ofVideoPlayer* movie);
  void                      initGUI();
  void                      update();
  void                      updateContourFinder();
  cv::Mat                   gerROIImage();
  void                      drawROI();
  void                      draw(int x, int y);
  void                      draw(int x, int y, int width, int height);
  void                      draw();
  void                      drawGUI(bool showTracking);
  void                      mousePressed(ofMouseEventArgs &e);
  void                      keyReleased(ofKeyEventArgs &e);
  void                      setGUIPosition(ofPoint pos);
  void                      drawContourFinder();
  void                      initOSC(string ip, int port);
  void                      sendOSC(ofRectangle rect, int pos);
  void                      readXML();
  ofVideoPlayer*            movie;
  ofVideoGrabber            cam;
  ofxCv::ContourFinder      contourFinder;
  ofxCv::TrackingColorMode  trackingColorMode;
  ofParameter<float>        threshold;
  ofParameter<float>        minBlobArea;
  ofParameter<float>        maxBlobArea;
  ofParameter<bool>         active;
  ofParameter<bool>         bFindHoles;
  ofParameter<bool>         bInvert;
  ofParameter<bool>         bSimplify;
  ofParameter<bool>         seeInput;
  ofParameter<bool>         sortBySize;
  ofParameter<int>          ROIx;
  ofParameter<int>          ROIy;
  ofParameter<int>          ROIwidth;
  ofParameter<int>          ROIheight;
  ofParameter<bool>         useTargetColor;
  ofParameter<int>          actualTrackingColorMode;
  ofParameter<bool>         saveBackground;
  ofParameter<bool>         pause;
  ofParameter<bool>         drawShape;
  ofParameter<bool>         drawRect;
  bool                      guiVisible;
  ofxPanel                  gui;
  ofParameter<ofColor>      targetColor;
  cv::Mat                   cam_mat;
  cv::Mat                   crop;
  ofxCv::TrackingColorMode  trackingColorModes[4];
  string                    trackingColorModeNames[4];
  float                     inputWidth;
  float                     inputHeight;
  string                    name;
  ofParameter<bool>         bSendOSC;
  ofParameter<int>          maxBlobToSend;
  ofPoint                   outputPos;
  ofPoint                   scale;
  ofxOscSender              sender;
  
};


#endif /* defined(__verticalWaves__GoofyBlobTracker__) */


