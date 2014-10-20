//
//  kflCAImageRecompElemTimePropApp.cpp
//  kflImageCARecomp
//
//  Created by Kitefish Labs on 10/19/14.
//
//

#include "kflCAImageRecompElemTimePropApp.h"

#define ROOTDIR "/Users/kfl/Pictures/ants/"
#define USE_BASE_IMAGE 0
#define USE_THRESHOLD 0

kflCAImageRecompElemTimePropApp::kflCAImageRecompElemTimePropApp() {
    
    kflCAImageRecompElemTimePropApp(ROOTDIR, "cate_blanchet.jpg");
}

kflCAImageRecompElemTimePropApp::kflCAImageRecompElemTimePropApp(string basepath, string filename) {
    basePath = basepath;
    cout << "BASEPATH: " << basePath << endl;
    
    imgSourceFile = filename;
    cout << "IMG SOURCE FILE: " << imgSourceFile << endl;
    
    cout << (basePath+"log.txt") << endl;
    font.loadFont(OF_TTF_SANS, 13);
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::setup(){
    reset();
    currentBackground = 255;
    threshold = 64;
    overlayRatio = 1.f;
    ofSetFrameRate(50);
    ofEnableAlphaBlending();
}

void kflCAImageRecompElemTimePropApp::reset() {
    
    // load a test image
    targetImg.loadImage(basePath+imgSourceFile);
    cout << "Loading: " << (basePath+imgSourceFile) << endl;
    targetImg.setImageType(OF_IMAGE_GRAYSCALE);
    
    imageWidth = targetImg.getWidth();
    imageHeight = targetImg.getHeight();

    cout << "imageWidth/Height: " << imageWidth << ", " << imageHeight << endl;
    
#ifdef USE_BASE_IMAGE
    drawImg.loadImage(basePath+imgSourceFile);
    drawImg.setImageType(OF_IMAGE_GRAYSCALE);
#else
    // otherwise: use blank background
    drawImg.allocate(imageWidth, imageHeight, OF_IMAGE_GRAYSCALE);
    drawImg.setColor(ofColor(currentBackground));
#endif
    
    cout << "targ img W/H: " << targetImg.width << ", " << targetImg.height << endl;
    cout << "draw img W/H: " << drawImg.width << ", " << drawImg.height << endl;

    // target pixels and draw pixels
    // - target is the image that we are looking at that should not change
    // - draw is the image that we are writing pixels on that is also the current image state
    targetPixels = targetImg.getPixels();
    drawPixels = drawImg.getPixels();
}
//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::update(){
    ofBackground(currentBackground);
    
    int a = 0; int b = 0; int c = 0; // left- center- and right- current-val state vars
    
    for (int r=0; r<(imageHeight-1); r++) {
        
        int crw = (r * imageWidth); // 0th coll of curr. row
//        int nrw = (r+1) * imageWidth; // 0th coll of next. row
        
        for (int col=0; col<(imageWidth-2); col++) {
            
#ifdef USE_THRESHOLD
            a = (int)((((float)targetPixels[crw+col] * (1.0 - overlayRatio)) +                  ((float)drawPixels[crw+col] * overlayRatio)) < threshold)                      ?   0   :   1;
            b = (int)((((float)targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + ((float)drawPixels[crw+((col+1)%imageWidth)] * overlayRatio)) < threshold)     ?   0   :   1;
            c = (int)((((float)targetPixels[crw+((col+2)%imageWidth)] * (1.0 - overlayRatio)) + ((float)drawPixels[crw+((col+2)%imageWidth)] * overlayRatio)) < threshold)     ?   0   :   1;
#else
            a = (int)(((float)targetPixels[crw+col] * (1.0 - overlayRatio)) + ((float)drawPixels[crw+col] * overlayRatio));
            b = (int)(((float)targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + ((float)drawPixels[crw+((col+1)%imageWidth)] * overlayRatio));
            c = (int)(((float)targetPixels[crw+((col+2)%imageWidth)] * (1.0 - overlayRatio)) + ((float)drawPixels[crw+((col+2)%imageWidth)] * overlayRatio));
#endif
            
            switch ((a*4)+(b*2)+c) {
                case 0:
                    // vertical propagation of rule 30
                    //drawPixels[nrw+i+1] = (unsigned char)(int)(float)(targetPixels[crw+((i+1)%imageWidth)] * overlayRatio);
                    // temporal propagation of rule 30
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)(targetPixels[crw+((col+1)%imageWidth)] * overlayRatio);
                    break;
                case 1:
                    //drawPixels[nrw+i+1] = (unsigned char)(int)(float)((targetPixels[crw+((i+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[nrw+((i+1)%imageWidth)] * overlayRatio));
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[crw+((col+1)%imageWidth)] * overlayRatio));
                    break;
                case 2:
                    // drawPixels[nrw+i+1] = (unsigned char)(int)(float)((targetPixels[crw+((i+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[nrw+((i+1)%imageWidth)] * overlayRatio));
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[crw+((col+1)%imageWidth)] * overlayRatio));
                    break;
                case 3:
                    // drawPixels[nrw+i+1] = (unsigned char)(int)(float)(targetPixels[crw+((i+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[nrw+((i+1)%imageWidth)] * overlayRatio));
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[crw+((col+1)%imageWidth)] * overlayRatio));
                    break;
                case 4:
                    // drawPixels[nrw+i+1] = (unsigned char)(int)(float)((targetPixels[crw+((i+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[nrw+((i+1)%imageWidth)] * overlayRatio));
                    drawPixels[crw+col+1] = (unsigned char)(int)(float)((targetPixels[crw+((col+1)%imageWidth)] * (1.0 - overlayRatio)) + (drawPixels[crw+((col+1)%imageWidth)] * overlayRatio));
                    break;
                case 5:
                    // drawPixels[nrw+i+1] = (unsigned char)(int)(float)(targetPixels[crw+((i+1)%imageWidth)] * overlayRatio);
                    drawPixels[crw+c+1] = (unsigned char)(int)(float)(targetPixels[crw+((col+1)%imageWidth)] * overlayRatio);
                    break;
                case 6:
                    // drawPixels[nrw+i+1] = (unsigned char)(int)(float)(targetPixels[crw+((i+1)%imageWidth)] * overlayRatio);
                    drawPixels[crw+c+1] = (unsigned char)(int)(float)(targetPixels[crw+((col+1)%imageWidth)] * overlayRatio);
                    break;
                    // case 7:
//                    drawPixels[nrw+c+1] = (unsigned char)(int)(float)(targetPixels[crw+((i+1)%imageWidth)] * overlayRatio);
                default:
                    break;
            }
        }
    }
    //    currentRow = (currentRow + 1) % (imageHeight-1); // move down a row!
    
    drawImg.setFromPixels(drawPixels, imageWidth, imageHeight, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::draw(){
    ofBackground(currentBackground);
    drawImg.draw(0, 0);
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::keyReleased(int key){
    if (key == 114) { // LOWERCASE R = Reset
        reset();
    }
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::mousePressed(int x, int y, int button){
    overlayRatio = ((float)y/(float)ofGetScreenHeight());// SET OVERLAY RATIO BASED ON HEIGHT OF MOUSE CLICK
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void kflCAImageRecompElemTimePropApp::dragEvent(ofDragInfo dragInfo){
    
}
