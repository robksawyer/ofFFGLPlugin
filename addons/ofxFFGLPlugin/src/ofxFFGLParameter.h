/*
 *  ofxFFGLParameter.h
 *  FFGLExample
 *
 *  Created by ensta on 2/4/10.
 *  Copyright 2010 Daniel Berio. All rights reserved.
 *  http://www.enist.org
 *
 */

#ifndef _OFXFFGLPARAMETER
#define _OFXFFGLPARAMETER


#include <string>

using namespace std;


// TODO This stuff could be totaly disconnected from FFGL & OF

enum FFGL_PARAMETER_TYPE
{
	PARAM_FLOAT = 0,
	PARAM_BOOL,
	PARAM_STRING,
	PARAM_CSTRING,
	PARAM_EVENT,
	PARAM_UNKNOWN  = -1
};

class ofFFGLParameter
{
public:
	ofFFGLParameter();
	~ofFFGLParameter();
	
	////////////////////////////////////////////
	// initialization funcs
	
	void initFloat( const char * name, float * addr, float min = 0.0f, float max = 1.0f );
	void initBool( const char * name, bool * addr );
	void initCString( const char * name, char * addr );
	void initString( const char * name, std::string * addr );

	/// This is actually a boolean value, so use getBool and setBool to manipulate it.
	/// See Trigger struct at end of file for a easy way to use it.
	void initEvent( const char * name, bool * addr );

	////////////////////////////////////////////
	// set the value of parameter
	
	void setFloat( float val );
	void setBool( bool val );
	void setString( const char * val );
	
	////////////////////////////////////////////
	
	float getFloat();
	bool  getBool();
	const char* getString();
	
	/// get float value in 0.0-1.0 range
	float getFloat01() {
		float f = getFloat();
		return (f-_min) / (_max-_min);
	}
	
	
	float getMin() const { return _min; }
	float getMax() const { return _max; }
	void *getAddress() { return _addr; }
	
	FFGL_PARAMETER_TYPE getType() const { return _type; }
	
	const char * getName() const { return _name.c_str(); }
	
protected:
	string _name;
	void * _addr;
	
	// just for float parameters
	float _min;
	float _max;

	FFGL_PARAMETER_TYPE _type;
	
};


/////////////////////////////////////////////////////
/// Utility class for testing if a value changed
/// This saves some work when a value needs to be always tested for a change, as in FFGL event parameters.

// Example usage:
//
// In class definition:
//		Trigger<bool> myEvent;

// In constructor:
//		addEvent("myEvent",&myEvent.val);

// In update loop:
//		if(myEvent.isTriggered())
//			doSomething();

template <class Type> struct Trigger
{
	Trigger() { oldVal = val = 0; }
	
	bool isTriggered()
	{
		bool res = false;
		if( oldVal != val )
		{
			res = true;
			oldVal = val;
		}
		
		
		return res;
	}
	
	void reset()
	{
		oldVal = val;
	}
	
	Type oldVal;
	Type val;
};




#endif

