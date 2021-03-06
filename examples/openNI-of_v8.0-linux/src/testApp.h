#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"

struct BoundingBox {
  ofVec3f min;
  ofVec3f max;
  ofVec3f center;
};

struct Repere {
    ofVec3f pos;
    float radius;
};

class testApp : public ofBaseApp{

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

        void userEvent(ofxOpenNIUserEvent & event);
    
        int userSeen;
        BoundingBox bb;
	ofxOpenNI openNIDevice;
        
        float zmult;
        Repere rep;
        
        ofEasyCam cam;
        
};

#endif
