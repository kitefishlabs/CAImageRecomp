//
//  kflCAImageRecompElemTimePropApp.h
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

class kflCAImageRecompElemTimePropApp : public ofBaseApp {
public:
    kflCAImageRecompElemTimePropApp();
    kflCAImageRecompElemTimePropApp(string basepath, string filename);
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
    
    ofImage targetImg, drawImg;
    unsigned char *targetPixels, *drawPixels, *currentRowPixels;
    
    int imageWidth, imageHeight, currentRow, threshold;
    string basePath, imgSourceFile;
    
//    ofxCvHaarFinder faceFinder;
    ofTrueTypeFont font;
    
    float overlayRatio;
    
};
