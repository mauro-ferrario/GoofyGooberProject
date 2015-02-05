//
//  GoofyCamBackgroundSubstraction.h
//  LaLinea2
//
//  Created by XXX on 27/06/14.
//
//

#ifndef __LaLinea2__GoofyCamBackgroundSubstraction__
#define __LaLinea2__GoofyCamBackgroundSubstraction__

//#define USA_CAM

#include "ofMain.h"

class GoofyCamBackgroundSubstraction
{
public:
                    GoofyCamBackgroundSubstraction();
  void              setup(int width, int height, bool useOpticalFlow = false);
  void              initFrameBuffer(int width, int height, ofFbo& fb);
  void              setBackground();
  void              update();
  void              draw();
  int               width;
  int               height;
  ofFbo             backgroundFbo;
  ofFbo             substractionFbo;
  ofShader          substractionShader;
  ofParameter<bool> activeOpticalFlow;
#if defined USE_CAM
  ofVideoGrabber    cam;
#else
  ofVideoPlayer 	movie;
#endif
  ofParameterGroup*  getParameterGroup();
  ofParameterGroup*  goofyCamBackgroundSubstractionGroup;
  ofParameter<float>      thresholdSensitivity;
  ofParameter<float>      smoothing;
  ofParameter<ofColor> background;
#if !defined USE_CAM
  ofParameter<bool>      bPause;
  float             pausedTime;
#endif
};

#endif /* defined(__LaLinea2__GoofyCamBackgroundSubstraction__) */
