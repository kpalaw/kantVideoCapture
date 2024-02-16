#include "paramExtract.h"
paramExtract::paramExtract(int argc, char* argv[]){
  for (int i=1; i<=argc-1; i++){
    
    argumentPack += argv[i];
    argumentPack += " ";
  }
  hasParamInputFile();
  //hasParamFile();
  hasParamFrameCapture();
  //hasParamFrameCaptureRange();
  //hasParamPlay();
}

std::string paramExtract::getMediaFileName(){
  return mediaFileName;
}
int paramExtract::getFrameCaptureStart(){
  return frameCaptureStart;
}
int paramExtract::getFrameCaptureStop(){
  return frameCaptureStop;
}
int paramExtract::getPlayStart(){
  return playStart;
}


bool paramExtract::hasParamInputFile(){//eg. kantVid -i imput.mp4
auto re10 = std::regex("(-i)");
auto re12 = std::regex("(-i) +([^ \n]+)");

  if (std::regex_search(argumentPack, match, re10)){
    if(std::regex_search(argumentPack, match, re12)){
        mediaFileName += match[2].str();
        return true;
      }else{
        std::cout << "-i but invalid file name" << std::endl;
        return false;
      }
  }else{
    if (mediaFileName==""){
        std::cout << "require input: -i missing" << std::endl;
        return false;
    }
  }
  
}

bool paramExtract::hasParamFile(){//eg. kantVid input.mpv
  auto re2 = std::regex("( +[a-zA-Z0-9.]+)");
  if(std::regex_search(argumentPack, match, re2)){
    if (mediaFileName == ""){
      mediaFileName += match[0].str();
      return true;
    }else{
      return false;
    }
    
  }else{
    return false;
  }
}

bool paramExtract::hasParamFrameCapture(){
std::string paramText = "";
auto re30 = std::regex("(-fc)");
auto re31 = std::regex("(-fc) +([^- \n]+)");
auto re32 = std::regex("(-fc) +([0-9]+)");
  if (std::regex_search(argumentPack, match, re30)){
    if(std::regex_search(argumentPack, match, re31)){
      paramText = match[2].str();
      if(std::regex_search(argumentPack, match, re32)){
        if (hasMediaName()){
          if (paramText != match[2].str()){
            std::cout << "-fc can not capture frame -invalid frame number" << std::endl;
            return false;
          }else{
            frameCaptureStart = atoi(match[2].str().c_str());
            frameCaptureStop  = frameCaptureStart;
            
            return true;
          }
                      
        }else{
          return false;
        }
      }else{
        return false;
      }

    }else{
      std::cout << "-fc without frame number" << std::endl;
      return false;
    }
  }else{
    return false;
  }
  
}
bool paramExtract::hasParamFrameCaptureRange(){
std::string paramText = "";
auto re30 = std::regex("(-frc)");//-fcr
auto re31 = std::regex("(-frc) +([^ \n]+)");//-fcr "start-stop"
auto re32 = std::regex("(-frc) +([0-9]+ *)-( *[0-9]+)");//-fcr start-stop

  if (std::regex_search(argumentPack, match, re30)){
    if (std::regex_search(argumentPack, match, re31)){
      paramText = match[2].str();
    }else{//re31 else //not found [-fcr "start-stop"]
      std::cout << "-fcr -frame number range error" << std::endl;
      return false;
    }

    if(std::regex_search(argumentPack, match, re32)){
      if (paramText == match[2].str()+"-"+match[3].str()){
        std::cout << "Capture mode"  <<match[2].str() << std::endl;
        
        frameCaptureStart = atoi(match[2].str().c_str());
        frameCaptureStop  = atoi(match[3].str().c_str());
        if (frameCaptureStart > frameCaptureStop){
          std::cout << "Can not Capture: [FrameStart < FrameStop]" << std::endl;
          return false;          
        }
        if (hasMediaName()){
                      
          return true;
        }else{
          return false;
        }
      }else{// paramText!= match[2].str()
        std::cout << "-frc frame number range error" << std::endl;
        return false;
      }
    }else{//re32 else //not found [-fcr start-stop]
      std::cout << "-frc frame number range error" << std::endl;
      return false;
    }


  }else{//re30 else //not found -fcr
    std::cout << "-fcr not found re30" << std::endl;
    return false;
  }

}

bool paramExtract::hasParamPlay(){
std::string paramText = "";
auto re40 = std::regex("(-p)");
auto re41 = std::regex("(-p) +([^- \n]+)");
auto re42 = std::regex("(-p) +([0-9]+)");

  if (std::regex_search(argumentPack, match, re40)){
    if (std::regex_search(argumentPack, match, re41)){
        paramText = match[2].str();
    }else{//re41 else //not found [-p "start"]
      playStart =0;
      return true;
    }

    if(std::regex_search(argumentPack, match, re42)){
      if (hasMediaName()){
        
        if (paramText!= match[2].str()){
          std::cout << "-p invalid frame number" << std::endl;
          return false;
        }else{
          playStart = atoi(match[2].str().c_str());
          return true;
        }
        
      }
      return false;
    }else{
      std::cout << "-p can not play -invalid frame number" << std::endl;
      return false;
    }
    playStart=0;
    std::cout << "-p 0" << std::endl;
    return true;
  }else{//re40 else //not found -p
    return false;
  }

}
void paramExtract::printInfo(){
  std::cout <<"argumentPack: " << argumentPack  << std::endl;
  std::cout <<"Input Media File: " << mediaFileName << std::endl;
  std::cout <<"frameCaptureStart: " << frameCaptureStart <<std::endl;
  std::cout <<"frameCaptureStop: " << frameCaptureStop <<std::endl;
  std::cout <<"playStart: " << playStart << std::endl;
}

bool paramExtract::hasMediaName(){
  return (mediaFileName=="")?false:true;
}

bool paramExtract::hasFrameCaptureStart(){
  return (frameCaptureStart != frameCaptureInit)?true:false;
}

bool paramExtract::hasFrameCaptureStop(){
  return (frameCaptureStop != frameCaptureInit)?true:false;
}

bool paramExtract::hasPlayStart(){
  return (playStart != maxFrame)?true:false;
}

bool paramExtract::isAlphaString(std::string str){//check a-z and A-Z and -
return all_of( str.begin(), 
                str.end(), 
                [](char ch) { 
                  return (isalpha(ch)||ch=='-'||ch=='.');
                }
              );
}
bool paramExtract::isNumericString(std::string str){
  return !any_of( str.begin(), 
                  str.end(), 
                  [](char ch) { 
                    return !isdigit(ch);
                  }
                );
}
