extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <map>
#include <regex>

#include <cstdio>
#include <cstdlib>
#include <png.h>


#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
#include <libswscale/swscale.h>

#include <thread>
#include <chrono>

#include "progressBar.h"
#include "paramExtract.h"
#include "videoCapture.h"

class progressBar;



int main(int argc,char** argv) {

paramExtract pe(argc, argv);

  if (pe.hasMediaName()) {
    if ( pe.hasFrameCaptureStart() && 
        (pe.getFrameCaptureStart()==pe.getFrameCaptureStop())
       )
    {//-fc 123
      std::cout << pe.getMediaFileName() << ":Capture mode" << std::endl;
      videoCapture* vc =new videoCapture();
      vc->capturVideo(pe.getMediaFileName(),pe.getFrameCaptureStart(), pe.getFrameCaptureStop());
      
      return 0;
    }

    if (pe.hasMediaName()) {
      if ( pe.hasFrameCaptureStop() && 
          (pe.getFrameCaptureStart() != pe.getFrameCaptureStop())
        )
      {//-fc 123
        std::cout << pe.getMediaFileName() <<":Capture mode" << std::endl;
        videoCapture* vc =new videoCapture();
        vc->capturVideo(pe.getMediaFileName(),pe.getFrameCaptureStart(), pe.getFrameCaptureStop());
        
        return 0;
      }
    }

    if (pe.hasParamPlay()){//-p
      std::cout << "space bar to pause/resume" << std::endl;
      videoCapture* vc =new videoCapture();
      vc->playVideo(pe.getMediaFileName(),-1, pe.getPlayStart());
      
      return 0;
    }
    
  }

  return 0;
}


//Todo:
//-p x%
//-s silence mode
//-fbc frame bunch capture eg -fbc start:numberofFrames -fbc 1234:100 (means capture from 1234th frame to 1234+100th frame)
//error handling
