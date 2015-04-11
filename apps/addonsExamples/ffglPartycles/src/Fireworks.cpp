/*
 *  Fireworks.cpp
 *
 *  Created by Daniel Berio on 12/1/10.
 *  http://www.enist.org
 *  Copyright 2010. All rights reserved.
 *
 */


#include "Fireworks.h"

#define frand ofRandom
#define TWOPI M_TWO_PI

///////////////////////////////////////////////////////////////////////
// some util functions

#ifndef GL_MAKERGBA
#define GL_MAKERGBA(r,g,b,a) ((unsigned int)((((unsigned char)(a*255.0)&0xff)<<24)|(((unsigned char)(b*255.0)&0xff)<<16)|(((unsigned char)(g*255.0)&0xff)<<8)|((unsigned char)(r*255.0)&0xff)))
#endif

unsigned int getHSV( double h, double s, double v, double alpha );

float wrap(float val, float limit)
{
	while(val>limit)
		val-=limit;
	while(val<0.0f)
		val+=limit;
	return val;
}


///////////////////////////////////////////////////////////////////////


Vec3 Firework::gravity = Vec3(0,6.05,0);
bool Firework::blink = true;


///////////////////////////////////////////////////////////////////////


Firework::Firework()
:
exploding(false),
explosionTrails(true)
{
	spawnRadius = 3.0;
	damping = 0.1;
	explosionPower = 70;
	explosionRandomness = 1.0;
	explosionDuration = 3.0;
}

Firework::~Firework()
{
}


///////////////////////////////////////////////////////////////////////


Particle * Firework::addParticle( const Vec3 & pos, 
						const Vec3 & vel,
						const Vec3 & accel,
						float lifetime )
{
	Particle * p = pool.add();
	if(!p)
		return 0;
		
	p->pos = p->oldpos = pos;
	p->accel = accel;
	p->vel = vel;
	p->lifetime = lifetime;
	p->t = 0;
	
	return p;
}


///////////////////////////////////////////////////////////////////////


void Firework::init( int numParticles )
{
	// allocate default particle pool
	pool.allocate(numParticles);
	// allocate pool for trails
	trailPool.allocate(numParticles*2);
}


///////////////////////////////////////////////////////////////////////


void Firework::spawn( float x, float y, float z )
{
	pool.removeAll();
	
	// if explosion has trails it will trigger other small explosions after...
	explosionTrails = (frand(0,100)>50)?true:false;
	
	Vec3 accel;
	
	// random vertical direction in a cone....
	float ang = frand(0,TWOPI);
	accel.z = sin( ang )*spawnRadius;
	accel.x = cos( ang )*spawnRadius;
	accel.y = 1.0;
	accel.y *= frand(20,30);//explosionPower,explosionPower*3.0);
	
	Particle *p = addParticle(	Vec3(x,y,z), // pos
								Vec3(0,0,0), // velocity
								accel, // accel
								frand(1.0,1.5) // lifetime
							 );
	p->exploding = false;
	p->hasTrail = true;
	
	if( frand(0,100) > 30 )
		p->hue = wrap(frand(311,311+120),360); // yellow-red fireworks
	else
		p->hue = wrap(frand(200,270),360); // blue fireworks
		
	p->color = getHSV( p->hue, 1.0, 1.0, 1.0f );
	
	exploding = false;
}


///////////////////////////////////////////////////////////////////////


void Firework::explode( Particle & initialParticle, int numExplosionParticles, float power )
{
	// randomize stuff
	explosionType = rand()%2;
	
	Vec3 pos = initialParticle.pos;
	Vec3 vel = initialParticle.vel;
	int hue = initialParticle.hue;
	
	exploding = true;
	
	int n = pool.getNumParticles() + numExplosionParticles;
	if( n > pool.getMaxParticles() )
		n = pool.getMaxParticles();

	for( int i = pool.getNumParticles(); i < n; i++ )
	{
		Particle &p=pool[i];
		p.pos = pos;
		p.oldpos = pos;
		p.accel = Vec3(frand(-1,1),frand(-1,1),frand(-1,1));
		p.accel.normalize();
		p.accel *= power * (1.0 + explosionRandomness*frand(0.5,2.0)); 
		
		if(explosionTrails)
			p.hasTrail = true;
		else
			p.hasTrail = false;
			
		p.exploding = true;
		p.vel = vel*0.5;
		
		p.lifetime = explosionDuration*(frand(0.5,2.0));
		p.t = 0.0;
		
		p.hue = hue;
		p.color = getHSV( p.hue, 1.0, 1.0, 1.0f );
	}
	
	pool.setNumParticles(n);
}


///////////////////////////////////////////////////////////////////////


void Firework::addTrail( Particle & missile, double dt )
{
	// add trail
	Particle trail = missile;
	//trail.accel*=0.1;
	trail.accel = Vec3(0,0,0);
	trail.vel *= 0.3;
	trail.t  = 0;
	trail.lifetime = 0.2;
	trail.exploding = false;
	trail.hasTrail = false;
	trail.hue = missile.hue;
	
	trailPool.add(trail);
}

///////////////////////////////////////////////////////////////////////

inline void Firework::integrate( Particle & p, double dt, double damp )
{
	p.oldpos = p.pos;
	p.vel += (p.accel-gravity)*dt;
	p.pos += p.vel*dt;
	p.accel *= (1.0-damp*dt);
	
	p.t += dt;
}


///////////////////////////////////////////////////////////////////////


void Firework::integrate( double msecs )
{
	double dt = msecs*0.001;
	
	// missile mode
	if( !exploding && pool.getNumParticles() )
	{
		Particle & missile = pool[0];

		integrate(missile,dt,damping*0.1);
		
		addTrail(missile,dt);
		
		// when missile starts descending explode it
		if( missile.vel.y < 0 || missile.t > missile.lifetime)
		{
			if( explosionTrails )
			{
				explode(missile,10,explosionPower);
				particleEvent(&missile,1);
			}
			else
			{
				explode(missile,2000,explosionPower);
				particleEvent(&missile,0);
			}
		}
	}
	else
	// explosion mode
	{
		for( int i = 0; i < pool.getNumParticles(); i++ )
		{
			Particle & p = pool[i];
			
			integrate(p,dt,damping );
			
			float t = (p.t/p.lifetime);
			
			// this keeps em lit for a while...
			t = pow(t,7);
			t = 1.0-t;
			
			float blinky = t;
			if(blink)
				blinky *= frand(0.1,1.0);
				
			p.color = getHSV( p.hue,
							  1.0-t*0.5,
							  1.0, 
							  blinky // blinky alpha
							 );
			
			double lifetime = p.lifetime;
			
			// kill particle earier if it has a trail ( it's a missile! )
			if(p.hasTrail)
				lifetime*=0.5;
				
			if( p.t > lifetime )
			{
				particleDead( &p );
				
				// If the particle is removed, another particle will be swapped in,
				// so we need to repeat the calculations for the same index
				if( pool.removeAndSwap(i) ) 
					i = i-1;
			}
		}
		
		// add trails
		if( pool.getNumParticles() )
		{
			for( int i = 0; i < pool.getNumParticles(); i++ )
			{
				if( pool[i].hasTrail )
					addTrail( pool[i], dt );
			}
				
				
			//printf("right now there are %d normal particles\n",pool.getNumParticles());
		}
	}
	
	// update trails
	Particle * trails = trailPool.getParticles();
	for( int i = 0; i < trailPool.getNumParticles(); i++ )
	{
		Particle & p = trails[i];
		integrate(p,dt,damping);
		
		float t = 1.0-(p.t/p.lifetime);
		p.color = getHSV( p.hue,
						  1.0-t,
						  1.0, 
						  t 
						 );

		if( p.t > p.lifetime )
		{
			if( trailPool.removeAndSwap(i) )
			{
				i = i-1;
			}
		}
	}
	
	if( pool.getNumParticles() == 0 && trailPool.getNumParticles() == 0)
	{
		emitterEmpty();
	}
}



void HSVtoRGB(double h, double s, double v, double* r, double* g, double* b)
{

  if ( s == 0 )

  {

     *r = v;

     *g = v;

     *b = v;

  } else {

     double var_h = h * 6;

     double var_i = floor( var_h );
     
	 double var_1 = v * ( 1 - s );
     
	 double var_2 = v * ( 1 - s * ( var_h - var_i ) );
     
	 double var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );
 

     if      ( var_i == 0 ) { *r = v     ; *g = var_3 ; *b = var_1; }

     else if ( var_i == 1 ) { *r = var_2 ; *g = v     ; *b = var_1; }

     else if ( var_i == 2 ) { *r = var_1 ; *g = v     ; *b = var_3; }

     else if ( var_i == 3 ) { *r = var_1 ; *g = var_2 ; *b = v;     }

     else if ( var_i == 4 ) { *r = var_3 ; *g = var_1 ; *b = v;     }

     else                   { *r = v     ; *g = var_1 ; *b = var_2; }

 

  }

}


unsigned int getHSV( double h, double s, double v, double alpha )
{
	static double inv = 1.0/360.0;
	double r,g,b;
	HSVtoRGB(h*inv,s,v,&r,&g,&b);
	return GL_MAKERGBA(r,g,b,alpha);
}
