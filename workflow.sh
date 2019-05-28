#!/bin/bash
###############################################################################
# Config.

BUILD_OPENCV=true

###############################################################################
# Resources.

# http://dlib.net/compile.html

###############################################################################
# Download

mkdir download
pushd download
#TODO
wget https://github.com/git-for-windows/git/releases/download/v2.21.0.windows.1/Git-2.21.0-64-bit.exe
wget https://github.com/Kitware/CMake/releases/download/v3.14.4/cmake-3.14.4-win64-x64.msi
wget http://dlib.net/files/dlib-19.17.zip
wget http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
curl -OJ https://codeload.github.com/opencv/opencv/zip/3.2.0
wget https://kent.dl.sourceforge.net/project/opencvlibrary/opencv-win/3.2.0/opencv-3.2.0-vc14.exe
popd

###############################################################################
# Install.

echo "Install git"
../../download/Git-2.21.0-64-bit.exe
# On last dialog, uncheck "Enable Git Credential Manager"
echo "Install cmake"
msiexec /a ../../download/cmake-3.14.4-win64-x64.msi

###############################################################################
# Build prerequisites.

PREFIX=$PWD/out

mkdir -p $PREFIX

mkdir -p build
pushd build


if [ "$BUILD_OPENCV" == "true" ]
then
	unzip ../download/opencv-3.2.0.zip
	mkdir -p opencv-3.2.0/build
	pushd opencv-3.2.0/build
	if [[ $OSTYPE == "windows" ]]
	then
		cmake .. -DCMAKE_INSTALL_PREFIX=$PREFIX \
			-G "Visual Studio 14 2015 Win64" -T host=x64
			#TODO Maybe do not need -T
	else
		cmake .. -DCMAKE_INSTALL_PREFIX=$PREFIX
	fi
	cmake --build . --config Release --target install
	popd
	rm -rf opencv-3.2.0
else
	echo "Install opencv-3.2.0-vc14.exe"
fi


unzip ../download/dlib-19.17.zip
mkdir -p dlib-19.17/build
pushd dlib-19.17/build
if [[ $OSTYPE == "windows" ]]
then
	cmake .. -DCMAKE_INSTALL_PREFIX=$PREFIX \
		-G "Visual Studio 14 2015 Win64" -T host=x64 -DUSE_AVX_INSTRUCTIONS=ON
else
	cmake .. -DCMAKE_INSTALL_PREFIX=$PREFIX
fi
cmake --build . --config Release --target install
popd
rm -rf dlib-19.17

popd

###############################################################################
# Build app.

mkdir -p build/face_landmark_remote_controller
pushd build/face_landmark_remote_controller

if [[ $OSTYPE == "windows" ]]
then
	cmake ../../source/face_landmark_remote_controller/ \
		-G "Visual Studio 14 2015 Win64"
else
	cmake ../../source/face_landmark_remote_controller/
fi
cmake --build . --config Release


#TODO Hack for DLLs.
# When run from Windows console in "build\face_landmark_remote_controller\Release\"
cp ../../out/x64/vc14/bin/opencv_*.dll Release/
# When run from Git Bash console in "build/face_landmark_remote_controller/"
cp ../../out/x64/vc14/bin/opencv_*.dll .
cp /c/Windows/SysWOW64/downlevel/api-ms-win-crt-*-l1-1-0.dll .

###############################################################################
# Test.

cp ../../download/shape_predictor_68_face_landmarks.dat.bz2 .
bzip2 -d shape_predictor_68_face_landmarks.dat.bz2
if [[ $OSTYPE == "windows" ]]
then
	./Release/face_landmark_remote_controller.exe \
		shape_predictor_68_face_landmarks.dat \
		../../build/dlib-19.17/examples/faces/*.jpg
else
	./face_landmark_remote_controller \
		shape_predictor_68_face_landmarks.dat \
		../../build/dlib-19.17/examples/faces/*.jpg
fi
popd

###############################################################################
