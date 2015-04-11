#include "ofMain.h"

#include "ofxFFGLPlugin.h"

#include "testApp.h"


// Here is the main difference between a normal openFrameworks app and a OF FFGL plugin:

// This defines the creation function for our plugin
// parameters:

// appType:	 the class name of your App 
// minInputs: minimum number of inputs ( textures )
// maxInputs: maximum number of inputs ( textures )
DEFINE_FFGL_PLUGIN( testApp, 0, 0 );




// FFGL Plugin Info Structure

// Insert the info that will appear on the host here 
/*! 
	!!! Do not change fields marked with *      
*/

static CFFGLPluginInfo PluginInfo (
	createPluginInstance,					// * Create method
	"OFEX",									// Plugin unique ID
	"OFSourceExample",								// Plugin name
	1,										// * API major version number
	500,									// * API minor version number
	1,										// Plugin major version number
	000,									// Plugin minor version number
	FF_SOURCE,								// Plugin type FF_SOURCE or FF_EFFECT ( filters ) 
	"Test OpenFrameworks Plugin",			// Plugin description
	"by Daniel Berio - www.enist.org"		// About
	,0
	,NULL
);



