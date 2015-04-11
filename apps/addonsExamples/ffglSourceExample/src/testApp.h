#ifndef _TEST_APP
#define _TEST_APP


#include "ofxFFGLApp.h"

#define PID2	PI/2.0

struct XYZ
{
	float x;
	float y;
	float z;
};

class testApp : public ofFFGLApp{
	
	public:
		testApp();
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
		float _power1;
		float _power2;
		float _rotx;
		float _roty;
		float _radius;
		float _subdivision;
		
		// other example parameters
		
		Trigger<bool> eventExample; // event
		std::string stdString; // string
		char cString[512]; // C string
		
		
		void createSuperEllipse(double power1,double power2,int n,int method);
		void evalSuperEllipse(double t1,double t2,double p1,double p2,XYZ *p);
		
};

#endif
	
