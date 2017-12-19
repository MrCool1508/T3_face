#ifndef FACEENGINE_H
#define FACEENGINE_H

#include <QObject>
#include <QImage>

#include "asvloffscreen.h"
#include "arcsoft_fsdk_face_recognition.h"
#include "arcsoft_fsdk_face_tracking.h"

#define MAX_FT_FACE (50)

class FaceInfo
{
    unsigned char *pFeature;
    int featureSize;
    QString name;
    int id;
    FaceInfo(){
        pFeature = NULL;
        id = -1;
    }
};
class FaceEngine : public QObject
{

    Q_OBJECT
public:
    FaceEngine();
    ~FaceEngine();
    int getFaceFeatureFromImage(QImage image,
                                AFR_FSDK_FACEMODEL *_faceModels);

    int _faceNum_ = 0;
    int _uniqueIncID_ = 0;
    int _faceID_[MAX_FT_FACE];

private:
    void *_hFTEngine;
    void *_hFREngine;
    unsigned char *_FTWorkMem;
    unsigned char *_FRWorkMem;
    float _threshold = 0.0;
    bool _bReady = false;
    int _doFRFrameCount = 0;
};

#endif // FACEENGINE_H
