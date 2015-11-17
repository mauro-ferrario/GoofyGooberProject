#include "GoofyOSCController.h"


GoofyOSCController::GoofyOSCController()
{
  lastString = "";
  addListener();
  setupOSC();
  drawEnabled = true;
}

void GoofyOSCController::setupOSC()
{
  receiver.setup(6666);
}

void GoofyOSCController::addListener()
{
  ofAddListener(ofEvents().update, this, &GoofyOSCController::update);
  ofAddListener(ofEvents().draw, this, &GoofyOSCController::draw);
}

void GoofyOSCController::update(ofEventArgs &e)
{
  while(receiver.hasWaitingMessages())
  {
    ofxOscMessage m;
    receiver.getNextMessage(&m);
    string address = m.getAddress();
    if(mapToFloatValue[address] != NULL)
    {
      *mapToFloatValue[address] = m.getArgAsFloat(0) * multipliers[address];
      lastString = address;
    }
  }
}

void GoofyOSCController::draw(ofEventArgs &e)
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(0,100);
  ofRect(0, ofGetWindowHeight() - 30, ofGetWindowWidth(), 30);
  if(lastString != "")
  {
    ofSetColor(255);
    ofDrawBitmapString(lastString + " => " + ofToString(varNames[lastString]) + " = " + ofToString(*mapToFloatValue[lastString]), 10, ofGetWindowHeight() - 10);
  }
  ofPopStyle();
  ofPopMatrix();
}

void GoofyOSCController::_toggleDraw()
{
  drawEnabled = !drawEnabled;
  if(drawEnabled)
    ofAddListener(ofEvents().draw, this, &GoofyOSCController::draw);
  else
    ofRemoveListener(ofEvents().draw, this, &GoofyOSCController::draw);
    
}