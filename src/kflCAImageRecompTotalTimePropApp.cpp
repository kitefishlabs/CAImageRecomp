//
//  kflCAImageRecompTotalTimePropApp.cpp
//  kflImageCARecomp
//
//  Created by Kitefish Labs on 10/19/14.
//
//

#include "kflCAImageRecompTotalTimePropApp.h"

#define ROOTDIR "/Users/kfl/Pictures/ants/"
#define USE_BASE_IMAGE 0

kflCAImageRecompTotalTimePropApp::kflCAImageRecompTotalTimePropApp() {
    
    kflCAImageRecompTotalTimePropApp(ROOTDIR, "cate_blanchet.jpg");
}

kflCAImageRecompTotalTimePropApp::kflCAImageRecompTotalTimePropApp(string basepath, string filename) {
    basePath = basepath;
    cout << "BASEPATH: " << basePath << endl;
    
    imgSourceFile = filename;
    cout << "IMG SOURCE FILE: " << imgSourceFile << endl;

    cout << (basePath+"log.txt") << endl;
    font.loadFont(OF_TTF_SANS, 13);
}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::setup(){
    reset();
    currentBackground = 255;
    threshold = 64;
    overlayRatio = 1.f;
    ofSetFrameRate(50);
    ofEnableAlphaBlending();
}

void kflCAImageRecompTotalTimePropApp::reset() {
    
    // load a test image
    targetImg.loadImage(basePath+imgSourceFile);
    cout << "Loading: " << (basePath+imgSourceFile) << endl;
    targetImg.setImageType(OF_IMAGE_GRAYSCALE);
    
    imageWidth = targetImg.getWidth();
    imageHeight = targetImg.getHeight();
    
    cout << "++ IMG height: " << imageWidth << endl;
    cout << "++ IMG Width:  " << imageHeight << endl;
    
#ifdef USE_BASE_IMAGE
    drawImg.loadImage(basePath+imgSourceFile);
    drawImg.setImageType(OF_IMAGE_GRAYSCALE);
#else
    // otherwise: use blank background
    drawImg.allocate(imageWidth, imageHeight, OF_IMAGE_GRAYSCALE);
    drawImg.setColor(ofColor(currentBackground));
#endif
    
    // target pixels and draw pixels
    // - target is the image that we are looking at that should not change
    // - draw is the image that we are writing pixels on that is also the current image state
    targetPixels = targetImg.getPixels();
    drawPixels = drawImg.getPixels();
}
//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::update(){

    int a = 0; int b = 0; int c = 0; // left- center- and right- current-val state vars
    
    // iterate over all rows per update frame
    for (int row=0; row<(imageHeight-1); row++) {

        int crw = (row * imageWidth); // 0th coll of curr. row

        // iterate over all cols per row
        for (int col=0; col<(imageWidth-2); col++) {
            
            // pixel = target interpolated with draw according to overlayRatio (0.0 = all target <---> 1.0 = all current draw)
            a = ((targetPixels[crw+col] * (1.0 - overlayRatio)) + (drawPixels[crw+col] * overlayRatio));
            b = ((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[crw+((col+1)%imageWidth)] * overlayRatio));
            c = ((targetPixels[crw+((col+2)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[crw+((col+2)%imageWidth)] * overlayRatio));
            
            switch ((a + b + c) / 132) { // 3 * (256/6) = 132, our color space is divided into 6 sections
                case 0:
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (currentTR[0] * overlayRatio));
                    break;
                case 1:
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (currentTR[1] * overlayRatio));
                    break;
                case 2:
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (currentTR[2] * overlayRatio));
                    break;
                case 3:
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (currentTR[3] * overlayRatio));
                    break;
                case 4:
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (currentTR[4] * overlayRatio));
                    break;
                case 5:
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (currentTR[5] * overlayRatio));
                    break;
                case 6:
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (currentTR[6] * overlayRatio));
                    break;
                // REMEMBER: NO CASE 7!
                default:
                    break;
            }
        }
    }
    drawImg.setFromPixels(drawPixels, imageWidth, imageHeight, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::draw(){
    ofBackground(currentBackground);
    drawImg.draw(0, 0);
}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::keyPressed(int key){

}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::keyReleased(int key){
    if (key == 114) { // LOWERCASE R = Reset
        reset();
    }
}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::mousePressed(int x, int y, int button){
    overlayRatio = ((float)y/(float)ofGetScreenHeight()); // SET OVERLAY RATIO BASED ON HEIGHT OF MOUSE CLICK
}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void kflCAImageRecompTotalTimePropApp::dragEvent(ofDragInfo dragInfo){

}
