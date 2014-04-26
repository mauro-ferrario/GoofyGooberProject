//
//  GoofyDetectUser.h
//
//
//  Created by Mauro on 15/10/13.
//
//

#include "GoofyDetectUser.h"

GoofyDetectUser::GoofyDetectUser()
{
}

void GoofyDetectUser::setup(bool _useBodyShape, int width, int height)
{
  openNIDevice.setup();
  openNIDevice.setLogLevel(OF_LOG_VERBOSE);
  if(!_useBodyShape)
  {
    farThreshold  = 2000;
    nearThreshold = 200;
    openNIDevice.addDepthThreshold(nearThreshold, farThreshold);
  }
  openNIDevice.addDepthGenerator();
  openNIDevice.addImageGenerator();
  openNIDevice.setRegister(true);
  openNIDevice.setMirror(true);
  openNIDevice.addUserGenerator();
  openNIDevice.setUserSmoothing(1);
  openNIDevice.setMaxNumUsers(1);
  openNIDevice.setUseMaskTextureAllUsers(true);
  
  openNIDevice.addImageGenerator();   // comment this out
  openNIDevice.start();
  activeDetetion          = true;
  activeOpenCV            = false;
  sendImage               = false;
  useBodyShape            = _useBodyShape;
  this->width             = width;
  this->height            = height;
  grayImage.allocate(width, height);
}

ofParameterGroup* GoofyDetectUser::getParameterGroup()
{
  if(!arboretumDetectParams)
    arboretumDetectParams = new ofParameterGroup();
  if(arboretumDetectParams->getName() == "")
  {
    arboretumDetectParams->setName("Arboretum Detection");
    arboretumDetectParams->add(sendImage.set("Send Image", sendImage));
    arboretumDetectParams->add(activeDetetion.set("Active Detection", activeDetetion));
    arboretumDetectParams->add(activeOpenCV.set("Check Blob Position", true));
    if(!useBodyShape)
    {
      arboretumDetectParams->add(nearThreshold.set("Near Threshold", nearThreshold, 0, 10000));
      arboretumDetectParams->add(farThreshold.set("Far Threshold", farThreshold, 0, 10000));
    }
  }
  return arboretumDetectParams;
}

void GoofyDetectUser::update()
{
  if(!activeDetetion)
    return;
  openNIDevice.update();
  if(useBodyShape)
    updateWithUser();
  else
  {
    openNIDevice.getDepthThreshold(0).set(nearThreshold, farThreshold);
    updateWithoutUser();
  }

  if(sendImage)
  {
    sendImage = false;
    saveImage();
  }
}

void GoofyDetectUser::updateWithUser()
{
  int numUsers = openNIDevice.getNumTrackedUsers();
  for (int i = 0; i < numUsers; i++)
  {
    ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
    ofPixels pixels = user.getMaskPixels();
    
    if(user.getMaskPixels().getWidth() != 0)
    {
      userImage.setFromPixels(&pixels[0], width, height, OF_IMAGE_COLOR_ALPHA);
      userImage.update();
      if(activeOpenCV)
        checkBlobPosition(user);
    }
  }
}

void GoofyDetectUser::checkBlobPosition(ofxOpenNIUser &user)
{
  unsigned char *userImagePixels = userImage.getPixels();
  unsigned char tempImagePixels[userImage.width*userImage.height];
  int totPixels = width * height;
  for(int a = 0; a < totPixels ; a++)
  {
    if(userImagePixels[(a*4)+3] == 0)
      tempImagePixels[a] = 255;
    else
      tempImagePixels[a] = 0;
  }
  grayImage.setFromPixels(tempImagePixels, width, height);
  contourFinder.findContours(grayImage, 1000, (640*480), 10, false);
}

void GoofyDetectUser::updateWithoutUser()
{
  ofxOpenNIDepthThreshold depthThreshold = openNIDevice.getDepthThreshold(0);
  ofPixels pixels = depthThreshold.getMaskPixels();
  if(depthThreshold.getMaskPixels().getWidth() != 0)
  {
    userImage.setFromPixels(&pixels[0], width, height, OF_IMAGE_GRAYSCALE);
    userImage.update();
    if(activeOpenCV)
      checkBlobPosition();
  }
}

void GoofyDetectUser::checkBlobPosition()
{
  ofxOpenNIDepthThreshold depthThreshold = openNIDevice.getDepthThreshold(0);
  grayImage.setFromPixels(depthThreshold.getMaskPixels());
  contourFinder.findContours(grayImage, 1000, (640*480), 10, true);
}

void GoofyDetectUser::draw()
{
  ofPushStyle();
  ofSetColor(255);
  if(!activeDetetion)
    return;
  drawBorderBox(0,0);
  ofPushMatrix();
  ofTranslate(-20, -8);
  openNIDevice.drawDepth();
  if(useBodyShape)
  {
    int numUsers = openNIDevice.getNumTrackedUsers();
    for (int i = 0; i < numUsers; i++)
    {
      ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
      user.drawMask();
    }
  }
  
  ofTranslate(0,480);
  drawBorderBox(20,22);
  
  if(userImage.isAllocated())
  {
    ofRect(20,34,userImage.width-2, userImage.height-2);
    userImage.draw(20,34,userImage.width-2, userImage.height-2);
  }

  ofPushMatrix();
  ofTranslate(20,34);

  if(activeOpenCV)
      drawContourFinder();
  
  if(useBodyShape)
    openNIDevice.drawSkeletons();
  
  ofPopMatrix();
  ofPopMatrix();
  
  ofPopStyle();
}

void GoofyDetectUser::drawContourFinder()
{
  for (int i = 0; i < contourFinder.nBlobs; i++)
  {
    ofNoFill();
    contourFinder.blobs[i].draw(0,0);
    ofFill();
    ofCircle(contourFinder.blobs[i].centroid,10);
  }
}

void GoofyDetectUser::drawBorderBox(int x, int y)
{
  ofPushStyle();
  ofNoFill();
  ofSetColor(100, 100, 100);
  ofRect(x, y, 640, 500);
  ofPopStyle();
}

void GoofyDetectUser::saveImage(string newName)
{
  if(newName == "")
  {
    newName = ofToString(ofGetYear()) + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + "_" + ofToString(ofGetSeconds());
  }
  if(userImage.getPixelsRef().getNumChannels() == 4)
    convertTransparentPixelIntoWhitePixel(userImage);
  userImage.saveImage("/Volumes/Mauro/Volumes/Mauro/Users/Mauro/Desktop/"+newName+".png");
}

void GoofyDetectUser::convertTransparentPixelIntoWhitePixel(ofImage& _img)
{
  if(_img.getPixelsRef().getImageType() != OF_IMAGE_COLOR_ALPHA)
    return;
  
  unsigned char* pixels = _img.getPixels();
  int totPixel = _img.getWidth() * _img.getHeight();
  for(int a = 0; a < totPixel; a++)
  {
    if(pixels[(a*4) + 3] == 0)
    {
      pixels[a*4] = 255;
      pixels[(a*4)+1] = 255;
      pixels[(a*4)+2] = 255;
      pixels[(a*4)+3] = 255;
    }
    pixels[(a*4)+3] = 255;
  }
  _img.update();
}

void GoofyDetectUser::exit()
{
    openNIDevice.setMaxNumUsers(0);
    openNIDevice.stop();
}