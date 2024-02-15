#include "progressBar.h"

progressBar::progressBar(){

}
progressBar::progressBar(int bs, int percentStart,int barT ){
  blankSizeInit = bs;
  percentageInit = percentStart;
  barType = barT;
}

int progressBar::increasePercentage(){
  return ++percentageCurrent;
}

void progressBar::updateBar(int percentage, int fnumber){
  
  int blockSize = percentage * blankSizeInit/100;
  if (barType == 0){
    bar = "";
    bar += "[";
    for(int i=0; i<blockSize; i++){
      bar += "=";
    }
    for (int i=blockSize; i<blankSizeInit; i++){
      bar += " ";
    }
    bar += "]";
  }

  if (barType == 1){//bar  + percentage
    bar = "";
    bar += "[";
    for(int i=0; i<blockSize; i++){
      bar += "=";
    }
    for (int i=blockSize; i<blankSizeInit; i++){
      bar += " ";
    }

    bar += "][";
    bar += std::to_string(percentage);
    bar += "%]";
    bar += "[";
    bar += std::to_string(fnumber);
    bar += "]";
  }
  
  std::cout << bar;
  std::cout << "\r";
  std::cout.flush();
}
void progressBar::clearBar(){
  bar = "";
  bar += " ";
  for(int i=0; i<blankSizeInit+12; i++){
    bar += " ";
  }
  bar += " ";
  std::cout << bar;
  std::cout << "\r";
  std::cout.flush();
  std::cout << std::endl;
}

