#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLWindowSettings settings;
    settings.setGLVersion(4,1);
    settings.width = 1280;
    settings.height = 480;
    
    ofCreateWindow(settings);
//    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
//    ofSetupOpenGL(1280,480, OF_WINDOW);			// <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
