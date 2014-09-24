//
//  GoofyBlobTracker.cpp
//  verticalWaves
//
//  Created by Mauro on 22/09/14.
//
//

#include "GoofyBlobTracker.h"

using namespace ofxCv;
using namespace cv;

GoofyBlobTracker::GoofyBlobTracker()
{
}

void GoofyBlobTracker::init(inputMode mode)
{
  initTrackingColor();
  initContourFinder();
  initGUI();
  ofAddListener(ofEvents().mousePressed, this, &GoofyBlobTracker::mousePressed);
  ofAddListener(ofEvents().keyReleased, this, &GoofyBlobTracker::keyReleased);
  outputPos = ofPoint(0,0);
  scale = ofPoint(1,1);
  readXML();
}

void GoofyBlobTracker::readXML()
{
  ofxXmlSettings XML;
  if(XML.loadFile("osc.xml"))
  {
    string ip = XML.getValue("osc:host","127.0.0.1");
    int port = XML.getValue("osc:port", 12346);
    initOSC(ip, port);
  }
}

void GoofyBlobTracker::setName(string name)
{
  this->name = name;
}

void GoofyBlobTracker::init(ofVideoPlayer* movie)
{
  this->movie = movie;
  inputWidth = this->movie->getWidth();
  inputHeight = this->movie->getHeight();
  init(INPUT_MODE_MOVIE);
}

void GoofyBlobTracker::initTrackingColor()
{
  actualTrackingColorMode = 0;
  trackingColorModes[0] = TRACK_COLOR_RGB;
  trackingColorModes[1] = TRACK_COLOR_HSV;
  trackingColorModes[2] = TRACK_COLOR_H;
  trackingColorModes[3] = TRACK_COLOR_HS;
  trackingColorModeNames[0] = "TRACK_COLOR_RGB";
  trackingColorModeNames[1] = "TRACK_COLOR_HSV";
  trackingColorModeNames[2] = "TRACK_COLOR_H";
  trackingColorModeNames[3] = "TRACK_COLOR_HS";
}

void GoofyBlobTracker::initContourFinder()
{
	trackingColorMode = TRACK_COLOR_RGB;
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
  contourFinder.setSimplify(true);
  contourFinder.setMinAreaRadius(100);
  contourFinder.setFindHoles(true);
  contourFinder.setInvert(true);
}

void GoofyBlobTracker::initGUI()
{
  ofParameterGroup openCVParams;
  openCVParams.setName("OpenCV");
  openCVParams.add(active.set("Active", true));
  openCVParams.add(threshold.set("Threshold", 0, 0, 300));
  openCVParams.add(targetColor.set("Target color", ofColor(73,153,102), ofColor(0), ofColor(255)));
  openCVParams.add(actualTrackingColorMode.set("Tracking color mode", 0, 0, 3));
  openCVParams.add(minBlobArea.set("Min Blob Area", 10, 0, 200));
  openCVParams.add(maxBlobArea.set("Max Blob Area", 200, 10, inputWidth * inputHeight));
  openCVParams.add(bFindHoles.set("Find holes", true));
  openCVParams.add(sortBySize.set("Sort by size", true));
  openCVParams.add(drawShape.set("Draw shape", true));
  openCVParams.add(drawRect.set("Draw rect", true));
  openCVParams.add(useTargetColor.set("Use Target Color", true));
  openCVParams.add(bInvert.set("Invert", true));
  openCVParams.add(bSimplify.set("Simplify", true));
  openCVParams.add(ROIx.set("ROI x", 0, 0, 500));
  openCVParams.add(ROIy.set("ROI y", 0, 0, 5000));
  openCVParams.add(ROIwidth.set("ROI width", 100, 10, 5000));
  openCVParams.add(ROIheight.set("ROI heighy", 100, 10, 5000));
  openCVParams.add(bSendOSC.set("Send OSC", false));
  openCVParams.add(maxBlobToSend.set("Max blob to send", 1, 0, 50));
  gui.setup(openCVParams);
  ofParameterGroup inputParams;
  inputParams.setName("Input");
  inputParams.add(seeInput.set("See input", true));
  gui.add(inputParams);
  gui.loadFromFile("settings.xml");
  guiVisible = true;
}

void GoofyBlobTracker::setGUIPosition(ofPoint pos)
{
  gui.setPosition(pos);
}

void GoofyBlobTracker::update()
{
  if(active)
    updateContourFinder();
}

void GoofyBlobTracker::draw(int x, int y)
{
  ofPushMatrix();
  ofTranslate(x,y);
  outputPos.x = x;
  outputPos.y = y;
  draw();
  ofPopMatrix();
}

void GoofyBlobTracker::draw(int x, int y, int width, int height)
{
  ofPushMatrix();
  ofTranslate(x,y);
  outputPos.x = x;
  outputPos.y = y;
  scale.x = width/inputWidth;
  scale.y = height/inputHeight;
  ofScale(scale.x, scale.y);
  draw();
  ofPopMatrix();
}

void GoofyBlobTracker::draw()
{
  if(!active)
    return;
  if(seeInput)
    movie->draw(0,0);
  else
  {
    ofPushStyle();
    ofSetColor(0);
    ofFill();
    ofRect(0,0,inputWidth, inputHeight);
    ofPopStyle();
  }
  ofPushMatrix();
  ofTranslate(ROIx, ROIy);
  drawContourFinder();
  ofPopMatrix();
  if(guiVisible)
    drawROI();
}


void GoofyBlobTracker::drawContourFinder()
{
  ofPushStyle();
  ofNoFill();
  vector<ofPolyline> polylines = contourFinder.getPolylines();
  int totPolyLines = polylines.size();
  for(int i = 0; i < totPolyLines; i++)
  {
    ofRectangle rect = toOf(contourFinder.getBoundingRect(i));
    if(drawShape)
      polylines[i].draw();
    if(drawRect)
      ofRect(rect);
    if(bSendOSC&&i<maxBlobToSend)
      sendOSC(rect, i);
  }
  ofPopStyle();
}

void GoofyBlobTracker::drawGUI(bool showTracking)
{
  if(!guiVisible)
    return;
  gui.draw();
  if(showTracking)
  {
    float newHeight = (float)(inputHeight/inputWidth) * gui.getWidth();
    draw(gui.getPosition().x, gui.getPosition().y + gui.getHeight(), gui.getWidth(), newHeight);
  }
}

void GoofyBlobTracker::updateContourFinder()
{
  contourFinder.setThreshold(threshold);
	contourFinder.setFindHoles(bFindHoles);
	contourFinder.setSimplify(bSimplify);
	contourFinder.setInvert(bInvert);
  contourFinder.setMinAreaRadius(minBlobArea);
  contourFinder.setMaxAreaRadius(maxBlobArea);
  contourFinder.setSortBySize(sortBySize);
  if(useTargetColor)
    contourFinder.setTargetColor(targetColor, trackingColorModes[actualTrackingColorMode]);
  else
    contourFinder.useTargetColor = useTargetColor;
  contourFinder.findContours(gerROIImage());
}

cv::Mat GoofyBlobTracker::gerROIImage()
{
  cam_mat = toCv(*movie);
  ROIx = ofClamp(ROIx, 1, inputWidth - 1);
  ROIy = ofClamp(ROIy, 1, 200);
  ROIwidth = ofClamp(ROIwidth, 1, inputWidth - 50 -ROIx-1);
  ROIheight = ofClamp(ROIheight, 1, inputHeight - ROIy - 1);
  cv::Rect crop_roi = cv::Rect(ROIx, ROIy, ROIwidth, ROIheight);
  crop = cam_mat(crop_roi).clone();
  return crop;
}

void GoofyBlobTracker::drawROI()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(255);
  ofFill();
  ofNoFill();
  ofSetColor(255,0,0);
  ofRect(ROIx, ROIy, ROIwidth, ROIheight);
  ofPopStyle();
  ofPopMatrix();
}

void GoofyBlobTracker::mousePressed(ofMouseEventArgs &e){
  if(e.x > outputPos.x && e.x < outputPos.x + inputWidth * scale.x)
  {
    if(e.y > outputPos.y && e.y < outputPos.y + inputHeight * scale.y)
    {
      targetColor = movie->getPixelsRef().getColor((e.x - outputPos.x) / scale.x, (e.y - outputPos.y)/scale.y);
    }
  }
}

void GoofyBlobTracker::keyReleased(ofKeyEventArgs &e){
  if(e.key == 'g')
    guiVisible = !guiVisible;
}

void GoofyBlobTracker::initOSC(string ip, int port)
{
  sender.setup(ip, port);
}

void GoofyBlobTracker::sendOSC(ofRectangle rect, int pos)
{
  ofxOscMessage message;
  message.setAddress(name+"/"+"pos/"+ofToString(pos)+"/");
  message.addFloatArg(rect.x/inputWidth);
  message.addFloatArg(rect.y/inputHeight);
  message.addFloatArg(rect.width/inputWidth);
  message.addFloatArg(rect.height/inputHeight);
  sender.sendMessage(message);
}