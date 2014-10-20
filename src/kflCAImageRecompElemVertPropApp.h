//
//  kflCAImageRecompElemVertPropApp.h
//  kflImageCARecomp
//
//  Created by Kitefish Labs on 10/19/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"
#include <vector>

class kflCAImageRecompElemVertPropApp : public ofBaseApp {
public:
    kflCAImageRecompElemVertPropApp(string basepath, string filename);
    kflCAImageRecompElemVertPropApp(string basepath, string filename, float r0, float r1, float r2, float r3, float r4, float r5, float r6, float r7);
    void setup();
    void reset();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int currentBackground;
    
    ofVideoGrabber vidGrabber;
    
    ofxCvColorImage targetImg;
    ofxCvGrayscaleImage drawImg, grayTargetImg;
    unsigned char *targetPixels, *drawPixels, *grayTargetPixels;
    
    int imageWidth, imageHeight, currentRow, threshold;
    string basePath, imgSourceFile, fullPath;
    
    ofTrueTypeFont font;
    
    float overlayRatio, drawOverlayRatio;
    
    float rule[8];
    
};
