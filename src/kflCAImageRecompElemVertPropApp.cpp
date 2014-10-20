//
//  kflCAImageRecompElemVertPropApp.cpp
//  kflImageCARecomp
//
//  Created by Kitefish Labs on 10/19/14.
//
//

#include "kflCAImageRecompElemVertPropApp.h"

//#define USE_BASE_IMAGE

#define THRESHOLD 64
#define OVERLAYINTERP 1.0
#define DRAWOVERLAYRATIO 1.0

#define VID

//#define RULEARRAY {0,1,1,1,1,0,0,0} // rule 30

kflCAImageRecompElemVertPropApp::kflCAImageRecompElemVertPropApp(string basepath, string filename) {
    
    kflCAImageRecompElemVertPropApp(basepath, filename, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f); // if nothing else, default to RULE 30
}

kflCAImageRecompElemVertPropApp::kflCAImageRecompElemVertPropApp(string basepath, string filename, float r0, float r1, float r2, float r3, float r4, float r5, float r6, float r7) {
    basePath = basepath;
    cout << "BASEPATH: " << basePath << endl;
    
    imgSourceFile = filename;
    cout << "IMG SOURCE FILE: " << imgSourceFile << endl;
    
    fullPath = basepath + filename;
    cout << "IMG FULL PATH: " << this->fullPath << endl;
    
    cout << (basePath+"log.txt") << endl;
    font.loadFont(OF_TTF_SANS, 13);
    
    rule[0]=r0;rule[1]=r1;rule[2]=r2;rule[3]=r3;rule[4]=r4;rule[5]=r5;rule[6]=r6;rule[7]=r7;
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::setup(){
    reset();
    currentBackground = 0;
    threshold = THRESHOLD;
    overlayRatio = OVERLAYINTERP;
    drawOverlayRatio = DRAWOVERLAYRATIO;
    ofSetFrameRate(50);
    ofEnableAlphaBlending();
    
#ifdef VID
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(1280, 720);
    vidGrabber.setPixelFormat(OF_PIXELS_MONO);
    ofSetVerticalSync(true);
#endif
}

void kflCAImageRecompElemVertPropApp::reset() {
    
    imageWidth = 1280; //targetImg.getWidth();
    imageHeight = 720; // targetImg.getHeight();

#ifdef VID
    grayTargetImg.allocate(imageWidth, imageHeight);
//    grayTargetImg.set setColor(ofColor(currentBackground));
    
    targetImg.allocate(imageWidth, imageHeight);
//    targetImg.setColor(ofColor(currentBackground));

#else
    // load a test image
    targetImg.loadImage(fullPath);
    cout << "Loading: " << fullPath << endl;
    targetImg.setImageType(OF_IMAGE_GRAYSCALE);
    
    imageWidth = targetImg.getWidth();
    imageHeight = targetImg.getHeight();
#endif
    cout << "imageWidth/Height: " << imageWidth << ", " << imageHeight << endl;
    
#ifdef USE_BASE_IMAGE
    drawImg.loadImage(fullPath);
    drawImg.setImageType(OF_IMAGE_GRAYSCALE);
#else
    // otherwise: use blank background
    drawImg.allocate(imageWidth, imageHeight);
//    drawImg.setColor(ofColor(currentBackground));
#endif
    
    cout << "targ img W/H: " << targetImg.width << ", " << targetImg.height << endl;
    cout << "draw img W/H: " << drawImg.width << ", " << drawImg.height << endl;
    
    // target pixels and draw pixels
    // - target is the image that we are looking at that should not change
    // - draw is the image that we are writing pixels on that is also the current image state
    grayTargetPixels = grayTargetImg.getPixels();
    targetPixels = targetImg.getPixels();
    drawPixels = drawImg.getPixels();
    
}
//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::update(){
    ofBackground(currentBackground);
    
    vidGrabber.update();
    if (vidGrabber.isFrameNew()) {
        cout << "get" << endl;
        targetImg.setFromPixels(vidGrabber.getPixelsRef());
        grayTargetImg = targetImg;
        grayTargetPixels = grayTargetImg.getPixels();
    }
    
    int a = 0; int b = 0; int c = 0; // left- center- and right- current-val state vars
    
    cout << drawPixels[300+100] << endl;
    
    for (int r=0; r<(imageHeight-1); r++) {
        
        int crw = (r * imageWidth); // 0th coll of curr. row
        int nrw = (r+1) * imageWidth; // 0th coll of next. row
        
        

        for (int col=0; col<(imageWidth-2); col++) {

            a = (int)((((float)grayTargetPixels[crw+col] * (1.0 - overlayRatio)) +                  ((float)drawPixels[crw+col] * overlayRatio)) < threshold)                      ?   0   :   1;
            b = (int)((((float)grayTargetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + ((float)drawPixels[crw+((col+1)%imageWidth)] * overlayRatio)) < threshold)     ?   0   :   1;
            c = (int)((((float)grayTargetPixels[crw+((col+2)%imageWidth)] * (1.0 - overlayRatio)) + ((float)drawPixels[crw+((col+2)%imageWidth)] * overlayRatio)) < threshold)     ?   0   :   1;

            assert((a >= 0) && (a < 2));
            assert((b >= 0) && (b < 2));
            assert((c >= 0) && (c < 2));
            
            switch ((a*4)+(b*2)+c) {
                case 0:
                    // vertical propagation, average TARGET and RULE
                    drawPixels[nrw+col+1] = (unsigned char)(int)(((float)(drawPixels[crw+col+1] * drawOverlayRatio) + (rule[0] * 255.f * (1.f - drawOverlayRatio))));
                    break;
                case 1:
                    drawPixels[nrw+col+1] = (unsigned char)(int)(((float)(drawPixels[crw+col+1] * drawOverlayRatio) + (rule[1] * 255.f * (1.f - drawOverlayRatio))));
                    break;
                case 2:
                    drawPixels[nrw+col+1] = (unsigned char)(int)(((float)(drawPixels[crw+col+1] * drawOverlayRatio) + (rule[2] * 255.f * (1.f - drawOverlayRatio))));
                    break;
                case 3:
                    drawPixels[nrw+col+1] = (unsigned char)(int)(((float)(drawPixels[crw+col+1] * drawOverlayRatio) + (rule[3] * 255.f * (1.f - drawOverlayRatio))));
                    break;
                case 4:
                    drawPixels[nrw+col+1] = (unsigned char)(int)(((float)(drawPixels[crw+col+1] * drawOverlayRatio) + (rule[4] * 255.f * (1.f - drawOverlayRatio))));
                    break;
                case 5:
                    drawPixels[nrw+col+1] = (unsigned char)(int)(((float)(drawPixels[crw+col+1] * drawOverlayRatio) + (rule[5] * 255.f * (1.f - drawOverlayRatio))));
                    break;
                case 6:
                    drawPixels[nrw+col+1] = (unsigned char)(int)(((float)(drawPixels[crw+col+1] * drawOverlayRatio) + (rule[6] * 255.f * (1.f - drawOverlayRatio))));
                    break;
                case 7:
                    drawPixels[nrw+col+1] = (unsigned char)(int)(((float)(drawPixels[crw+col+1] * drawOverlayRatio) + (rule[7] * 255.f * (1.f - drawOverlayRatio))));
                    break;
                default:
                    break;
            }
        }
    }
    //    currentRow = (currentRow + 1) % (imageHeight-1); // move down a row!
    
    drawImg.setFromPixels(drawPixels, imageWidth, imageHeight);
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::draw(){
    ofBackground(currentBackground);
    drawImg.draw(0, 0);
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::keyReleased(int key){
    if (key == 114) { // LOWERCASE R = Reset
        reset();
    }
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::mousePressed(int x, int y, int button){
    overlayRatio = ((float)y/(float)ofGetScreenHeight());// SET OVERLAY RATIO BASED ON HEIGHT OF MOUSE CLICK
    drawOverlayRatio = ((float)x/(float)ofGetScreenWidth());// SET DRAW OVERLAY RATIO BASED ON WIDTH OF MOUSE CLICK
}
//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemVertPropApp::dragEvent(ofDragInfo dragInfo){
    
}
