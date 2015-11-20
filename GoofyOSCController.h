//
//  GoofyOSCControllerUtils.h
//  goofyLeapImageGallery
//
//  Created by Mauro Ferrario on 29/10/15.
//
//

#ifndef __goofyLeapImageGallery__GoofyOSCControllerUtils__
#define __goofyLeapImageGallery__GoofyOSCControllerUtils__

#include "ofMain.h"
#include "ofxOsc.h"
#include<tr1/unordered_map>





class GoofyOSCController
{
public:
  static GoofyOSCController& getInstance()
  {
    static GoofyOSCController instance;
    return instance;
  }
  
  static void addMapValue(float& value, float multiplier = 1)
  {
    int posToFill = GoofyOSCController::getInstance().posToFill++;
    addMapValue(GoofyOSCController::getInstance().korgNanoController2[posToFill], GoofyOSCController::getInstance().korgNanoController2[posToFill], value, multiplier);
  }
  
  static void addMapValue(string varName, float& value, float multiplier = 1)
  {
    int posToFill = GoofyOSCController::getInstance().posToFill++;
    addMapValue(posToFill, varName, value, multiplier);
  }
  
  static void addMapValue(int posToFill, string varName, float& value, float multiplier = 1)
  {
    string address = GoofyOSCController::getInstance().korgNanoController2[posToFill];
    addMapValue(varName, address, value, multiplier);
  }
  
  static void addMapValue(string varName, string mapString, float& value, float multiplier = 1)
  {
    GoofyOSCController::getInstance().mapToFloatValue[mapString] = &value;
    GoofyOSCController::getInstance().multipliers[mapString] = multiplier;
    GoofyOSCController::getInstance().varNames[mapString] = varName;
  }
  
  static void toggleDraw()
  {
    GoofyOSCController::getInstance()._toggleDraw();
  }
  
private:
  
  GoofyOSCController(GoofyOSCController const&);
  void                                operator=(GoofyOSCController const&);
  GoofyOSCController();
  void                                addListener();
  void                                update(ofEventArgs &e);
  void                                draw(ofEventArgs &e);
  ofxOscReceiver                      receiver;
  void                                setupOSC();
  tr1::unordered_map<string, float* > mapToFloatValue;
  tr1::unordered_map<string, float >  multipliers;
  tr1::unordered_map<string, string > varNames;
  string                              lastString;
  int                                 posToFill;
  bool                                drawEnabled;
  void                                _toggleDraw();
  
  const char *korgNanoController2[16] =
  {
    "/midi/cc0/1",
    "/midi/cc1/1",
    "/midi/cc2/1",
    "/midi/cc3/1",
    "/midi/cc4/1",
    "/midi/cc5/1",
    "/midi/cc6/1",
    "/midi/cc7/1"
    "/midi/cc16/1",
    "/midi/cc17/1",
    "/midi/cc18/1",
    "/midi/cc19/1",
    "/midi/cc20/1",
    "/midi/cc21/1",
    "/midi/cc22/1",
    "/midi/cc23/1"
  };
};

#endif /* defined(__goofyLeapImageGallery__GoofyOSCControllerUtils__) */
