#include "App.h"

#include "ofxFFGLPlugin.h"
#include "particleImg.h"


App::App()
{
	// Here we add the parameters for the plugin 
	spawnRadius = 8.0;
	addFloatParameter("spawn radius",&spawnRadius,2,25.0);
	
	damping = 4.0;
//	addFloatParameter("damping",&damping,0,4.0);

	explosionPower = 36;
	addFloatParameter("power",&explosionPower,10,70.0);

	explosionRandomness = 0.1;
	addFloatParameter("randomness",&explosionRandomness,0,1);

	explosionDuration = 3.1;
	addFloatParameter("duration",&explosionDuration,1.0,5.0);

	pointSize = 35;
	addFloatParameter("particle size",&pointSize,5,130);
	
	
	autoShoot = true;
	addBoolParameter("auto shoot",&autoShoot);
	
	addEventParameter("shoot",&spawnTrigger.val);
	
	
	camDistance = -65;
	//addFloatParameter("distance",&distance,-120,-100);
	
	rotx = 0;
	addFloatParameter("rotation",&rotx,-90,90);
	
	rotySpeed = 0;
	addFloatParameter("orbit speed",&rotySpeed,-180,180);
	
	doFlashes = false;
	addBoolParameter("sky flash",&doFlashes);
	
	addBoolParameter("blink",&Firework::blink);
	
	curFirework = 0;
	skyLight = 0;
}


//--------------------------------------------------------------
void App::setup(){

	//ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
	
	curFirework = 0;
	
	for( int i = 0; i < NFIREWORKS; i++ )
	{
		f[i].init(5000);
		f[i].addListener(this);
	}
	
	ofDisableArbTex(); // use normal GL_TEXTURE_2D textures or point sprites won't work...
	ptex.setFromPixels(particleImg,64,64,OF_IMAGE_COLOR_ALPHA);	
	
}


//--------------------------------------------------------------

void App::computeParticles()
{
	double msecs = ofGetLastFrameTime()*1000;
	for( int i = 0; i < NFIREWORKS; i++ )
	{
		f[i].spawnRadius = spawnRadius;
		f[i].damping = damping;
		f[i].explosionPower = explosionPower;
		f[i].explosionRandomness = explosionRandomness;
		f[i].explosionDuration = explosionDuration;
		f[i].integrate(msecs);
	}
	
}


//--------------------------------------------------------------


void App::drawFirework( Firework & fw, float ptSize )
{
	glPointSize(ptSize);
	
	// draw trails
	Particle* p = fw.getTrailParticles();

	glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(Particle),&p->color );
	glVertexPointer(	3,
						GL_FLOAT,
						sizeof(Particle),
						&p->pos );
	glDrawArrays(GL_POINTS, 0, fw.getNumTrailParticles()); 
	
	// draw particles
	p = fw.getParticles(); 
	glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(Particle),&p->color );
	glVertexPointer(	3,
						GL_FLOAT,
						sizeof(Particle),
						&p->pos );
	glDrawArrays(GL_POINTS, 0, fw.getNumParticles()); 
	
}


//--------------------------------------------------------------

void  App::drawParticles()
{	
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY); 
	
	for( int i = 0; i < NFIREWORKS; i++ )
	{
		drawFirework(f[i], (f[i].exploding && !f[i].explosionTrails) ? pointSize*2 : pointSize);
	}
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

//--------------------------------------------------------------

void App::shoot()
{
	f[curFirework].spawn(ofRandom(-3,3), ofRandom(-20,-2), ofRandom(-3,3));
	curFirework = (curFirework+1)%NFIREWORKS;
}

//--------------------------------------------------------------
void App::update()
{
	if( spawnTrigger.isTriggered() )
	{	
		// shoot!
		shoot();
	}
	
	static double time = 0.0;
	static double interval = ofRandom(1.0,3.0);
	
	if( autoShoot )
	{
		if( time > interval )
		{
			time = 0;
			interval = ofRandom(1.0,3.0);
			
			int n = 1 + (rand()%2);
			
			for( int i = 0; i < n; i++ )
				shoot();
		}
		
		time += ofGetLastFrameTime();
	}
	
	computeParticles();
}

//--------------------------------------------------------------
void App::draw()
{
	if(doFlashes)
	{
		float l=skyLight;
		glClearColor(l,l,l,l);
		skyLight*=(1.0-ofGetLastFrameTime()*10);
	}
	else
	{
		glClearColor(0,0,0,0);
	}
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	// additive blending
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
	static float roty = 0;
	roty+=rotySpeed*ofGetLastFrameTime();
	while(roty>360)
		roty-=360;
	while(roty<0)
		roty+=360;

	// setup perspective
	//setPerspectiveProjection( 45, (float)ofGetWidth()/ofGetHeight(), 0.1, 500 );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float) ofGetWidth()/ofGetHeight(), 0.1, 500.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,camDistance);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);

	glColor4f(1,1,1,1);
	
	// enable point sprite ext
	glEnable(GL_POINT_SPRITE_ARB);
	
	// this causes the points to scale with distance
	float quadratic[] =  { 1.0f, 0.0f, 0.01f };
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );
	
	ptex.getTextureReference().bind();
	glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	drawParticles();
	ptex.getTextureReference().unbind();
	glDisable(GL_POINT_SPRITE_ARB);
	 
}


void App::onParticleDead( Particle * particle, Emitter<Particle> * emitter )
{
	Firework * fw = (Firework*)emitter;
	if(particle->exploding && particle->hasTrail)
	{
		// sub explosion
		fw->explosionTrails = false;
		fw->explode(*particle,70,explosionPower*0.5);
	}
}

void App::onParticleAdded( Particle * particle, Emitter<Particle> * emitter )
{

}

void App::onEmitterEmpty( Emitter<Particle> * emitter )
{

}

void App::onParticleEvent( Particle * particle, Emitter<Particle> * emitter, int eventId )
{
	switch(eventId)
	{
		case 0: // main explosion
			skyLight += 0.3;
			break;
			
		case 1: // sub explosion
			//skyLight += 0.001;
			break;
			
		default:	
			break;
	}
}
	
//--------------------------------------------------------------
void App::keyPressed  (int key) 
{
	
}

//--------------------------------------------------------------
void App::keyReleased  (int key)
{
}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){
}


//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void App::windowResized(int w, int h){

}

