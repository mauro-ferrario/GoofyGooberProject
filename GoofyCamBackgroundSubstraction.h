//
//  GoofyCamBackgroundSubstraction.h
//  LaLinea2
//
//  Created by XXX on 27/06/14.
//
//

#ifndef __LaLinea2__GoofyCamBackgroundSubstraction__
#define __LaLinea2__GoofyCamBackgroundSubstraction__

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
  ofVideoGrabber    cam;
  ofVideoPlayer 	movie;
};

#endif /* defined(__LaLinea2__GoofyCamBackgroundSubstraction__) */
