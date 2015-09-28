#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber());
    
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.setGrabber(vidRecorder);
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(GRABBER_WIDTH, GRABBER_HEIGHT);
    
    vidRecorder->initRecording();
    
    // Register for events so we'll know when videos finish saving.
    // TODO: add event for when video is done converting. Do this conversion via node.
    ofAddListener(vidRecorder->videoSavedEvent, this, &ofApp::videoSaved);

    
    bRecording = false;
    ofEnableAlphaBlending();
    
    badTvShader.load("shaders/passthrough_vert.c", "shaders/badtv_frag.c");
    staticShader.load("shaders/passthrough_vert.c", "shaders/static_frag.c");
    
    //OSC Setup
    sender.setup(HOST, SEND_PORT);
    receiver.setup(RECEIVE_PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    
    if(recordedVideoPlayback.isLoaded()){
        recordedVideoPlayback.update();
    }
    
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
    
    //TODO: scale the video and maintain the aspect ratio
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

void ofApp::videoSaved(ofVideoSavedEventArgs& e){
    // the ofQTKitGrabber sends a message with the file name and any errors when the video is done recording
    if(e.error.empty()){
        //send the OSC signal with e.videoPath to start the conversion process
        //load the video when this process is complete
        
//        recordedVideoPlayback.loadMovie(e.videoPath);
//        recordedVideoPlayback.play();
        
        //clue for how to launch system commands
        //could be used to send the last video somewhere
        //        if(bLaunchInQuicktime) {
        //            ofSystem("open " + e.videoPath);
        //        }
    }
    else {
        ofLogError("videoSavedEvent") << "Video save error: " << e.error;
    }
}

//-------------------------------------------------------------
void ofApp::exit(){

}

//-------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' '){
        
        //if it is recording, stop
        if(vidRecorder->isRecording()){
            vidRecorder->stopRecording();
        }
        else {
            // otherwise start a new recording.
            // before starting, make sure that the video file
            // is already in use by us (i.e. being played), or else
            // we won't be able to record over it.
            if(recordedVideoPlayback.isLoaded()){
                recordedVideoPlayback.close();
            }
            vidRecorder->startRecording(ofGetTimestampString("%n-%e-%Y-%H-%M-%s_")+ "oc.mov");
        }
    }
}

//--------------------------------------------------------------
void ofApp::sendMessage(const string& filename, const string& address){
    cout << "filename: " << filename << ", address: " << address << endl;
    ofxOscMessage m;
    m.setAddress(address);
    m.addStringArg(filename);
    sender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
