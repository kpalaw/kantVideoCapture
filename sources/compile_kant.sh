exec_file="./kantcap"

if [ -f "$exec_file" ]; then
    rm $exec_file
fi

g++ -c progressBar.cpp -o progressBar.o
g++ -c paramExtract.cpp -o paramExtract.o
g++ -c videoCapture.cpp -o videoCapture.o
g++ -c kantcap.cpp -o kantcap.o
gcc kantcap.o paramExtract.o progressBar.o videoCapture.o /usr/lib/x86_64-linux-gnu/libstdc++.so.6 -o kantcap -lavformat -lavcodec -lavutil -lswscale -lpng -lSDL2 

if [ -f "$exec_file" ]; then
    echo "Run... $exec_file"
    ./$exec_file "-i ../sampleVideos/vid2.mp4 -p 3000"
else
    echo "Compile/Link Fail... $exec_file does not exist"
fi