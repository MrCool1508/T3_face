#include "FaceEngine.h"
#include "t3_log.h"
#include "key.h"

FaceEngine::FaceEngine()
{
    _doFRFrameCount = 0;
    _bReady = false;
    _threshold = 0.5f;

    _faceNum_ = 0;
    for(int k=0;k<MAX_FT_FACE;k++){
        _faceID_[k] = -1;
    }

    _uniqueIncID_ = 0;

    int ftWorkmemSize_ = 20 * 1024 * 1024;
    _FTWorkMem = new unsigned char[ftWorkmemSize_];
    int frWorkmemSize = 40 * 1024 * 1024;
    _FRWorkMem = new unsigned char[frWorkmemSize];

    _hFTEngine = nullptr;
    _hFREngine = nullptr;
    int ret = AFT_FSDK_InitialFaceEngine((MPChar)APPID, (MPChar)FT_SDKKEY, _FTWorkMem, ftWorkmemSize_, &_hFTEngine, AFT_FSDK_OPF_0_HIGHER_EXT, 16, MAX_FT_FACE);
    if(ret != 0){
        T3LOG <<"fail to AFT_FSDK_InitialFaceEngine():"<<ret;
        return;
    }
    ret = AFR_FSDK_InitialEngine((MPChar)APPID, (MPChar)FR_SDKKEY, _FRWorkMem, frWorkmemSize, &_hFREngine);
    if(ret != 0){
        qDebug() <<"fail to AFR_FSDK_InitialEngine():"<<ret;
        return;
    }

    _bReady = true;
}
FaceEngine::~FaceEngine()
{
    if(_hFTEngine)
    {
        AFT_FSDK_UninitialFaceEngine(_hFTEngine);
    }
    if(_hFREngine)
    {
        AFR_FSDK_UninitialEngine(_hFREngine);
    }

    if(_FTWorkMem)
    {
        delete[] _FTWorkMem;
        _FTWorkMem = nullptr;
    }
    if(_FRWorkMem)
    {
        delete[] _FRWorkMem;
        _FRWorkMem = nullptr;
    }
}

int FaceEngine::getFaceFeatureFromImage(QImage image,AFR_FSDK_FACEMODEL
                                        *_faceModels)
{

    int height_ = image.height();
    int width_ = image.width();

    unsigned char *frameData_ = new unsigned char [height_*width_*3];
    int frameFromat_ = ASVL_PAF_RGB24_B8G8R8;
    int pixelIndex_ = 0;
    for(int i =0 ;i<width_;i++)
    {
        for(int j = 0;j<height_;j++)
        {
            QColor color_ = image.pixelColor(i,j);
            *(frameData_ + pixelIndex_) = color_.blue();
            *(frameData_ + pixelIndex_ + 1) = color_.green();
            *(frameData_ + pixelIndex_ + 2) = color_.red();
            pixelIndex_ += 3;
        }
    }
    ASVLOFFSCREEN inputImg_ = { 0 };
    inputImg_.i32Height = width_;
    inputImg_.i32Width = height_;
    inputImg_.pi32Pitch[0] = height_*3;
    inputImg_.ppu8Plane[0] = frameData_;
    inputImg_.u32PixelArrayFormat = frameFromat_;
    LPAFT_FSDK_FACERES faceResult_ = nullptr;
    int ret = AFT_FSDK_FaceFeatureDetect(_hFTEngine,&inputImg_,&faceResult_);
    T3LOG << faceResult_->nFace;
    if(ret == 0)
    {
        T3LOG << "detect the faceFeature";
    }
    AFR_FSDK_FACEINPUT faceInput_;
    faceInput_.lOrient = faceResult_->lfaceOrient;
    faceInput_.rcFace.left = faceResult_->rcFace->left;
    faceInput_.rcFace.bottom = faceResult_->rcFace->bottom;
    faceInput_.rcFace.right = faceResult_->rcFace->right;
    faceInput_.rcFace.top = faceResult_->rcFace->top;

    return AFR_FSDK_ExtractFRFeature(_hFREngine,&inputImg_,&faceInput_,_faceModels);
}
