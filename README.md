#FDesktopRecorder

##Description
FDesktopRecorder is a program that can video record a single window or your entire desktop using FFmpeg.

###This can be handy for:
* "How-to's"
* Product demoes
* in-game videoes
* Educational videoes 
* Recording Interactive whiteboards like "SMART Boards"
* etc.

###Basic features:
* Video record a single window
* Video record your entire desktop

Based on a script by Peter Lyberth from the danish GNU/Linux show "Kanal Tux" http://www.kanaltux.dk 

##About
Created by: Ole "Froksen" Holm Frandsen
You can contact me via kde-apps.org. Find me under the username "froksen".

###Translated
* Danish by Ole Holm Frandsen (froksen)
* German by Christian Räss (saftsocken)
* Spanish by Javi
* Italian by Symbian Flo

##Build and Install

###Requires to run
* ffmpeq is used for the recording
* libqt4
* libkde4
* Optional:
  - kaffeine (the default preview player)

###Note to packagers
Currently CMake and QMake are equally supported. Although I might move towards only using CMake in the future. So if you haven't packaged the FDesktopRecorder before, use
CMake to build the program.

Please also read the "KNotifications" section below.
  
###Requires to build 
(NOTE: these are openSUSE 12.1 package names)
  ```
desktop-file-utils, libqt4-devel, gcc-c++, make, gcc46, libkde4-devel
  ```
###KNotifications
#### QMake 
Please note, that for Knotfications to work the "FDesktopRecorder.notifyrc" file must be placed in "/usr/share/kde4/apps/FDesktopRecorder". If Folder does not exists then create it.
This is currently beeing done for openSUSE by the RPM-file.

This way of installing the *.notifyrc file might change in the future.. :-)

#### CMake 
If you are using CMake to build this project, you can just do "# make install" after the "CMake-steps" below. 
This will install the *.notifyrc file in the correct place. If you are building an package (.rpm,.deb etc), please also read the "QMake" section above.

###How to build/compile via qmake
  ```
    $ cd source
    $ qmake FDesktopRecorder.pro
    $ make
  ```

###How to build/compile via CMake
  ```
    $ cd source
    $ mkdir build
    $ cd build/
    $ cmake ..
    $ make
  ```
  (You can do "make install" after this to install)
