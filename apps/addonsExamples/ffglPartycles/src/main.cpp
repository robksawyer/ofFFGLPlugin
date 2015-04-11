#include "ofMain.h"

#include "ofxFFGLPlugin.h"

#include "App.h"


// Here is the main difference between a normal openFrameworks app and a OF FFGL plugin:

// This defines the creation function for our plugin
// parameters:

// appType:	 the class name of your App 
// minInputs: minimum number of inputs ( textures )
// maxInputs: maximum number of inputs ( textures )
DEFINE_FFGL_PLUGIN( App, 0, 0 );




// FFGL Plugin Info Structure

// Insert the info that will appear on the host here 
/*! 
	!!! Do not change fields marked with *      
*/

static CFFGLPluginInfo PluginInfo (
	createPluginInstance,					// * Create method
	"OFPX",									// Plugin unique ID
	"FireWorks",								// Plugin name
	1,										// * API major version number
	500,									// * API minor version number
	1,										// Plugin major version number
	000,									// Plugin minor version number
	FF_SOURCE,								// Plugin type FF_SOURCE or FF_EFFECT ( filters ) 
	"Firework Particles Plugin (Made with OpenFrameworks)",			// Plugin description
	"by Daniel Berio for Resolume"		// About
	,0
	,NULL
);



