/*
 *  ofxFFGLParameter.cpp
 *  FFGLExample
 *
 *  Created by ensta on 2/4/10.
 *  Copyright 2010 Daniel Berio. All rights reserved.
 *
 *  http://www.enist.org
 */

#include "ofxFFGLParameter.h"

ofFFGLParameter::ofFFGLParameter()
{
	_type = PARAM_UNKNOWN;
	_addr = 0;
	_min = 0.0f;
	_max = 1.0f;
	_name = "unknown";
}

ofFFGLParameter::~ofFFGLParameter()
{
}



void ofFFGLParameter::initFloat( const char * name, float * addr, float min , float max  )
{
	_addr = addr;
	_type = PARAM_FLOAT;
	_name = name;
	_min = min;
	_max = max;
}

void ofFFGLParameter::initBool( const char * name, bool * addr )
{
	_addr = addr;
	_type = PARAM_BOOL;
	_name = name;
}

void ofFFGLParameter::initEvent( const char * name, bool * addr )
{
	_addr = addr;
	_type = PARAM_EVENT;
	_name = name;
}

void ofFFGLParameter::initString( const char * name, std::string * addr )
{
	_addr = addr;
	_type = PARAM_STRING;
	_name = name;
}

void ofFFGLParameter::initCString( const char * name, char * addr )
{
	_addr = addr;
	_type = PARAM_CSTRING;
	_name = name;
}

void ofFFGLParameter::setFloat( float val )
{
	if( _type != PARAM_FLOAT )
		return;
		
	*((float*)_addr) = val;
}

void ofFFGLParameter::setBool( bool val )
{
	if( _type != PARAM_BOOL && _type != PARAM_EVENT )
		return;

	*((bool*)_addr) = val;
}

void ofFFGLParameter::setString( const char * val )
{
	switch( _type )
	{
		case PARAM_STRING:
		{
			std::string * str = (std::string*)_addr;
			*str = val;
			break;
		}
		
		case PARAM_CSTRING:
		{
			strcpy((char*)_addr, val);
			break;
		}
		
		default:
			break;
	}
}


float ofFFGLParameter::getFloat()
{
	if( _type != PARAM_FLOAT )
		return 0.0f;
	
	return *((float*)_addr);
}

bool  ofFFGLParameter::getBool()
{
	if( _type != PARAM_BOOL && _type != PARAM_EVENT )
		return false;
	
	return *((bool*)_addr);
}

const char* ofFFGLParameter::getString()
{
	switch( _type )
	{
		case PARAM_STRING:
		{
			std::string * str = (std::string*)_addr;
			return str->c_str();
		}
		
		case PARAM_CSTRING:
		{
			return (const char*)_addr;
		}
		
		default:
			return 0;
			// this could return a string version of parameter
	}
}
