#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{

    myFont.loadFont("fonts/verdana.ttf", 24);
    ofBackground(0,0,0);

    if ( xml.loadFile("config.xml"))
    { 
        ofLog(OF_LOG_VERBOSE,"config.xml loaded");
    }
    else
    {
        ofLog(OF_LOG_ERROR,"Could not Found arconfig.xml");
    }

    bool bDebug = xml.getValue("SETTINGS:DEBUG",false);
    if (bDebug)
    {
        ofSetLogLevel(OF_LOG_VERBOSE);

    }
    else
    {
        ofSetLogLevel(OF_LOG_ERROR);

    }
    ofSetFullscreen(xml.getValue("SETTINGS:FULLSCREEN",false));
    ofHideCursor();
    int width = xml.getAttribute("SETTINGS:CANVAS","width",1920);
    int height = xml.getAttribute("SETTINGS:CANVAS","height",1080);
    gui.addSlider("timeout",timeout,0.00001f,10.0f);// = xml.getAttribute("SETTINGS:SCREENSAVER","timeout",1);
    gui.addSlider("maskA",maskA,1,255);


    ofSetWindowShape(width,height);
    ofSetWindowPosition(0,0);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    bSetup = false;

    loading.loadImage("images/loading.png");
    intro.loadImage("images/intro.png");

//    ofxDirList dir;
//    num = dir.listDir("movies/inst");
//    dir.setVerbose(true);
//    inst = new ofImage[num];
//    int count = 0 ;
//    for(int i = 0 ; i < num ; i ++)
//    {
//        size_t found = dir.getPath(i).find(".png");
//        if (found != string::npos)
//        {
//            inst[i].loadImage(dir.getPath(i));
//            count ++;
//        }
//    }
//    num = count;

    fadable.setAlpha(1);
    fadable.setFadeSeconds(1);
    bLoaded = player.loadMovie(xml.getAttribute("SETTINGS:SCREENSAVER","file","movies/instruction.mov"));
#ifdef INSTRUCTION

    secondImage.loadImage("images/green.jpg");


    shaderBGSub.loadShader("shaders/mix");



    if (bLoaded)
    {
        ofDisableArbTex();
        player.stop();
        player.setLoopState(OF_LOOP_NORMAL);
        vw = player.getWidth();
        vh = player.getHeight();
        secondImage.resize(vw,vh);


		unsigned char *imgPixels  = player.getPixels();

        myTexture.allocate(vw,vh,GL_RGB);
        myTexture.loadData(imgPixels, vw,vh, GL_RGB);


        unsigned char *imgPixels2 = secondImage.getPixels();
        myTexture2.allocate( vw , vh , GL_RGB);
        myTexture2.loadData(imgPixels2, vw , vh , GL_RGB);




        ///// multitextureLoad stuff
        glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, myTexture.getTextureData().textureID);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, myTexture2.getTextureData().textureID);


        // go back to default texture unit !!
        glActiveTexture(GL_TEXTURE0);

        ofEnableArbTex();


        gui.addSlider("Shader Tolorence",tol,0.0f,1.0f);
    }
    else
    {

        //std::exit(0);
        ofLog(OF_LOG_FATAL_ERROR,"Can not Load model instructioon movie!");
    }
#endif//#ifdef INSTRUCTION

}
void testApp::threadedFunction()
{
    while ( isThreadRunning() != 0 )
    {
        if ( lock() )
        {
            unlock();
            ofSleepMillis(1 * 1000);
        }

    }
}
//--------------------------------------------------------------
void testApp::update()
{
    //ARModelCollections *activeModel = NULL;


    if (ofGetFrameNum()==10)
    {


        if (!bSetup )
        {

            arhelper.setModelHandler(modelHandler);

            bSetup = arhelper.setup();

#ifdef USE_OA
            OverAll *oa = new OverAll();
            oa->setup();
            modelHandler.addModelCollection((ARModelCollections*)oa);
#endif
            xml.pushTag("SETTINGS");
            int tags = xml.getNumTags("MODELS");
            for (int i = 0 ; i <  tags ; i++)
            {
                int id = xml.getAttribute("MODELS","id",0,i);
                float scale = xml.getAttribute("MODELS","scale",1.0f,i);
                string fn = xml.getAttribute("MODELS","foldername","",i);
                OMAModel *omaModel = new OMAModel(id,scale,fn);
                omaModel->setup();
                glutSwapBuffers();
                modelHandler.addModelCollection((ARModelCollections*)omaModel);
            }
            /*
            #ifdef USE_MPLUS
                        MPlus *mplus = new MPlus();
                        mplus->setup();
                        modelHandler.addModelCollection((ARModelCollections*)mplus);
            #endif
            #ifdef USE_MV
                        MiddleVillage *mv = new MiddleVillage();
                        mv->setup();
                        modelHandler.addModelCollection((ARModelCollections*)mv);
            #endif
            #ifdef USE_TV
                        TheaterVillage *tv = new TheaterVillage();
                        tv->setup();
                        modelHandler.addModelCollection((ARModelCollections*)tv);
            #endif*/

            for (int  i = 0 ; i < modelHandler.getModel().size();i++)
            {
                ARModelCollections* armodel = modelHandler.getModel()[i];
                ofAddListener(armodel->MODEL_ACTIVE,this,&testApp::arModelActived);
                ofAddListener(armodel->MODEL_DEACTIVE,this,&testApp::arModelDeactived);
            }

            gui.loadFromXML();
            //minimap.enabled = true;
            fadable.fadeOut();
            timer.setup(60*timeout,false);
            ofAddListener(timer.TIMER_REACHED,this,&testApp::fadeIn);
            //bSetup = true;

        }

    }

    arhelper.update();
    ofSoundUpdate();
    fadable.updateFade();
    if (arhelper.isDetected())
    {

        timer.setup(timeout*60,false);
        timer.startTimer();
    }
    if (fadable.getAlpha()>0.9f)
    {
        if (bLoaded)
        {
            player.update();
#ifdef INSTRUCTION
			unsigned char *imgPixels = player.getPixels();
			myTexture.loadData(imgPixels, vw,vh, GL_RGB);

            glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, myTexture.getTextureData().textureID);


            // go back to default texture unit !!
            glActiveTexture(GL_TEXTURE0);
#endif//#ifdef INSTRUCTION
        }
    }


}

//--------------------------------------------------------------
void testApp::draw()
{
//	glActiveTexture(GL_TEXTURE0);
    if (bSetup)
    {

        arhelper.draw();
        if (bText)
        {

            glPushMatrix();
            ofSetColor(255 , 255, 255 );
            string msg = "";;
            myFont.drawString(msg, (ofGetWidth()/2)-myFont.stringWidth(msg)/2,ofGetHeight()/2);
            glPopMatrix();
        }
        ofSetupScreen();
        glDisable(GL_DEPTH_TEST);
        if (fadable.getAlpha()>0)
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(0,0,0,maskA*fadable.getAlpha());
            ofRect(0,0,ofGetWidth(),ofGetHeight());



            if (bLoaded)
            {
#ifdef INSTRUCTION
                int cx = ofGetWidth()/2;
                int cy = ofGetHeight()/2;
                ofSetColor(255,255,255);

                shaderBGSub.setShaderActive(true);
                shaderBGSub.setUniform("tex1", 1);
                shaderBGSub.setUniform("tex2", 2);
                shaderBGSub.setUniform("tol", tol);
                player.draw(cx-vw/2,cy-vh/2);


                myTexture2.unbind();
                myTexture.unbind();

                shaderBGSub.setShaderActive(false);
                glDisable(GL_DEPTH_TEST);
#endif //#ifdef INSTRUCTION
            }

            ofDisableAlphaBlending();
            ofPopStyle();
        }
		ofSetColor(0xffffff);
        gui.draw();

    }
    else
    {
        ofEnableAlphaBlending();
        ofSetColor(255,255,255,255*sinf(ofGetElapsedTimef())+125);
        loading.draw(0,0);
        ofDisableAlphaBlending();
        ofDrawBitmapString("Loading ... ",ofGetWidth()/2,ofGetHeight()/2);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{


    switch (key)
    {
    case 's':
        arhelper.videoSettings();
        break;
    case 'd':
        arhelper.toggleDebug();
        break;
    case 'f':
        ofToggleFullscreen();
        break;
    case ' ':
        gui.toggleDraw();
        if (gui.isOn())
        {
            ofShowCursor();
        }
        else
            ofHideCursor();
        break;
    case '[':
        gui.nextPage();
        break;
    case ']':
        gui.prevPage();
        break;
    case '`':
        gui.nextPageWithBlank();
        break;
        /* case '=':
             scale+=scale*0.01f;
             for (int i = 0 ; i < numModels ; i ++)
             {
                 models[i]->setScale(scale,scale,scale);
             }
             break;
         case '-':
             scale-=scale*0.01f;
             for (int i = 0 ; i < numModels ; i ++)
             {
                 models[i]->setScale(scale,scale,scale);
             }
             break;
         case 'l':
             glEnable (GL_LIGHTING);
             break;
         case 'k':
             glDisable (GL_LIGHTING);
             break;*/
    case 'm':
        arhelper.toggleMirror();
        break;
        /*case 't':
            bMove = !bMove;
            unsigned delay = 0;
            unsigned duration = 1000;
            int target = 100;
            if (bMove)tweenlinear.setParameters(7,easinglinear,ofxTween::easeOut,0,target,duration,delay);
            else tweenlinear.setParameters(7,easinglinear,ofxTween::easeIn,target,0,duration,delay);
            break;*/
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{


}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

void testApp::fadeIn(ofEventArgs &e)
{

    fadable.fadeIn();
    if (bLoaded)
    {
#ifdef TARGET_WIN32
        player.setFrame(1);
#endif
        player.setPaused(false);

    }
    timer.stopTimer();
    //minimap.setCandidate(-1);
    //minimap.enabled = false;
    //minimap.enabled = false;
    ofLog(OF_LOG_VERBOSE,"testApp Timer Event --- fade In screen saver");

}
void testApp::arModelActived(ofAREventArgs &e)//armodels active
{
    //minimap.enableAppEvents();
    fadable.fadeOut();
    if (bLoaded)player.setPaused(true);
    //minimap.setCandidate(e.markerId);
    //minimap.enabled = true;
    timer.setTimer(60*timeout);
    timer.startTimer();
    ofLog(OF_LOG_VERBOSE,"testApp AR Marker Event --- fade Out screen saver");
    bText = false;

}
void testApp::arModelDeactived(ofAREventArgs &e)//armodel deactive
{
    //minimap.enabled = false;
    bText = true;
}
