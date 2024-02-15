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

class progressBar;


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
  paramExtract(int argc, char* argv[]){
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

  std::string getMediaFileName(){
    return mediaFileName;
  }
  int getFrameCaptureStart(){
    return frameCaptureStart;
  }
  int getFrameCaptureStop(){
    return frameCaptureStop;
  }
  int getPlayStart(){
    return playStart;
  }


  bool hasParamInputFile(){//eg. kantVid -i imput.mp4
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

  bool hasParamFile(){//eg. kantVid input.mpv
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
  
  bool hasParamFrameCapture(){
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
  bool hasParamFrameCaptureRange(){
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

  bool hasParamPlay(){
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
  void printInfo(){
    std::cout <<"argumentPack: " << argumentPack  << std::endl;
    std::cout <<"Input Media File: " << mediaFileName << std::endl;
    std::cout <<"frameCaptureStart: " << frameCaptureStart <<std::endl;
    std::cout <<"frameCaptureStop: " << frameCaptureStop <<std::endl;
    std::cout <<"playStart: " << playStart << std::endl;
  }

  bool hasMediaName(){
    return (mediaFileName=="")?false:true;
  }

  bool hasFrameCaptureStart(){
    return (frameCaptureStart != frameCaptureInit)?true:false;
  }

  bool hasFrameCaptureStop(){
    return (frameCaptureStop != frameCaptureInit)?true:false;
  }

  bool hasPlayStart(){
    return (playStart != maxFrame)?true:false;
  }
  
  bool isAlphaString(std::string str){//check a-z and A-Z and -
  return all_of( str.begin(), 
                  str.end(), 
                  [](char ch) { 
                    return (isalpha(ch)||ch=='-'||ch=='.');
                  }
                );
  }
  bool isNumericString(std::string str){
    return !any_of( str.begin(), 
                    str.end(), 
                    [](char ch) { 
                      return !isdigit(ch);
                    }
                  );
  }

};

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
    videoCapture(){

    }
    videoCapture(std::string vidFile){
      vidFileName = vidFile;
      
    }

    void pauseToggle(){
      SDL_Event event; 
      bool pause = false; 
      
      do{
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT){
          pgBar->clearBar();
          exit(1);
        }
        if (event.type == SDL_KEYDOWN){
          //std::cout << "debug" << std::endl;
          if (event.key.keysym.sym == SDLK_SPACE){
            pause = !pause;
          }
          if (event.key.keysym.sym == SDLK_c){
            std::cout << "cPress" << std::endl;
            //pause = !pause;
          }
        }
      }while(pause);

    }//end fn

    int vidInit(std::string vidFile){
      // Open input file
      formatContext = avformat_alloc_context();//format of input file
      if (avformat_open_input(&formatContext, vidFile.c_str(), nullptr, nullptr) != 0) {
        std::cout << "Can not find or open [" << vidFile.c_str() << "]" << std::endl;
        exit(-1);
      }
      
      // for (int i = 0; i < (int)formatContext->nb_streams; i++) {
      //     AVStream* stream = formatContext->streams[i];
                  
      // }
      lastFrame = formatContext->streams[0]->nb_frames;
      avformat_find_stream_info(formatContext, nullptr);//format of input
      
      videoStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
      codecContext = avcodec_alloc_context3(nullptr);
      avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar);
      codec = avcodec_find_decoder(codecContext->codec_id);
      avcodec_open2(codecContext, codec, nullptr);

      width = codecContext->width;
      hight = codecContext->height;
      showInfo(); 
      
      return 0;
    }

    int capturVideo(std::string vidFile, int frameCapStart, int frameCapStop){
                  
      vidInit(vidFile);        

      AVPacket* packet = av_packet_alloc();
      AVFrame* frame = av_frame_alloc();
      AVFrame* newframe = av_frame_alloc();
      swsContext = sws_getContext(width, hight, codecContext->pix_fmt, 
                                  width, hight, AV_PIX_FMT_RGB24, 
                                  SWS_BILINEAR, nullptr, nullptr, nullptr);

      //int numberOfFrameRead =1000; //
      for (unsigned int i=0; i< formatContext->nb_streams*lastFrame+200; i++){
        //pauseToggle();
        if (av_read_frame(formatContext, packet) >= 0) { //read from stream
          
          if (packet->stream_index == videoStreamIndex) { //only video and ignor audio

            int ret = avcodec_send_packet(codecContext, packet); //send to decoder

            ret = avcodec_receive_frame(codecContext, frame); // get frame frome decoder
              
            if (ret == 0) {//frame ok
              
              std::cout << packet->pts<< "debugO:" <<frameCapStart << " " << frameCapStop << std::endl;
              if (packet->pts >= frameCapStart && packet->pts <=frameCapStop) { //frameNumber to capture
                  
                  int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, frame->width, frame->height, 1);
                  uint8_t *buffer = (uint8_t *)av_malloc(buffer_size);
                  av_image_fill_arrays(newframe->data, newframe->linesize, buffer, AV_PIX_FMT_RGB24, frame->width, frame->height, 1);
                  int h;
                  h = sws_scale(swsContext, frame->data, frame->linesize, 0,
                            frame->height,
                            newframe->data, newframe->linesize);
                  std::string fileNameSaved = "";//numberOfFrame.png
                  fileNameSaved += std::to_string(packet->pts);
                  fileNameSaved += ".png";
                  saveFrameAsPNG(fileNameSaved.c_str(), newframe, h);  
                  //saveFrameAsPNG("frameX.png", newframe, h);                         
              }
            }
            
            av_packet_unref(packet);
          } 
        }
      }//end for
      
      // Cleanup resources
      av_frame_free(&frame);
      avcodec_free_context(&codecContext);
      avformat_close_input(&formatContext);
      
      sws_freeContext(swsContext);

      // Deinitialize FFmpeg
      avformat_network_deinit();
      SDL_Quit();      

    }
    void playVideo(std::string vidFile, int frameCap, int framePlaySt){
      
      vidInit(vidFile);
      
      AVPacket* packet = av_packet_alloc();
      AVFrame* frame = av_frame_alloc();
      AVFrame* newframe = av_frame_alloc();
      
      SDL_Init(SDL_INIT_VIDEO);
      SDL_Window* window = SDL_CreateWindow("Kant Video Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, codecContext->width, codecContext->height, SDL_WINDOW_OPENGL);
      SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
      
      swsContext = sws_getContext(width, hight, codecContext->pix_fmt, 
                                  width, hight, AV_PIX_FMT_RGB24, 
                                  SWS_BILINEAR, nullptr, nullptr, nullptr);


      pgBar= new progressBar(80,0,1);
      //int numberOfFrameRead =1000; //
      
      for (unsigned int i=0; i<(formatContext->nb_streams+1)*lastFrame; i++){//

        pauseToggle();
        if (av_read_frame(formatContext, packet) >= 0) { //read from stream
          
          if (packet->stream_index == videoStreamIndex) { //only video and ignor audio

            int ret = avcodec_send_packet(codecContext, packet); //send to decoder

            ret = avcodec_receive_frame(codecContext, frame); // get frame frome decoder
              
            if (ret == 0) {//frame ok
              
              if (packet->pts == frameCap ) { //frameNumber to capture
                  
                  int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, frame->width, frame->height, 1);
                  uint8_t *buffer = (uint8_t *)av_malloc(buffer_size);
                  av_image_fill_arrays(newframe->data, newframe->linesize, buffer, AV_PIX_FMT_RGB24, frame->width, frame->height, 1);
                  int h;
                  h = sws_scale(swsContext, frame->data, frame->linesize, 0,
                            frame->height,
                            newframe->data, newframe->linesize);
                  std::cout << "Frame: " << packet->pts << " Saved." <<std::endl;
                  std::string fileNameSaved = "";//numberOfFrame.png
                  fileNameSaved += std::to_string(packet->pts);
                  fileNameSaved += ".png";
                  saveFrameAsPNG(fileNameSaved.c_str(), newframe, h);                         
              }

              if (packet->pts >= framePlaySt){
                
                // Display the frame
                std::cout << "display Frame:" << packet->pts << "\r"; //display frame number
                std::cout.flush();
                pgBar->updateBar((int)((packet->pts*100)/lastFrame), (int)packet->pts);               
                SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, codecContext->width, codecContext->height);
                SDL_UpdateYUVTexture(texture, nullptr, frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
                SDL_RenderCopy(renderer, texture, nullptr, nullptr);
                SDL_RenderPresent(renderer);
                
                SDL_DestroyTexture(texture); 
                std::this_thread::sleep_for(std::chrono::milliseconds(41));
              }
              
            }
            
            av_packet_unref(packet);
          } 
        }
      }//end for
      
      pgBar->clearBar();
      // Cleanup resources
      av_frame_free(&frame);
      avcodec_free_context(&codecContext);
      avformat_close_input(&formatContext);
      
      sws_freeContext(swsContext);
      //pauseUntilClickClose();

      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      // Deinitialize FFmpeg
      avformat_network_deinit();
      SDL_Quit();      
    }

    int frameToPng(int frameNumber){

      return 0;
    }

    void saveFrameAsPNG(const char* filename, AVFrame* frame, int height) {
    FILE* fp = fopen(filename, "wb");
    std::cout << filename << " Saved" <<std::endl;
    // Initialize PNG writer
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    png_infop info = png_create_info_struct(png);
    
    png_set_compression_level(png, 0);

    // Set up error handling
    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "Error during PNG creation" << std::endl;
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    png_bytep* rows = new png_bytep[hight];

    for (int y = 0; y < hight; ++y) {
        rows[y] = frame->data[0] + y * frame->linesize[0];
    }

    png_init_io(png, fp);

    png_set_IHDR(png, info, width, hight, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_rows(png, info, rows);

    png_write_png(png, info, PNG_TRANSFORM_INVERT_MONO, NULL);
    
    // Cleanup
    png_destroy_write_struct(&png, &info);
    fclose(fp);

    delete[] rows;
}


    void showInfo(){
      pixelFormat = av_get_pix_fmt_name(codecContext->pix_fmt);
      nameOfEncoder += codec->name;
      
      //get MetaData
      std::cout << "--Meta Data--" << std::endl;
      while ((metaDataEntry = av_dict_get(formatContext->metadata, "", metaDataEntry, AV_DICT_IGNORE_SUFFIX))) {
            vidMetaData[metaDataEntry->key] = metaDataEntry->value;
      }
      for (const auto& pair : vidMetaData) {
        std::cout <<  pair.first << "-->" << pair.second << std::endl;
      }
      std::cout << "Resolution W x H-->" << width << "x" << hight << std::endl
                << "Pixel Format-->" << pixelFormat << std::endl
                << "Encoder-->" << nameOfEncoder << std::endl
                << "Total Frames-->" << lastFrame << std::endl;

    }
};

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
