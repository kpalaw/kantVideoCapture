#include <iostream>
#include <string>
#include <regex>

class paramExtract{
  private:
    std::string argumentPack = "";
    std::string mediaFileName= "";
    const int frameCaptureInit = -1;
    int frameCaptureStart = frameCaptureInit;// frame number
    int frameCaptureStop = frameCaptureInit;
    const int maxFrame =10000000;
    int playStart = maxFrame; //in sec
    std::smatch match;
    bool paramValidity = true;

  public:
  paramExtract(int argc, char* argv[]);
  std::string getMediaFileName();
  int getFrameCaptureStart();
  int getFrameCaptureStop();
  int getPlayStart();
  bool hasParamInputFile();
  bool hasParamFile();

  bool hasParamFrameCapture();
  bool hasParamFrameCaptureRange();
  bool hasParamPlay();
  void printInfo();
  bool hasMediaName();
  bool hasFrameCaptureStart();
  bool hasFrameCaptureStop();
  bool hasPlayStart();
  bool isAlphaString(std::string str);
  bool isNumericString(std::string str);

};