#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    
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
    filmFbo.allocate(vidGrabber.width, vidGrabber.height);
    badTVFbo.allocate(vidGrabber.width, vidGrabber.height);
    rgbShiftFbo.allocate(vidGrabber.width, vidGrabber.height);
    staticFbo.allocate(vidGrabber.width, vidGrabber.height);
    clearFbo(badTVFbo);
    clearFbo(rgbShiftFbo);
    clearFbo(staticFbo);
    
    quad.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    quad.addVertex(ofPoint(0, vidGrabber.width, 0));
    quad.addTexCoord(ofPoint(0, vidGrabber.width, 0));
    quad.addVertex(ofPoint(0, 0, 0));
    quad.addTexCoord(ofPoint(0, 0, 0));
    quad.addVertex(ofPoint(vidGrabber.width, vidGrabber.height, 0));
    quad.addTexCoord(ofPoint(vidGrabber.width, vidGrabber.height, 0));
    quad.addVertex(ofPoint(vidGrabber.width, 0, 0));
    quad.addTexCoord(ofPoint(vidGrabber.width, 0, 0));
    
    filmShader.load("shaders/passthrough_vert.c", "shaders/film_shader.c");
    badTvShader.load("shaders/passthrough_vert.c", "shaders/badtv_frag.c");
    rgbShiftShader.load("shaders/passthrough_vert.c", "shaders/rgb_shift_frag.c");
    staticShader.load("shaders/passthrough_vert.c", "shaders/static_frag.c");
    
    //gui stuff
    BadTV.setName("Bad TV");
    BadTV.add(thickDistort.set("Thick Distort", 0.1, 0.1, 20));
    BadTV.add(fineDistort.set("Fine Distort", 0.1, 0.1, 20));
    BadTV.add(distortSpeed.set("Distort Speed", 0.0, 0.0, 1.0));
    BadTV.add(rollSpeed.set("Roll Speed", 0.0, 0.0, 1.0));
    time = 0.0;
    
    RGBShift.setName("RGB Shift");
    RGBShift.add(rgbAmount.set("Amount", 0.0, 0.0, 0.1));
    RGBShift.add(angle.set("Angle", 0.0, 0.0, 2.0));
    
    Static.setName("Static");
    Static.add(staticAmount.set("Amount", 0.0, 0.0, 1.0));
    Static.add(size.set("Size", 1, 1, 100));
    
    Scanlines.setName("Scanlines");
    Scanlines.add(count.set("Count", 800, 50, 1000));
    Scanlines.add(sIntensity.set("sIntensity", 0.4, 0, 2));
    Scanlines.add(nIntensity.set("nIntensity", 0.1, 0, 2));
    
    parameters.add(BadTV);
    parameters.add(RGBShift);
    parameters.add(Static);
    parameters.add(Scanlines);
    
    gui.setup(parameters);
    gui.minimizeAll();
    
    //for testing so I don't have to keep taking videos
    recordedVideoPlayback.loadMovie("test_video.mov");
    recordedVideoPlayback.play();
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
        
        //do fbo stack stuff here?
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
    
    time += 0.1;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
    vidGrabber.draw(0,0);
    
    stringstream ss;
    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
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
        
        filmFbo.begin();
        filmShader.begin();
        filmShader.setUniformTexture("tDiffuse", recordedVideoPlayback.getTextureReference(), 1);
        filmShader.setUniform1f("width", vidGrabber.width);
        filmShader.setUniform1f("height", vidGrabber.height);
        filmShader.setUniform1f("time", time);
        filmShader.setUniform1i("grayscale", 0);
        filmShader.setUniform1f("nIntensity", nIntensity);
        filmShader.setUniform1f("sIntensity", sIntensity);
        filmShader.setUniform1f("sCount", count);
        quad.draw();
        filmShader.end();
        filmFbo.end();
        
        badTVFbo.begin();
        badTvShader.begin();
        badTvShader.setUniformTexture("tDiffuse", filmFbo.getTextureReference(), 1);
        badTvShader.setUniform1f("width", vidGrabber.width);
        badTvShader.setUniform1f("height", vidGrabber.height);
        badTvShader.setUniform1f("time", time);
        badTvShader.setUniform1f("distortion", thickDistort);
        badTvShader.setUniform1f("distortion2", fineDistort);
        badTvShader.setUniform1f("speed", distortSpeed);
        badTvShader.setUniform1f("rollSpeed", rollSpeed);
        quad.draw();
        badTvShader.end();
        badTVFbo.end();
        
        rgbShiftFbo.begin();
        rgbShiftShader.begin();
        rgbShiftShader.setUniformTexture("tDiffuse", badTVFbo.getTextureReference(), 1);
        rgbShiftShader.setUniform1f("width", vidGrabber.width);
        rgbShiftShader.setUniform1f("height", vidGrabber.height);
        rgbShiftShader.setUniform1f("amount", rgbAmount);
        rgbShiftShader.setUniform1f("angle", angle);
        quad.draw();
        rgbShiftShader.end();
        rgbShiftFbo.end();
    
        staticFbo.begin();
        staticShader.begin();
        staticShader.setUniformTexture("tDiffuse", rgbShiftFbo.getTextureReference(), 1);
        staticShader.setUniform1f("width", vidGrabber.width);
        staticShader.setUniform1f("height", vidGrabber.height);
        staticShader.setUniform1f("time", time);
        staticShader.setUniform1f("amount", staticAmount);
        staticShader.setUniform1f("size", size);
        quad.draw();
        staticShader.end();
        staticFbo.end();
        
        //final output
        staticFbo.draw(640, 0);
        
    }
    
    if( hideGui ){
        ofShowCursor();
        gui.draw();
    }else{
        ofHideCursor();
    }
}

//--------------------------------------------------------------
void ofApp::clearFbo(ofFbo &fbo){
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
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
    if(key=='h'){
        hideGui = !hideGui;
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
