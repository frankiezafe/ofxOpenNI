#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel( OF_LOG_ERROR );

    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers( 10 );
    openNIDevice.start();
    
    //openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers( 4 ); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers( 4 ); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    
    userSeen = 0;
    
    zmult = 1;
    rep.pos.y = -1000;
    rep.pos.z = 1500;
    rep.radius = 250;
    
    cam.setDistance(100);
    
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground( 0,0,0 );
    
//    cam.begin();
    
    ofSetColor( 255, 0, 255);
    ofLine( 0, ofGetHeight() * 0.5, ofGetWidth(), ofGetHeight() * 0.5 );
    ofLine( ofGetWidth() * 0.5, 0, ofGetWidth() * 0.5, ofGetHeight() );
    
    ofSetColor(255, 255, 255);
    
//    ofPushMatrix();
//    openNIDevice.drawDebug();
//    ofPopMatrix();
    
    glEnable( GL_DEPTH_TEST );
    
    ofPushMatrix();
    ofEnableBlendMode( OF_BLENDMODE_ALPHA );
    
    userSeen = openNIDevice.getNumTrackedUsers();
    
    bb.min.set( 0,0,0 );
    bb.max.set( 0,0,0 );
    bb.center.set( 0,0,0 );
    
    ofTranslate( ( ofGetWidth() * 0.5 ), ( ofGetHeight() * 0.5 ), 0 );
    
    ofRotateY( 180 );
    ofRotateZ( 180 );
    
    ofScale( 1, 1, zmult );
    
    ofPushMatrix();
        ofTranslate( rep.pos.x, rep.pos.y, rep.pos.z );
        ofRotateX( -90 );
        ofSetColor( 127,127,127 );
        ofFill();
        ofTranslate( rep.pos.x, rep.pos.y, rep.pos.z * 0.99 );
        ofCircle( 0,0,0, rep.radius * 2 );
        ofSetColor( 255, 255, 255 );
        ofFill();
        ofCircle( 0,0,0, rep.radius );
    ofPopMatrix();
    
//    ofTranslate( -320, -240, 0 );
    
    for (int i = 0; i < userSeen; i++){
        
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
//        user.drawMask();
        
        ofMesh & mesh = user.getPointCloud();
        vector< ofVec3f > & vertices = mesh.getVertices();
        int vcount = vertices.size();
        if ( vcount < 10 ) {
            // user not tracked
            continue;
        }
        
        vector< ofVec3f >::iterator itv;  
        for ( itv = vertices.begin(); itv != vertices.end(); itv++ ) {
            ofVec3f & v = (*itv);
            
            bb.center += v;
            
            if ( itv == vertices.begin() ) {
                bb.min.set( v );
                bb.max.set( v );
                continue;
            }
            
            if ( v.x < bb.min.x )
                bb.min.x = v.x;
            if ( v.y < bb.min.y )
                bb.min.y = v.y;
            if ( v.z < bb.min.z )
                bb.min.z = v.z;
            if ( v.x > bb.max.x )
                bb.max.x = v.x;
            if ( v.y > bb.max.y )
                bb.max.y = v.y;
            if ( v.z > bb.max.z )
                bb.max.z = v.z;
        }
        
        bb.center /= vcount;
        
        ofPushMatrix();
            ofTranslate( 0, 0, bb.min.z );
            ofSetColor( 255, 0, 0 );
            ofNoFill();
            ofRect( bb.min.x, bb.min.y, bb.max.x - bb.min.x, bb.max.y - bb.min.y );
        ofPopMatrix();
            
        ofPushMatrix();
            ofTranslate( bb.center.x, bb.center.y, bb.center.z );
            ofSetColor( 255, 0, 0 );
            ofLine( 0, -20, 0, 20 );
            ofLine( -20, 0, 20, 0 );
        ofPopMatrix();
        
        ofPushMatrix();
            ofTranslate( 0, 0, bb.max.z );
            ofSetColor( 255, 0, 0 );
            ofNoFill();
            ofRect( bb.min.x, bb.min.y, bb.max.x - bb.min.x, bb.max.y - bb.min.y );
        ofPopMatrix();
        
        ofSetColor(255, 255, 255);
        user.drawPointCloud();
                
    }
    
    ofDisableBlendMode();
    
    ofPopMatrix();
    
//    cam.end();
    
    ofSetColor(0, 255, 0);
    string msg = "MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());

    ofDrawBitmapString( msg, 20, 20 );
    ofDrawBitmapString( "user seen: " + ofToString( userSeen ), 20, 40 );
    ofDrawBitmapString( "bounding box:\n\t" + 
            ofToString( bb.min.x ) + "/" + ofToString( bb.min.y ) + "/" + ofToString( bb.min.z ) +"\n\t"+
            ofToString( bb.max.x ) + "/" + ofToString( bb.max.y ) + "/" + ofToString( bb.max.z )
            , 20, 60 );
    
}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString( event.userStatus ) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}