/*
 *  ofxFFGLPlugin.h
 *
 *	Created by Daniel Berio 2010
 */

#ifndef _OFXFFGLPLUGIN
#define _OFXFFGLPLUGIN


#include "ofxFFGLApp.h"
#include "ofxFFGLWindow.h"
#include "FFGLPluginInfo.h"

#include <FFGL.h>
#include <FFGLLib.h>
#include "FFGLPluginSDK.h"


/// Plugin creation call. With Visual Studio __stdcall does not compile
#ifdef OF_FFGL_NOSTDCALL

#define DEFINE_FFGL_PLUGIN(  appType, minInputs, maxInputs )  \
DWORD   createPluginInstance(CFreeFrameGLPlugin **ppInstance) \
{  \
ofFFGLPlugin * plug = new ofFFGLPlugin(new appType(),minInputs,maxInputs); \
*ppInstance = plug; \
return FF_SUCCESS; \
}

#else

#define DEFINE_FFGL_PLUGIN(  appType, minInputs, maxInputs )  \
DWORD   __stdcall createPluginInstance(CFreeFrameGLPlugin **ppInstance) \
{  \
ofFFGLPlugin * plug = new ofFFGLPlugin(new appType(),minInputs,maxInputs); \
*ppInstance = plug; \
return FF_SUCCESS; \
}

#endif



class  ofFFGLPlugin: public CFreeFrameGLPlugin
{
public:
	ofFFGLPlugin( ofFFGLApp * app, int minInputs = 1, int maxInputs = 1 );
	~ofFFGLPlugin();
	
	void initParameters();
	
  	DWORD InitGL(const FFGLViewportStruct *vp);
	DWORD DeInitGL();


	DWORD	ProcessOpenGL(ProcessOpenGLStruct* pGL);
	void	setupInputTextures(ProcessOpenGLStruct* pGL);
	
	DWORD	SetTime(double time);
	
	DWORD	SetParameter(const SetParameterStruct* pParam);		
	DWORD	GetParameter(DWORD dwIndex);					
	char*	GetParameterDisplay(DWORD dwIndex);
	
protected:
	char _paramDisplay[128];
	
	bool isGLInitialized;
	
	ofFFGLApp *  	_app;
	ofFFGLWindow *  _ofWin;
	
private:
};

#endif
