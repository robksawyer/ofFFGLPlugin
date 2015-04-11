#include "testApp.h"

#include "ofxFFGLPlugin.h"


testApp::testApp()
{
	// Here we add the parameters for the plugin 
	
	_power1 = 2.1;
	addFloatParameter("power 1",&_power1,0.001,7.0);
	
	_power2 = 2.1;
	addFloatParameter("power 2",&_power2,0.001,7.0);
	
	_rotx = 0.0;
	addFloatParameter("rot x",&_rotx,0.0,360.0);
	
	_roty = 0.0;
	addFloatParameter("rot y",&_roty,0.0,360.0);
	
	_radius = 1.0;
	addFloatParameter("radius",&_radius,0.0,10.0);
	
	_subdivision = 10.0;
	addFloatParameter("subdivision",&_subdivision,5.0,50.0);
	
	// Example parameters
	
	// event parameter using a Trigger<bool> class
	// this will automatically handle value changes for us ( see draw() )
	addEventParameter("flash",&eventExample.val);
	
	// std::string example
	stdString = "hello world";
	addStringParameter("std string",&stdString);

	// oldschool C string with a buffer of chars
	strcpy(cString,"hello world");
	addCStringParameter("c string",cString);
	
	
}

//--------------------------------------------------------------
void testApp::setup(){
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	static float brt = 0;
	
	// calling isTriggered on a Trigger<> object, tells us if the internal value has changed.
	// this is handy for handling event parameters in FFGL without having to mantain two copies of a member
	if(eventExample.isTriggered())
	{
		brt = 1.0;
	}
	
	brt*=1.0-ofGetLastFrameTime();
	glClearColor(brt,brt,brt,brt);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	// draw test strings
	ofSetupScreen();
	
	ofSetColor(255,255,255);
	ofDrawBitmapString(stdString,20,20);
	ofDrawBitmapString(cString,20,40);
	
	// draw ellipsoid
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0*RAD_TO_DEG, (float)ofGetWidth()/ofGetHeight(), 0.1, 100.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-15.0f );
	glRotatef(_rotx,1.0,0.0,0.0);
	glRotatef(_roty,0.0,1.0,0.0);
	
	createSuperEllipse( _power1, _power2, _subdivision, 0 );
	
}


// superellipsoid code by Paul Bourke adapted from http://local.wasp.uwa.edu.au/~pbourke/geometry/superellipse/

void testApp::createSuperEllipse(double power1,double power2,int n,int method)
{
   int i,j;
   double theta1,theta2,theta3;
   XYZ p,p1,p2,en;
   double delta;

	glColor4f(1.0,1.0,1.0,1.0);
	
   /* Shall we just draw a point? */
   if (n < 4) {
      glBegin(GL_POINTS);
      glVertex3f(0.0,0.0,0.0);
      glEnd();
      return;
   }

   /* Shall we just draw a plus */
   if (power1 > 10 && power2 > 10) {
      glBegin(GL_LINES);
      glVertex3f(-1.0, 0.0, 0.0);
      glVertex3f( 1.0, 0.0, 0.0);
      glVertex3f( 0.0,-1.0, 0.0);
      glVertex3f( 0.0, 1.0, 0.0);
      glVertex3f( 0.0, 0.0,-1.0);
      glVertex3f( 0.0, 0.0, 1.0);
      glEnd();
      return;
   }

   delta = 0.01 * TWO_PI / n;
   for (j=0;j<n/2;j++) {
      theta1 = j * TWO_PI / (double)n - PID2;
      theta2 = (j + 1) * TWO_PI / (double)n - PID2;

      if (method == 0)
         glBegin(GL_QUAD_STRIP);
      else
         glBegin(GL_TRIANGLE_STRIP);
      for (i=0;i<=n;i++) {
         if (i == 0 || i == n)
            theta3 = 0;
         else
            theta3 = i * TWO_PI / n;

         evalSuperEllipse(theta2,theta3,power1,power2,&p);
         evalSuperEllipse(theta2+delta,theta3,power1,power2,&p1);
         evalSuperEllipse(theta2,theta3+delta,power1,power2,&p2);
        // en = CalcNormal(p1,p,p2);
         //glNormal3f(en.x,en.y,en.z);
        // glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
         glVertex3f(p.x,p.y,p.z);

         evalSuperEllipse(theta1,theta3,power1,power2,&p);
         evalSuperEllipse(theta1+delta,theta3,power1,power2,&p1);
         evalSuperEllipse(theta1,theta3+delta,power1,power2,&p2);
         //en = CalcNormal(p1,p,p2);
         //glNormal3f(en.x,en.y,en.z);
        // glTexCoord2f(i/(double)n,2*j/(double)n);
         glVertex3f(p.x,p.y,p.z);
      }
      glEnd();
   }
}

void testApp::evalSuperEllipse(double t1,double t2,double p1,double p2,XYZ *p)
{
   double tmp;
   double ct1,ct2,st1,st2;

   ct1 = cos(t1);
   ct2 = cos(t2);
   st1 = sin(t1);
   st2 = sin(t2);

   tmp  = ofSign(ct1) * pow(fabs(ct1),p1);
   p->x = tmp * ofSign(ct2) * pow(fabs(ct2),p2) * _radius;
   p->y = ofSign(st1) * pow(fabs(st1),p1) * _radius;
   p->z = tmp * ofSign(st2) * pow(fabs(st2),p2) * _radius;
}



//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

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
