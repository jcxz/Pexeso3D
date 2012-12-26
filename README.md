#What is this project ?#

This project is a 3D adaptation of the classical memory game PEXESO.
The goal of the game is to guess pairs of cards and
in the end the one who guessed more pairs becomes the winner.


#What platforms does it run on ?#

Though the primary development platform is Windows, it should run on Linux and Mac OS X as well.


#What are the dependancies ?#

Project uses Qt, OpenGL and GLU, no further dependencies are required.


#How to compile and run from command line ?#

If you have Qt SDK properly installed, the you can just do:

    qmake
    make -j5

To run the program you have to go to bin directory and run it from there,
because paths to assets and images are taken relative to this directory.
So you can do:

    cd bin
    ./pexeso


#How to compile and run from Qt Creator ?#

To compile and run from Qt Creator just double click the `pexeso.pro` file.
In the project setup screen uncheck the shadow build directory option.
To run the program you have to set your working directory to bin directory.
                                                                            