cmake_minimum_required(VERSION 3.5)

project(MultiCam LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_GENERATOR "MinGW Makefiles")

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

add_executable(MultiCam 
    src/mainwindow.ui
    src/formcamconfig.ui
    src/main.cpp
    src/mainwindow.cpp
    src/winusbdriver.cpp
    src/usbthread.cpp
    src/easytool.cpp
    src/image.cpp
    src/glview.cpp
    src/gldrow.cpp
    src/imu.cpp
    src/setting.cpp
    src/findstr.cpp
    src/log.cpp
    src/formcamconfig.cpp
    src/imuso3.cpp
)

include_directories(
                    src
                    lib/eigen-3.2.10
                    lib/glut
)

link_directories(
                    lib/eigen-3.2.10
                    lib/glut
                    )

target_link_libraries(MultiCam 
                        Qt5::Widgets
                        Qt5::Core
                        Qt5::Gui
                        ${CMAKE_SOURCE_DIR}/lib/usb/libusb-1.0.a
                        -lopengl32
				        -lglu32
                        
)