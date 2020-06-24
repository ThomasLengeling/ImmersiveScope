//
//  DetectedObject.hpp
//  ImmersiveScope
//
//  Created by Thomas on 6/22/20.
//

#pragma once

#include "ofMain.h"

class DectectorObject;
typedef std::shared_ptr<DectectorObject> DectectorObjectRef;

class DectectorObject{

public:
    
    DectectorObject() {
        mIncTime = 0.01;
        cTime = 0;
        mLock = true;
        mDetected = 0;
        cLockTime = ofGetElapsedTimeMillis();
    }
    
    static DectectorObjectRef create() {
        return std::make_shared<DectectorObject>();
    }
    
    void update(float time){
        if(abs(time - cTime) < 100.0){
            cout<<"time"<<std::endl;
            if(mDetected >= 50){
                mLock = false;
                cout<<"detected"<<std::endl;
            }
        }
        cout<<mDetected<<"  "<<abs(time - cTime)<<std::endl;
        
        if(mLockTime){
            if(abs(time - cLockTime) > 2500.0){
                cout<<" reset "<<std::endl;
                reset();
            }
        }
    }
    
    void reset(){
        cTime = 0;
        mDetected = 0;
        mLockTime = false;
    }
    
    void detected(){
        if(!mLockTime){
            mDetected++;
            cTime = ofGetElapsedTimeMillis();
        }else{
            cLockTime = ofGetElapsedTimeMillis();
        }
    }
    
    bool isLocked(){
        return mLock;
    }
    
    bool lockTime(){
        cLockTime = ofGetElapsedTimeMillis();
        mLockTime = true;
        mLock     = true;
    }


private:
    
    //long increments
    float cTime;
    float cLockTime;
    float mIncTime;
    
    int mDetected;
    
    bool  mLock;
    
    //short increments
    
    bool  mLockTime;
};
