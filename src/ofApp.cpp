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
    
    detector.setConfidenceThreshold(.38);
    cout << "version:" << CV_VERSION << endl;
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
    
    //gui
    
    //ROI
    mROITopPos    = glm::vec2(450, 200);
    mROIBottomPos = glm::vec2(900, 600);
    
    mCenterROI  = glm::vec2((mROIBottomPos.x + mROITopPos.x)/2.0, (mROIBottomPos.y + mROITopPos.y)/2.0);
    
    //clean
    mDectectorTime =  DectectorObject::create();
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
    
    //Camera
    camera.draw(0,0);
    
    //draw decector
    detector.draw(0,0, ofGetWidth(), ofGetHeight());
    ofDrawBitmapStringHighlight("FPS:"+ofToString(ofGetFrameRate(), 0),20, 20);
    ofDrawBitmapStringHighlight("Detection Time:"+ofToString(detection_time),20, 40);
    
    //rectangle
    for( int i = 0; i < detector.object.size(); i++ ){
        
        glm::vec2 topRec = glm::vec2(detector.object[i].r.getMinX() * ofGetWidth(),detector.object[i].r.getMinY() * ofGetHeight());
        glm::vec2 bottomRec = glm::vec2(detector.object[i].r.getMaxX() * ofGetWidth(),detector.object[i].r.getMaxY() * ofGetHeight());
        glm::vec2 centerDet = glm::vec2((topRec.x + bottomRec.x)/2.0, (topRec.y + bottomRec.y)/2.0);
        
        //cout<<topRec.x<<" "<<topRec.y<<" "<<bottomRec.x<<" "<<bottomRec.y<<std::endl;
        
        ofSetColor(0, 255, 0);
        ofDrawCircle(topRec.x, topRec.y, 2);
        ofDrawCircle(bottomRec.x, bottomRec.y, 2);
        
        if( centerDet.x > mROITopPos.x && centerDet.y > mROITopPos.y &&
           centerDet.x <  mROIBottomPos.x && centerDet.y < mROIBottomPos.y){
            ofRectangle box = detector.object[i].r;
            std::string name = detector.object[i].name;
            int classId = detector.object[i].class_id;
            float p = detector.object[i].p;
            ofDrawBitmapStringHighlight("Det: "+name+" "+to_string(classId)+" "+to_string(p), 20, 60 + 20*i);
            
            //phone
            if(classId == 67){
                mDectectorTime->detected();
            }
            
            //cup
            if(classId == 41){
                std::cout<<"sending Cup"<<std::endl;
                activeSendCup = true;
            }
        }
    }
    
    mDectectorTime->update(ofGetElapsedTimeMillis());
    
    if(!mDectectorTime->isLocked()){
        ofxOscMessage m;
        
        m.setAddress("/phone");
        m.addIntArg(1); //found  one
        m.addFloatArg(0.5); //probabily
        m.addIntArg(0.2);
        
        sender.sendMessage(m, false);
        
        std::cout<<"sending phone"<<std::endl;
        activeSendPhone =  false;
        mDectectorTime->lockTime();
        // mDectectorTime->reset();
        
        ofRectangle
    }
    
    if(activeSendCup){
        ofxOscMessage m;
        m.setAddress("/cup");
        m.addIntArg(1);
        sender.sendMessage(m, false);
        activeSendCup =  false;
    }
    
    //draw
    drawROI();
    
}
//--------------------------------------------------------------
void ofApp::drawROI(){
    ofPushStyle();
    ofNoFill();
    ofSetColor(0, 255, 0);
    ofSetLineWidth(2.5);
    ofDrawRectangle(mROITopPos, abs(mROIBottomPos.x - mROITopPos.x), abs(mROIBottomPos.y - mROITopPos.y));
    
    
    
    ofFill();
    ofSetColor(200, 150, 0);
    ofDrawCircle(mROITopPos.x, mROITopPos.y, 5);
    ofDrawBitmapStringHighlight(""+to_string(int(mROITopPos.x))+", "+to_string(int(mROITopPos.y)), mROITopPos.x - 10, mROITopPos.y - 5);
    
    ofSetColor(0, 150, 100);
    ofDrawCircle(mROIBottomPos.x, mROIBottomPos.y, 5);
    ofDrawBitmapStringHighlight(""+to_string(int(mROIBottomPos.x))+", "+to_string(int(mROIBottomPos.y)), mROIBottomPos.x +10, mROIBottomPos.y - 5);
    
    ofSetColor(50, 150, 100);
    ofDrawCircle(mCenterROI.x, mCenterROI.y, 4);
    
    ofPopStyle();
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'a'){
        ofxOscMessage m;
        m.setAddress("/phone");
        m.addIntArg(0);
        sender.sendMessage(m, false);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if( ofDist(x, y, mROITopPos.x, mROITopPos.y) < 35){
        mROITopPos.x = x;
        mROITopPos.y = y;
    }
    
    if( ofDist(x, y, abs(mROIBottomPos.x), abs(mROIBottomPos.y)) < 35){
        mROIBottomPos.x = x;
        mROIBottomPos.y = y;
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
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

//--------------------------------------------------------------
void ofApp::exit(){
    
}
