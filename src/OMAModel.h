#ifndef OMAMODEL_H
#define OMAMODEL_H

#include "ARModelCollections.h"

#ifdef TARGET_WIN32
#ifdef USE_GSTREAMER
#include "ofVideoPlayerGst.h"
#endif//USE_GSTREAMER
#else if TARGET_OSX
//#include "ofxQTKitVideoTexture.h"
#endif //TARGET_WIN32

#include "shaderChromaKey.h"
class OMAModel : public ARModelCollections
{
public:
    OMAModel(int _id,float _scale, string _folder);
    virtual ~OMAModel();
    void selfUpdate();
    void selfSetup();
    void drawModel();
    void draw2D();
    void findScreenCoord( const GLdouble *proj, const GLint *view);
    void fadeIn();
    void fadeOut();

protected:
private:

#ifdef TARGET_WIN32

    #ifdef USE_GSTREAMER
        ofVideoPlayerGst player;
    #else
        ofVideoPlayer player;
    #endif //USE_GSTREAMER
#else if TARGET_OSX
    //ofxQTKitVideoTexture player;
    ofVideoPlayer player;
#endif //TARGET_WIN32


    bool bLoaded;
    int w,h;


    ofxShader shaderBGSub;

    ofTexture myTexture,myTexture2;

    ofImage secondImage;
    ofPoint vidPoint[4];
    float tol;
    int z;
    ofPoint *points;
    ofPoint *modelPnts;
    //float pointZ;

};

#endif // OMAMODEL_H
