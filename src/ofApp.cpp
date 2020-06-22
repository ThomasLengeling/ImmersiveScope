#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
    ofSetFrameRate(60);
    // setup camera
    camera.setDeviceID( 0 );
    camera.initGrabber( 1280, 720 );
    ofSetWindowShape(camera.getWidth(), camera.getHeight());
    
    detector.setNetworkImageSize(416, 416);
    detector.setup(ofToDataPath("dnn/yolov3-tiny.cfg"),
                   ofToDataPath("dnn/yolov3-tiny.weights"),
                   ofToDataPath("dnn/coco.txt"));
    
    // For Video detection example
    //    video.load(ofToDataPath("sample.mp4"));
    //    ofSetWindowShape(video.getWidth(), video.getHeight());
    //    // setup dnn network for object detection. Case: Yolov2-tiny
    //    detector.setNetworkImageSize(608, 608);
    //    detector.setup(ofToDataPath("dnn/yolov3-spp.cfg"),
    //                   ofToDataPath("dnn/yolov3-spp.weights"),
    //                   ofToDataPath("dnn/coco.txt"));
    //video.play();
    
    detector.setConfidenceThreshold(0.6);
    cout << "version:" << CV_VERSION << endl;
    
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
        
    camera.update();
    if( camera.isFrameNew() ){
        // camera image automatically will be croped to detector.setNetworkImageSige
        uint64_t t0 = ofGetElapsedTimeMillis();
        detector.update(camera.getPixels());
        detection_time = ofGetElapsedTimeMillis()-t0;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    
    camera.draw(0,0);
    // For video detection
    //    video.draw(0,0);
    
    detector.draw(0,0, ofGetWidth(), ofGetHeight());
    ofDrawBitmapStringHighlight("FPS:"+ofToString(ofGetFrameRate(), 0),20, 20);
    ofDrawBitmapStringHighlight("Detection Time:"+ofToString(detection_time),20, 40);
    
    //rectangle
    for( int i = 0; i < detector.object.size(); i++ ){
        ofRectangle box = detector.object[i].r;
        std::string name = detector.object[i].name;
        int classId = detector.object[i].class_id;
        float p = detector.object[i].p;
        ofDrawBitmapStringHighlight("Det: "+name+" "+to_string(classId)+" "+to_string(p), 20, 60 + 20*i);
        
        //phone
        if(classId == 67){
            std::cout<<"sending phone"<<std::endl;
            activeSendPhone = true;
        }
        
        //cup
        if(classId == 41){
            std::cout<<"sending Cup"<<std::endl;
            activeSendCup = true;
        }
    }
    
    if(activeSendPhone){
        ofxOscMessage m;
        m.setAddress("/phone");
        m.addIntArg(1); //found  one
        m.addFloatArg(0.5); //probabily
        m.addIntArg(0.2);
        
        sender.sendMessage(m, false);
        activeSendPhone =  false;
    }
    
    if(activeSendCup){
        ofxOscMessage m;
        m.setAddress("/cup");
        m.addIntArg(1);
        sender.sendMessage(m, false);
        activeSendCup =  false;
    }
    
}

void ofApp::keyPressed(int key){
    
    if(key == 'a'){
        ofxOscMessage m;
        m.setAddress("/phone");
        m.addIntArg(0);
        sender.sendMessage(m, false);
    }
    
}


