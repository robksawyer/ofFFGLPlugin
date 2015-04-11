#ifndef _PARTYCLES_APP
#define _PARTYCLES_APP

#include "ofxFFGLApp.h"

#include "Fireworks.h"

#define NFIREWORKS	15

class App : public ofFFGLApp, public ParticleListener<Particle>
{
	
	public:
		App();
		void setup();
		void update();
		void draw();
		
		void shoot();
		
		void computeParticles();
		void drawParticles();
		void drawFirework( Firework & f, float ptSize );
		
		void onParticleDead( Particle * particle, Emitter<Particle> * emitter );
		void onParticleAdded( Particle * particle, Emitter<Particle> * emitter );
		void onEmitterEmpty( Emitter<Particle> * emitter );
		void onParticleEvent( Particle * particle, Emitter<Particle> * emitter, int eventId );
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
		Trigger<bool>	spawnTrigger;
		
		float 		camDistance;
		Firework	f[NFIREWORKS];
		
		bool		doFlashes;
		bool		autoShoot;
		
		int			curFirework;
		float		rotx;
		float		rotySpeed;
		float		pointSize;
		
		float		spawnRadius;
		float		damping;
		float		explosionPower;
		float		explosionRandomness;
		float		explosionDuration;

		float		trailAlpha;
		
		float		skyLight;
		
		ofImage		ptex;		
};

#endif
	
