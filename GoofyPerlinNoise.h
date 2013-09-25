//
//  GoofyPerlinNoise.h
//
//  Created by xxx on 21/09/13.
//
//

#ifndef __GoofyPerlinNoise__
#define __GoofyPerlinNoise__

#include "ofMain.h"

class GoofyPerlinNoise
{
public:
                    GoofyPerlinNoise(float resX = .01, float resY = .01, float speed = .005);
    void            initPerlinImage(int perliImgWidth, int perlinImgHeight, int width, int height);
    float           getValue(int x, int y);
    float           getValue(int x, int y, float tempResX, float tempResY);
    void            update();
    void            draw();
    void            setPerlinImageRect(ofRectangle rect);
    
    float           resX;
    float           resY;
    float           speed;

    bool            drawPerlinImage;
    
private:
    float           time;
    int             perlinWidth;
    int             perlinHeight;
    ofImage         perlinImg;
    ofRectangle     perlinImageRect;
};

#endif