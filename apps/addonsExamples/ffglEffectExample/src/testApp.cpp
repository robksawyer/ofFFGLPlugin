#include "testApp.h"

#include "ofxFFGLPlugin.h"

testApp::testApp()
{
	// add parameters
	
	numRepeats = 1;
	
	addFloatParameter(	"num repeats",	// name of parameter ( as it will appear in host )
						&numRepeats,	// address of the float this parameter will point to
						1,				// minimum value
						20				// maximum value
					  );
}

//--------------------------------------------------------------
void testApp::setup(){
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
}

//--------------------------------------------------------------
void testApp::draw(){
	// input textures from host are stored here
	ofTexture * tex = inputTextures[0];
	
	if( !tex )
		return;

	int nrep = numRepeats;

	float w = (float)ofGetWidth() / nrep;
	float h = (float)ofGetHeight() / nrep;
	
	for( int y = 0; y < nrep; y++ )
		for( int x = 0; x < nrep; x++ )
		{
			tex->draw(w*x,h*y,w,h);
		}
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
