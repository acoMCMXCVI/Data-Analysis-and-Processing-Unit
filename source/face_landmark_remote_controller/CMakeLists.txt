cmake_minimum_required(VERSION 2.8.12)

project(face_landmark_remote_controller)

add_executable(
	face_landmark_remote_controller
	face_landmark_remote_controller.cpp
)

set(CMAKE_PREFIX_PATH ../../out/)
find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)

target_link_libraries(
	face_landmark_remote_controller
	dlib::dlib
	${OpenCV_LIBS}
)
