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
  this->mode = mode;
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
  string ip = "127.0.0.1";
  int port = 12346;
  if(XML.loadFile("osc.xml"))
  {
    ip = XML.getValue("osc:host","127.0.0.1");
    port = XML.getValue("osc:port", 12346);
  }
  initOSC(ip, port);
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

void GoofyBlobTracker::init(ofVideoGrabber* cam)
{
  this->cam = cam;
  inputWidth = this->cam->getWidth();
  inputHeight = this->cam->getHeight();
  init(INPUT_MODE_CAM);
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
  ofParameterGroup defaultParams;
  defaultParams.setName((name));
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
  openCVParams.add(ROIx.set("ROI x", 0, 0, inputWidth));
  openCVParams.add(ROIy.set("ROI y", 0, 0, inputHeight));
  openCVParams.add(ROIwidth.set("ROI width", 100, 10, inputWidth));
  openCVParams.add(ROIheight.set("ROI heighy", 100, 10, inputHeight));
  openCVParams.add(maxBlobToCheck.set("Max blob to check", 1, 0, 50));
  openCVParams.add(blurSize.set("Blur Size", 1, 0, 50));
  openCVParams.add(drawFilled.set("Filled", true));
  
  
//  shadowParameters->setName("Shadow");
  
  openCVParams.add(smooth.set("Smooth", false));
  openCVParams.add(smoothingSize.set("SmoothingSize", 0, 0, 100));
  openCVParams.add(smoothingShape.set("smoothingShape", .3, 0, 1));
  openCVParams.add(resempleBySpacing.set("resempleBySpacing", false));
  openCVParams.add(resempledSpaceSize.set("resempledSpaceSize", 10, 0, 500));
  openCVParams.add(resempleByCount.set("resempleByCount", false));
  openCVParams.add(resempledCoundSize.set("resempledCoundSize", 10, 0, 2000));
  
  
  ofParameterGroup OSCParams;
  OSCParams.setName("Osc");
  OSCParams.add(bSendOSC.set("Send OSC", false));
  gui.setup(defaultParams);
  ofParameterGroup inputParams;
  inputParams.setName("Input");
  inputParams.add(seeInput.set("See input", true));
  gui.add(inputParams);
  gui.add(openCVParams);
  gui.add(OSCParams);
  addExtraParamsGUI();
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
  {
    drawInput();
  }
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

void GoofyBlobTracker::drawInput()
{
  switch (mode) {
    case INPUT_MODE_CAM:
      cam->draw(0,0);
      break;
    case INPUT_MODE_MOVIE:
      movie->draw(0,0);
      break;
    default:
      break;
  }
}

void GoofyBlobTracker::drawContourFinder()
{
  ofPushStyle();
  if(drawFilled)
    ofFill();
  else
    ofNoFill();
  vector<ofPolyline> polylines = contourFinder.getPolylines();
  
  ofxOscBundle bundle;
  int totPolyLines = polylines.size();
  for(int i = 0; i < totPolyLines; i++)
  {
    if(i<maxBlobToCheck)
    {
      ofRectangle rect = toOf(contourFinder.getBoundingRect(i));
      
      if(smooth)
        polylines[i] = polylines[i].getSmoothed(smoothingSize,smoothingShape);
      else if(resempleBySpacing)
        polylines[i] = polylines[i].getResampledBySpacing(resempledSpaceSize);
      else if(resempleByCount)
        polylines[i] = polylines[i].getResampledByCount(resempledCoundSize);
      
      if(drawShape)
        polylines[i].draw();
      
      if(drawRect)
        ofRect(rect);
      if(bSendOSC)
      {
        ofxOscMessage message;
        int cont = 0;

        message.setAddress(name+"_"+ofToString(cont)+"/pos/"+ofToString(i)+"/");
        int totPoints = polylines[i].size();
        
        int totMessageToSend = ceil(float(totPoints)/300);        if(totPoints < 300)
          message.addIntArg(totPoints);
        else
        {
          message.addIntArg(300);
        }
        for(int a = 0; a < totPoints; a++)
        {
          vector<ofPoint> & points = polylines[i].getVertices();
          message.addFloatArg((points[a].x + ROIx)/ROIwidth);
          message.addFloatArg((points[a].y + ROIy)/ROIheight);
          if((a%300 == 0 || a == totPoints-1) && a != 0)
          {
            bundle.addMessage(message);
            sender.sendMessage(message);
            message.clear();
            cont++;
            message.setAddress(name+"_"+ofToString(cont)+"/pos/"+ofToString(i)+"/");
            if(cont == totMessageToSend-1)
              message.addIntArg(totPoints%300);
            else
              message.addIntArg(300);
          }
          
        }
        //sendOSC(rect, i);
      }
    }
  }
  
  if(bSendOSC)
  {
//    sender.sendBundle(bundle);
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
  contourFinder.findContours(getROIImage());
}

cv::Mat GoofyBlobTracker::getROIImage()
{
//  switch (mode) {
//    case INPUT_MODE_CAM:
//      cam_mat = toCv(*cam);
//      break;
//    case INPUT_MODE_MOVIE:
//      cam_mat = toCv(*movie);
//      break;
//  }
  
  ofxCvColorImage color;
  color.setFromPixels(cam->getPixels(), inputWidth, inputHeight);
  if(blurSize > 0)
    color.blur((blurSize*2)+1);
  ROIx = ofClamp(ROIx, 1, inputWidth - 1);
  ROIy = ofClamp(ROIy, 1, 400);
  ROIwidth = ofClamp(ROIwidth, 1, inputWidth - ROIx-1);
  ROIheight = ofClamp(ROIheight, 1, inputHeight - ROIy - 1);
 // cv::Rect crop_roi = cv::Rect(ROIx, ROIy, ROIwidth, ROIheight);
  color.setROI(ofRectangle(ROIx, ROIy, ROIwidth, ROIheight));
  //crop = cam_mat(crop_roi).clone();
  cam_mat = toCv(color);
  return cam_mat;
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
  if(active&&guiVisible&&seeInput)
  {
    if(e.x > outputPos.x && e.x < outputPos.x + inputWidth * scale.x)
    {
      if(e.y > outputPos.y && e.y < outputPos.y + inputHeight * scale.y)
      {
        switch (mode) {
          case INPUT_MODE_CAM:
            targetColor = cam->getPixelsRef().getColor((e.x - outputPos.x) / scale.x, (e.y - outputPos.y)/scale.y);
            break;
          case INPUT_MODE_MOVIE:
            targetColor = movie->getPixelsRef().getColor((e.x - outputPos.x) / scale.x, (e.y - outputPos.y)/scale.y);
            break;
        }
        
      }
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
  message.addFloatArg((rect.x + ROIx)/ROIwidth);
  message.addFloatArg((rect.y + ROIy)/ROIheight);
  message.addFloatArg(rect.width/ROIwidth);
  message.addFloatArg(rect.height/ROIheight);
  sender.sendMessage(message);
}