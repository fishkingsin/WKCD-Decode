#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ARHelper.h"
#include "ofx3DModelLoader.h"
#ifdef TARGET_WIN32
#ifdef USE_GSTREAMER
#include "ofVideoPlayerGst.h"
#endif//USE_GSTREAMER
#else if TARGET_OSX
//#include "ofxQTKitVideoTexture.h"
#endif //TARGET_WIN32

#include "ofxVBO.h"

#include "global.h"

#include "OMAModel.h"

//#define USE_MPLUS
//#define USE_OA
//#define USE_MV
//#define USE_TV
#ifdef USE_MV
#include "MiddleVillage.h"
#endif //USE_MV
#ifdef USE_TV
#include "TheaterVillage.h"
#endif//USE_TV
#ifdef USE_MPLUS
#include "MPlus.h"
#endif//USE_MPLUS
#ifdef USE_OA
#include "OverAll.h"
#endif//USE_OA

#define DEBUG
#define  INSTRUCTION
class testApp : public ofBaseApp , public ofxThread
{

public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void threadedFunction();


    void fadeIn(ofEventArgs &e);
    void arModelActived(ofAREventArgs &e);
    void arModelDeactived(ofAREventArgs &e);


    ARHelper arhelper;
    ofx3DModelLoader model;
    ofImage loading;
    ofImage intro;
    ofxFadableBase fadable;
    ModelHandler modelHandler;

#ifdef TARGET_WIN32

#ifdef USE_GSTREAMER
    ofVideoPlayerGst player;
#else
    ofVideoPlayer player;
#endif //USE_GSTREAMER
#else if TARGET_OSX
    ofVideoPlayer player;


#endif //TARGET_WIN32
    ofxTimer timer;

    ofxXmlSettings xml;

    bool bSetup;
    int vw,vh;
    float tol;
    float timeout;
    float maskA;
    bool bLoaded;
    bool bText;
#ifdef INSTRUCTION
    ofxShader shaderBGSub;

    ofTexture myTexture,myTexture2;

    ofImage secondImage;
#endif
    ofTrueTypeFont myFont;

};

#endif
