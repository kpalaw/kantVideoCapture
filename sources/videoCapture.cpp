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
#include "videoCapture.h"



videoCapture::videoCapture(){

}
videoCapture::videoCapture(std::string vidFile){
  vidFileName = vidFile;
  
}

void videoCapture::pauseToggle(){
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

int videoCapture::vidInit(std::string vidFile){
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

int videoCapture::capturVideo(std::string vidFile, int frameCapStart, int frameCapStop){
              
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
void videoCapture::playVideo(std::string vidFile, int frameCap, int framePlaySt){
  
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

int videoCapture::frameToPng(int frameNumber){

  return 0;
}

void videoCapture::saveFrameAsPNG(const char* filename, AVFrame* frame, int height) {
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


void videoCapture::showInfo(){
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
