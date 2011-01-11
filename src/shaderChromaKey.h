/*
 *  shaderChromaKey.h
 *  openFrameworks
 *
 *  Created by theo on 17/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxShader.h"
#include "ofxFBOTexture.h"

class shaderChromaKey{

	public:
		void setup(int fboW, int fboH);

		void beginRender();
		void endRender();

		void setTolParams(float tol);
		void setColor(int r,int g,int b);
		void draw(float x, float y, float w, float h, bool useShader);

		ofxShader shaderCK;



		ofxFBOTexture fbo1,fbo2;

		float tol;

		int _r,_g,_b;


};
