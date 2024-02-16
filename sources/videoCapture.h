#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>

#include <png.h>
#include "progressBar.h"

class videoCapture{
  private:
    std::string vidFileName;
    AVFormatContext* formatContext;
    AVCodecContext* codecContext;
    AVCodec* codec;
    SwsContext* swsContext;

    int lastFrame;
    int width;
    int hight;
    bool flagWindow = false;
    std::string nameOfEncoder;
    std::string title;
    std::string pixelFormat;
    std::map<std::string, std::string> vidMetaData;
    AVDictionaryEntry* metaDataEntry = nullptr;
    int videoStreamIndex;

    int frameCaptureStart = 0;
    int frameCaptureStop = 0;
    int framePlayStart = 0;
    progressBar* pgBar;
  public:
    videoCapture();
    videoCapture(std::string vidFile);

    void pauseToggle();
    int vidInit(std::string vidFile);
    int capturVideo(std::string vidFile, int frameCapStart, int frameCapStop);
    void playVideo(std::string vidFile, int frameCap, int framePlaySt);
    int frameToPng(int frameNumber);

    void saveFrameAsPNG(const char* filename, AVFrame* frame, int height);
    void showInfo();
};

#endif