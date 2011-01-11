/*
 *  shaderChromaKey.cpp
 *  openFrameworks
 *
 *  Created by theo on 17/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "shaderChromaKey.h"

//--------------------------------------------------------------
void shaderChromaKey::setup(int fboW, int fboH){

	ofBackground(255,255,255);
	ofSetVerticalSync(true);

	fbo1.allocate(fboW, fboH, true);
	fbo2.allocate(fboW, fboH, true);

	shaderCK.loadShader("shaders/chromaKey");



	tol = 2.0;
	_r = 0;
	_g = 255;
	_b = 0;
}

//--------------------------------------------------------------
void shaderChromaKey::beginRender(){
	fbo1.swapIn();

}

//--------------------------------------------------------------
void shaderChromaKey::endRender(){
	fbo1.swapOut();
}

//--------------------------------------------------------------
void shaderChromaKey::setTolParams( float _tol){

	tol	= _tol;
}
void shaderChromaKey::setColor(int r,int g,int b){
	_r = r;
	_g = g;
	_b = b;
}

//--------------------------------------------------------------
void shaderChromaKey::draw(float x, float y, float w, float h, bool useShader){

	ofxFBOTexture * src, * dst;
	src = &fbo1;
	dst = &fbo2;

	if( useShader ){
			shaderCK.setShaderActive(true);
			shaderCK.setUniform("tol", tol);
			shaderCK.setUniform("color", _r/255.0f,_g/255.0f,_b/255.0f,1.0);

			dst->swapIn();

			src->draw(0, 0);
			dst->swapOut();

			shaderCK.setShaderActive(false);
	}

	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255);
	dst->draw(x, y, w, h);

}



