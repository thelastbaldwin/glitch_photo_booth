#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    sampleRate = 44100;
    channels = 2;
    
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(640, 480);
    //    vidRecorder.setFfmpegLocation(ofFilePath::getAbsolutePath("ffmpeg")); // use this is you have ffmpeg installed in your data folder
    
    fileName = "testMovie";
    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    
    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    //    vidRecorder.setVideoCodec("libx264");
    vidRecorder.setVideoBitrate("800k");
    
    bRecording = false;
    ofEnableAlphaBlending();
    
    //shader stuff
    recordFbo.allocate(vidGrabber.width, vidGrabber.height);
    badTvShader.load("shaders/passthrough_vert.c", "shaders/badtv_frag.c");
    staticShader.load("shaders/passthrough_vert.c", "shaders/static_frag.c");
}

void ofApp::exit() {
    vidRecorder.close();
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
    vidGrabber.draw(0,0);
    
    stringstream ss;
    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
    << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
    << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;
    
    ofSetColor(0,0,0,100);
    ofRect(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(),15,15);
    
    if(bRecording){
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofCircle(ofGetWidth() - 20, 20, 5);
        ofPopStyle();
    }
    
    //recorded video
    if(recordedVideoPlayback.isLoaded()){
        recordFbo.begin();
        badTvShader.begin();
        //        staticShader.begin();
        recordedVideoPlayback.draw(0, 0);
        badTvShader.end();
        //        staticShader.end();
        recordFbo.end();
        recordFbo.draw(640, 0);
        
    }
}

void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    if(bRecording)
        vidRecorder.addAudioSamples(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            lastFile = fileName+ofGetTimestampString()+fileExt;
            vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, 0, 0, "-vcodec libx264 -b 800k -pix_fmt yuv420p -f mov " + ofFilePath::getAbsolutePath(lastFile), true, false); //the last booleans sync the video timing to the main thread
            
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
        
        //race condition!
        ofSleepMillis(500);
        
        //try and load the video
        recordedVideoPlayback.loadMovie(lastFile);
        recordedVideoPlayback.play();
    }if(key=='q'){
        exit();
    }
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
