#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(GRABBER_WIDTH, GRABBER_HEIGHT);
    
    vidRecorder.setFfmpegLocation(ofFilePath::getAbsolutePath("ffmpeg/ffmpeg")); // use this is you have ffmpeg installed in your data folder
    
    fileName = "testMovie";
    fileExt = ".mp4"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    vidRecorder.setVideoCodec("h264");
    vidRecorder.setPixelFormat("yuv420p");
    vidRecorder.setVideoBitrate("800k");
    
    bRecording = false;
    ofEnableAlphaBlending();
    
    badTvShader.load("shaders/passthrough_vert.c", "shaders/badtv_frag.c");
    staticShader.load("shaders/passthrough_vert.c", "shaders/static_frag.c");
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    if(vidGrabber.isFrameNew() && bRecording){
        bool success = vidRecorder.addFrame(vidGrabber.getPixelsRef());
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }
    
    // Check if the video recorder encountered any error while writing video frame or audio smaples.
    if (vidRecorder.hasVideoError()) {
        ofLogWarning("The video recorder failed to write some frames!");
    }
    
    if (vidRecorder.hasAudioError()) {
        ofLogWarning("The video recorder failed to write some audio samples!");
    }
    
    if(recordedVideoPlayback.isLoaded()){
        recordedVideoPlayback.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
    
    int topMargin = (ofGetHeight() - GRABBER_HEIGHT) / 2;
    vidGrabber.draw(0,topMargin);
    
    stringstream ss;
    ss << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;
    
    ofSetColor(0,0,0,100);
    ofRect(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(),15,15);
    
    if(bRecording){
        ofSetColor(255, 0, 0);
        ofCircle(ofGetWidth() - 20, 20, 5);
    }
    
    if(recordedVideoPlayback.isLoaded()){
        recordedVideoPlayback.draw(ofGetWidth() - GRABBER_WIDTH, topMargin);
    }

}

//-------------------------------------------------------------
void ofApp::exit(){
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            fullFileName = fileName + ofGetTimestampString() + fileExt;
            vidRecorder.setup(fullFileName, GRABBER_WIDTH, GRABBER_HEIGHT, 30); // no audio
            
            // Start recording
            vidRecorder.start();
        }
        else if(!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        }
        else if(bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
    }
    if(key=='c'){
        bRecording = false;
        vidRecorder.close();
        recordedVideoPlayback.loadMovie(fullFileName);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
