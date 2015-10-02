#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void clearFbo(ofFbo& fbo);
    void startRecording();
    void stopRecording();
    string generateTimeStamp(unsigned long long time);
    
    ofMesh quad;
    
    bool hideGui;
    ofxPanel gui;
    ofParameterGroup parameters;
    ofParameterGroup BadTV;
    ofParameter<float> thickDistort;
    ofParameter<float> fineDistort;
    ofParameter<float> distortSpeed;
    ofParameter<float> rollSpeed;
    float time;
    
    ofParameterGroup RGBShift;
    ofParameter<float> rgbAmount;
    ofParameter<float> angle;
    
    ofParameterGroup Static;
    ofParameter<float> staticAmount;
    ofParameter<float> size;
    
    ofParameterGroup Scanlines;
    ofParameter<int> count;
    ofParameter<float> sIntensity;
    ofParameter<float> nIntensity;
    
    ofVideoGrabber      vidGrabber;
    ofxVideoRecorder    vidRecorderMP4;
    ofxVideoRecorder    vidRecorderMP4Distort;

    bool bRecording;
    long long mark;
    string fileName;
    string fileExt;
    string lastFile;
    
    ofFbo badTVFbo;
    ofFbo rgbShiftFbo;
    ofFbo staticFbo;
    ofFbo filmFbo;
    
    ofShader badTvShader, rgbShiftShader, filmShader, staticShader;
};
