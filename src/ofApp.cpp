#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    
    ofSetFrameRate(60);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(640, 480);
    
    fileName = "testMovie";
    
    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    string bitRate = "1000k";
    //    vidRecorder.setVideoCodec("libx264");
    vidRecorderMP4.setVideoBitrate(bitRate);
    vidRecorderMP4.setVideoCodec("libx264");
    vidRecorderMP4Distort.setVideoBitrate(bitRate);
    vidRecorderMP4Distort.setVideoCodec("libx264");
    
    bRecording = false;
    ofEnableAlphaBlending();
    
    //shader stuff
    filmFbo.allocate(vidGrabber.width, vidGrabber.height, GL_RGB);
    badTVFbo.allocate(vidGrabber.width, vidGrabber.height, GL_RGB);
    rgbShiftFbo.allocate(vidGrabber.width, vidGrabber.height, GL_RGB);
    staticFbo.allocate(vidGrabber.width, vidGrabber.height, GL_RGB);
    clearFbo(filmFbo);
    clearFbo(badTVFbo);
    clearFbo(rgbShiftFbo);
    clearFbo(staticFbo);
    
    //set up basic quad
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
    
    //set up font
    openSansLarge.loadFont("OpenSans-Regular.ttf", 22);
    openSansRegular.loadFont("OpenSans-Regular.ttf", 18);
}

void ofApp::exit() {
    vidRecorderMP4.close();
    vidRecorderMP4Distort.close();
}

//--------------------------------------------------------------
void ofApp::update(){
    if(bRecording){
        long long now = ofGetElapsedTimeMillis();
        if(mark - now <= 0){
            stopRecording();
            return;
        }
    }
    
    vidGrabber.update();
    if(vidGrabber.isFrameNew() && bRecording){
        if( !vidRecorderMP4.addFrame(vidGrabber.getPixelsRef())){
            ofLogWarning("This frame was not added to the mp4 recorder!");
        }
        
        
        ofPixels px;
        staticFbo.readToPixels(px);
        if (!vidRecorderMP4Distort.addFrame(px)){
            ofLogWarning("This frame was not added to the distorted mp4 recorder!");
        }
    }
    
    // Check if the video recorder encountered any error while writing video frame or audio smaples.
    if( vidRecorderMP4.hasVideoError()){
        ofLogWarning("The mp4 video recorder failed to write some frames!");
    }
    if(vidRecorderMP4Distort.hasVideoError()){
        ofLogWarning("The distorted mp4 video recorder failed to write some frames!");
    }
    
    time += 0.1;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255, 255, 255);

    //reference points
    ofPoint videoTopLeft((ofGetWidth() - staticFbo.getWidth())/2, 0 );
    ofPoint videoTopRight = videoTopLeft + ofPoint(staticFbo.getWidth(), 0);
    ofPoint videoBottomLeft = videoTopLeft + ofPoint(0, staticFbo.getHeight());
    ofPoint videoBottomRight = videoTopLeft + ofPoint(staticFbo.getWidth(), staticFbo.getHeight());
    
    
    //FBO sequence
    // Output to texture to make sure that FBO and texture color profiles match up
    ofTexture tex;
    tex.loadData(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), GL_RGB);
    
    filmFbo.begin();
    filmShader.begin();
    filmShader.setUniformTexture("tDiffuse", tex, 1);
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
    
    staticFbo.draw(videoTopLeft);
    
    //corners
    float margin = 10.0;
    float shortEdge = 3.0;
    float longEdge = 50.0;
    //top left
    ofRect(videoTopLeft.x + margin, videoTopLeft.y + margin, longEdge, shortEdge);
    ofRect(videoTopLeft.x + margin, videoTopLeft.y + margin, shortEdge, longEdge);
    //top right
    ofRect(videoTopRight.x - margin - longEdge, videoTopRight.y + margin, longEdge, shortEdge);
    ofRect(videoTopRight.x - margin - shortEdge, videoTopRight.y + margin, shortEdge, longEdge);
    //bottom left
    ofRect(videoBottomLeft.x + margin, videoBottomLeft.y - margin - shortEdge, longEdge, shortEdge);
    ofRect(videoBottomLeft.x + margin, videoBottomLeft.y - margin - longEdge, shortEdge, longEdge);
    //bottom right
    ofRect(videoBottomRight.x - margin - longEdge, videoBottomRight.y - margin - shortEdge, longEdge, shortEdge);
    ofRect(videoBottomRight.x - margin - shortEdge, videoBottomRight.y - margin - longEdge, shortEdge, longEdge);
    
    //timer
    if(bRecording){
        ofPushStyle();
        ofSetColor(255, 0, 0);
        openSansLarge.drawString("REC", videoTopRight.x - margin * 5 - 60, videoTopRight.y + margin * 4 + 10);
        ofCircle(videoTopRight.x - margin * 4, videoTopRight.y + margin * 4, 10);
        
        string timestamp = generateTimeStamp(mark - ofGetElapsedTimeMillis());
        openSansLarge.drawString(timestamp, videoBottomLeft.x + staticFbo.getWidth()/2 - 60, videoBottomLeft.y - margin);
        ofPopStyle();
    }
    
    drawButton(ofVec2f(videoBottomRight.x - 50, videoBottomRight.y + 60));
    
    if( hideGui ){
//        ofShowCursor();
        gui.draw();
    }else{
//        ofHideCursor();
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
void ofApp::startRecording(const unsigned long long duration){
    bRecording = !bRecording;
    mark = ofGetElapsedTimeMillis() + duration;
    if(bRecording && !vidRecorderMP4.isInitialized() && !vidRecorderMP4Distort.isInitialized()) {
        lastFile = fileName+ofGetTimestampString();
        vidRecorderMP4.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, 0, 0, "-vcodec libx264 -b 1000k -pix_fmt yuv420p -f mp4 " + ofFilePath::getAbsolutePath(lastFile + ".mp4"), true, false); //the last booleans sync the video timing to the main thread
        vidRecorderMP4Distort.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, 0, 0, "-vcodec libx264 -b 1000k -pix_fmt yuv420p -f mp4 " + ofFilePath::getAbsolutePath(lastFile) + "_distorted.mp4", true, false); //the last booleans sync the video timing to the main thread
        
        vidRecorderMP4.start();
        vidRecorderMP4Distort.start();
    }
}

//--------------------------------------------------------------
void ofApp::stopRecording(){
    bRecording = false;
    vidRecorderMP4.close();
    vidRecorderMP4Distort.close();
    
    //TODO:signal via osc that we've saved a new set of videos
}

string ofApp::generateTimeStamp(unsigned long long time){
    stringstream ss;
    //minutes : seconds . thousands of a second
    int seconds = time /1000;
    int hundreds = (float(time) / 1000 - seconds) * 100;
    
    ss << "00:" << ((seconds < 10)? "0":"") << seconds << ":" << ((hundreds < 10)? "0":"") << hundreds << endl;
    
    return ss.str();
}

//--------------------------------------------------------------
void ofApp::drawButton(const ofVec2f& center){
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofCircle(center, buttonSize);
    ofSetColor(255);
    openSansRegular.drawString("Record", center.x - 39, center.y + 7);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key=='r'){
        //TODO: remove stop/start functionality
        if(!bRecording){
            startRecording(DURATION);
        }
    }
    if(key=='c'){
        stopRecording();
    }if(key=='q'){
        ofExit();
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
    ofVec2f p1(x, y);
    ofVec2f buttonCenter(670, 540); //TODO: make the button center and important points a struct
    
    if(p1.distance(buttonCenter) < buttonSize){
        startRecording(DURATION);
    }
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
