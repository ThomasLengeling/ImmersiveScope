#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "ofxOpenCvDnnObjectDetection.h"

// send host (aka ip address)
#define HOST "127.0.0.1"

/// send port
#define PORT 32000

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    ofVideoGrabber camera;
    ofVideoPlayer video; // for video detection
    ofxOpenCvDnnObjectDetection detector;
    
    void keyPressed(int key);
    ofImage img;
    uint64_t detection_time;
    
    
    bool        activeSendPhone;
    bool        activeSendCup;
    ofxOscSender sender;

};
