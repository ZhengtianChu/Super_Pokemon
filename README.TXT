G52CPP coursework
=================

This readme includes instructions for various different platforms since you suggested that you may want to use different platforms. 
Please note that we can support you on Windows, e.g. helping out if there is some configuration issue, but you should only use a different platform if you think you can do so without needing our support for it!
i.e. the different options are provided for your convenience, at your own risk.

You can demo the coursework using any platform you wish - just make sure it works when you demo it.
It should be possible to take your files to any other platform and they just run.
If you want to submit to the hall of fame, please recompile the code on the lab PCs at that time, and provide me an exe for windows.

IMPORTANT: If you change platforms or versions of Visual Studio after you have added files, you will need to remember to add them to the project / makefiles that you are using.


Windows:
========

If you are running on Visual Studio 2019 (e.g. lab pcs), that is the default, so just run the CourseworkBase2021_vs2019.sln file which is in this directory.
This is set up to use Visual Studio 2019. You can probably migrate it to later versions if you wish.

When you build, the output executables will be placed in the Debug and Release directories in this folder.
When you run the program from within visual studio, it will look for things like the fonts and images in the src directory.
You MUST have the SDL dll files in your debug and release directories if you want to be able to run it. e.g. copy everything from the SDL/SDL2_dlls_x86 directory into your debug and release directories and it will find the SDL dlls that it needs to run.
I put the files there so that you don't need to install SDL on your computer - it will find these files instead.


Mac or Linux
============

If you are using Mac or Linux, download the additional mac/linux zip file and unzip it into the same directory as this one.
If you did it correctly, your directory will contain a Debug, Release, SDL, src, build, cmake and other directories.
