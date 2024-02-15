#include <iostream>
#include <string>

class progressBar {
  private:
  int percentageInit =0;
  int percentageCurrent =0;
  std::string bar = "[          ]"; //init 10 space
  int blankSizeInit = 10;
  int blockSize = 0;
  int barType =0;
  public:
    progressBar();
    progressBar(int, int ,int );
    int increasePercentage();
    void updateBar(int, int);
    void clearBar();
  };