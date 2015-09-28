#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        const static int GRABBER_WIDTH = 640;
        const static int GRABBER_HEIGHT = 480;
        const static int SEND_PORT = 12346;
        const static int RECEIVE_PORT = 12345;
    
        const std::string HOST = "localhost";
    
        ofVideoGrabber      vidGrabber;
        ofPtr<ofQTKitGrabber>	vidRecorder;
        ofVideoPlayer recordedVideoPlayback;
    
        void videoSaved(ofVideoSavedEventArgs& e);

        bool bRecording;
        int sampleRate;
        int channels;
    
        string fileName;
        string fileExt;
        string fullFileName;
    
        ofShader badTvShader, staticShader;
    
        ofFbo recordFbo;
        ofPixels recordPixels;
    
        ofxOscReceiver receiver;
        ofxOscSender sender;
        void sendMessage(const string& filename, const string& address);
};
