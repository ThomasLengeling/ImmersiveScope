#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxDatGui.h"

#include "ofxOpenCvDnnObjectDetection.h"

#include "DetectedObject.h"

// send host (aka ip address)
#define HOST "127.0.0.1"

/// send port
#define PORT 10000

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
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);    

    ofVideoGrabber camera;
    ofVideoPlayer video; // for video detection
    ofxOpenCvDnnObjectDetection detector;
    
    
    ofImage img;
    uint64_t detection_time;
    
    
    bool        activeSendPhone;
    bool        activeSendCup;
    ofxOscSender sender;
    
    
    void        drawROI();
    
    ofRectangle    mROICam;
    glm::vec2      mROITopPos;
    glm::vec2      mROIBottomPos;
    glm::vec2      mCenterROI;
    
    DectectorObjectRef   mDectectorTime;
};
