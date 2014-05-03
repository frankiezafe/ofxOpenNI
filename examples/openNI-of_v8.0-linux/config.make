OF_ROOT=/home/frankiezafe/softs/of_v0.8.0_linux64_release
PROJECT_ROOT = .
PROJECT_LDFLAGS=-Wl,-rpath=./libs,/usr/lib/libOpenNI.so
PROJECT_CFLAGS = -I /usr/include/ni -I $(OF_ROOT)/addons/ofxOpenNI/src
