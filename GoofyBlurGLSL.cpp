//
//  GoofyBlurGLSL.cpp
//  testblur
//
//  Created by Mauro on 23/09/14.
//
//

#include "GoofyBlurGLSL.h"



void GoofyBlurGLSL::setup(int width, int height)
{
  blurShader.load("blurShader.vert","blurShader.frag");
  blurPingPong.allocate(width, height, GL_RGBA32F_ARB);
  blurPingPong.clear();
  dimension.x = width;
  dimension.y = height;
}

void GoofyBlurGLSL::update(ofFbo &fbo, ofVec2f blur)
{
  blurPingPong.dst->begin();
  ofClear(0);
  blurShader.begin();
  blurShader.setUniform1i("direction",0);
  blurShader.setUniform2f("blurSize", blur.x, blur.y);
  blurShader.setUniform2f("size", dimension.x, dimension.y);
  fbo.draw(0,0);
  blurShader.end();
  blurPingPong.dst->end();
  blurPingPong.swap();
  
  blurPingPong.dst->begin();
  ofClear(0);
  blurShader.begin();
  blurShader.setUniform1i("direction",1);
  blurShader.setUniform2f("blurSize", blur.x, blur.y);
  blurShader.setUniform2f("size", dimension.x, dimension.y);
  blurPingPong.src->draw(0,0);
  blurShader.end();
  blurPingPong.dst->end();
  blurPingPong.swap();
}

void GoofyBlurGLSL::draw()
{
  draw(0,0);
}

void GoofyBlurGLSL::draw(int x, int y)
{
  ofPushMatrix();
  ofTranslate(x, y);
  blurPingPong.src->draw(0,0);
  ofPopMatrix();
}
