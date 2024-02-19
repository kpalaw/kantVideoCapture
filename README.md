## kantCap - Capture Video Frame to PNG image.
## Usage
Ex 1. kantcap will capture frame 1000 from "vid.mp4" video file to PNG file. 

    $kantca[ -i vid.mp4 -fc 1000
    kantcap -i <inputVideo> -fc <frameNumber>

Ex 2. kantcap will capture frame 100 to 200 from vid.mp4 video file to PNG files.

    $kantcap -i vid.mp4 -fcr 100-200
    kantcap -i <inputVideo> -frc <frameStart>-<frameStop>

Ex 3. kantcap will display vid.mp4 from frame 4000 onward.

    $kantcap -vid.mp4 -p 4000
    $kantcap -i <inputVideo> -p <frameStart>

![image](https://github.com/kpalaw/kantVideoCapture/assets/16006116/b8251612-87a1-4a73-b80f-64aa7fb66c7a)


 ## Install ffmpeg Library
 

    $sudo apt update
    $sudo apt install ffmpeg
To verity the installation,

    ffmpeg -version
The result would be like this is the installation success.

    ffmpeg version 4.2.7-0ubuntu0.1 Copyright (c) 2000-2022 the FFmpeg developers
    built with gcc 9 (Ubuntu 9.4.0-1ubuntu1~20.04.1).........

## Install SDL2
You can check whether the SDL2 is in your system or not

    apt-cache search libsdl2 

The result would be:

    libsdl2-2.0-0 - Simple DirectMedia Layer
    libsdl2-dev - Simple DirectMedia Layer development files
    libsdl2-doc - Reference manual for libsdl2
    libsdl2-gfx-1.0-0 - drawing and graphical effects extension for SDL2
    libsdl2-gfx-dev - development files for SDL2_gfx
    libsdl2-gfx-doc - documentation files for SDL2_gfx
    libsdl2-image-2.0-0 - Image loading library for Simple DirectMedia Layer 2, libraries
    libsdl2-image-dev - Image loading library for Simple DirectMedia Layer 2, development files
    libsdl2-mixer-2.0-0 - Mixer library for Simple DirectMedia Layer 2, libraries
    libsdl2-mixer-dev - Mixer library for Simple DirectMedia Layer 2, development files
    libsdl2-net-2.0-0 - Network library for Simple DirectMedia Layer 2, libraries
    libsdl2-net-dev - Network library for Simple DirectMedia Layer 2, development files
    libsdl2-ttf-2.0-0 - TrueType Font library for Simple DirectMedia Layer 2, libraries
    libsdl2-ttf-dev - TrueType Font library for Simple DirectMedia Layer 2, development files


Otherwise, we need to install if the library can't be detected.

    
    $sudo apt install libsdl2-dev
   
    $sudo apt install libsdl2-image-dev
    
    $sudo apt install libsdl2-mixer-dev
    
    $sudo apt install libsdl2-ttf-dev 
## Install libpng

    $sudo apt-get update
    $sudo apt-get install libpng-dev


## Compile using "compile_kant.sh"

go into "sources" folder and run

    ./compile_kant.sh



## Compile with cmake
at the same folder as "CMakeLists.txt"...then run
  

      $cmake -B build .
      $cmake --build build
![image](https://github.com/kpalaw/kantVideoCapture/assets/16006116/beb66aa7-1d39-4c83-b82a-e5388c3046d1)

![image](https://github.com/kpalaw/kantVideoCapture/assets/16006116/56fb202c-62ad-42e0-864c-d257331a7d57)
