#ifndef _ofxFFGLWINDOW_H
#define _ofxFFGLWINDOW_H

#include "ofAppBaseWindow.h"
#include "ofAppGLFWWindow.h"

// alot of code adapted from ofQC by vade, thanx!

class ofFFGLWindow : public ofAppGLFWWindow // ofAppBaseWindow //
{
public:
		ofFFGLWindow();
		//virtual ofFFGLWindow(){};
		
		void	setupOpenGL(int w, int h, int screenMode);
		void	initializeWindow();
		void	runAppViaInfiniteLoop(ofBaseApp * appPtr);
			
		void	hideCursor() {};
		void	showCursor() {};
		
        int     getHeight();
        int     getWidth();
    
		void    setWindowPosition(int x, int y);
		void	setWindowShape(int w, int h);
			
		int		getFrameNum();
		float	getFrameRate();
		double	getLastFrameTime();
		
		ofPoint	getWindowPosition(); 
		ofPoint	getWindowSize();
		ofPoint	getScreenSize();
		
		void	setFrameRate(float targetRate);
		void	setWindowTitle(string title){}
		
		int		getWindowMode();
		
		void	setFullscreen(bool fullscreen){}
		void	toggleFullscreen(){}
		
		void	enableSetupScreen(){}
		void	disableSetupScreen(){}
	
		// additional methods not defined in ofAppBaseWindow
		
		void		update();
		void		draw();

		int					windowMode;
		ofPoint				screenSize;
		ofPoint				windowSize;
		ofPoint				windowPos;
	
		float				timeNow, timeThen, fps;
		int					nFrameCount;
		bool				bEnableSetupScreen;
		float				frameRate;	
		double			lastFrameTime;

};


#endif
