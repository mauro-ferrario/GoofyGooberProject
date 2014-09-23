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
    if(mode == INPUT_MODE_MOVIE)
      initMovie("test420p.mov");
    initTrackingColor();
    initContourFinder();
    initGUI();
}


void GoofyBlobTracker::initMovie(string url)
{
  movie.loadMovie(url);
  movie.play();
  movie.setVolume(0);
  inputWidth = movie.getWidth();
  inputHeight = movie.getHeight();
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
  openCVParams.add(threshold.set("Threshold", 0, 0, 300));
  openCVParams.add(targetColor.set("Target color", ofColor(73,153,102), ofColor(0), ofColor(255)));
  openCVParams.add(actualTrackingColorMode.set("Tracking coor mode", 0, 0, 3));
  openCVParams.add(minBlobArea.set("Min Blob Area", 10, 0, 200));
  openCVParams.add(maxBlobArea.set("Max Blob Area", 200, 10, inputWidth * inputHeight));
  openCVParams.add(bFindHoles.set("Find holes", true));
  openCVParams.add(useTargetColor.set("Use Target Color", true));
  openCVParams.add(bInvert.set("Invert", true));
  openCVParams.add(bSimplify.set("Simplify", true));
  openCVParams.add(ROIx.set("ROI x", 0, 0, 500));
  openCVParams.add(ROIy.set("ROI y", 0, 0, 5000));
  openCVParams.add(ROIwidth.set("ROI width", 100, 10, 5000));
  openCVParams.add(ROIheight.set("ROI heighy", 100, 10, 5000));
  
  ofParameterGroup movieParams;
  movieParams.setName("Movie");
  movieParams.add(currentFrame.set("Current frame", 0, 0, movie.getTotalNumFrames()));
  currentFrame.addListener(this, &GoofyBlobTracker::currentFrameChanged);
  
  gui.setup(openCVParams);
  gui.add(movieParams);
  ofParameterGroup inputParams;
  inputParams.setName("Input");
  inputParams.add(seeInput.set("See input", true));
  gui.add(inputParams);
  gui.loadFromFile("settings.xml");
}

void GoofyBlobTracker::currentFrameChanged(int & curFrame)
{
  movie.setFrame(curFrame);
}

void GoofyBlobTracker::update()
{
  updateInput();
  updateContourFinder();
}

void GoofyBlobTracker::draw(int x, int y, int width, int height)
{
  ofPushMatrix();
  ofTranslate(x,y);
  ofScale(width/inputWidth, height/inputHeight);
  draw();
  ofPopMatrix();
}

void GoofyBlobTracker::draw()
{
  if(seeInput)
    movie.draw(0,0);
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
  contourFinder.draw();
  ofPopMatrix();
  drawROI();
}

void GoofyBlobTracker::drawGUI()
{
  gui.draw();
  float newHeight = (float)(inputHeight/inputWidth) * gui.getWidth();
  draw(gui.getPosition().x, gui.getPosition().y + gui.getHeight(), gui.getWidth(), newHeight);
}

void GoofyBlobTracker::updateInput()
{
  movie.update();
}

void GoofyBlobTracker::updateContourFinder()
{
  contourFinder.setThreshold(threshold);
	contourFinder.setFindHoles(bFindHoles);
	contourFinder.setSimplify(bSimplify);
	contourFinder.setInvert(bInvert);
  contourFinder.setMinAreaRadius(minBlobArea);
  contourFinder.setMaxAreaRadius(maxBlobArea);
  if(useTargetColor)
    contourFinder.setTargetColor(targetColor, trackingColorModes[actualTrackingColorMode]);
  else
    contourFinder.useTargetColor = useTargetColor;
  contourFinder.findContours(gerROIImage());
}

cv::Mat GoofyBlobTracker::gerROIImage()
{
  cam_mat = toCv(movie);
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

void GoofyBlobTracker::mousePressed(int x, int y, int button){
  targetColor = movie.getPixelsRef().getColor(x, y);
}