#include "ofMain.h"
#include "kflCAImageRecompElemVertPropApp.h"
#include "kflCAImageRecompElemTimePropApp.h"
#include "kflCAImageRecompTotalTimePropApp.h"

//========================================================================
int main( int argc, char* argv[] ){
	ofSetupOpenGL(1280, 720,OF_WINDOW);			// <-------- setup the GL context
    
    ofLogToFile("/Users/kfl/log.txt", true);
    ofLog() << ("#: " + ofToString(argc)) << endl;
    for (int i=0; i<argc; i++) {
        ofLog() <<  (": " + ofToString(argv[i])) << endl;
    }
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
    
    ofRunApp(new kflCAImageRecompElemVertPropApp("/Users/kfl/Pictures/ants/", "cate_blanchet.jpg", 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f));
//    ofRunApp(new kflCAImageRecompTotalTimePropApp("/Users/kfl/Pictures/ants/", "cate_blanchet.jpg"));

}
