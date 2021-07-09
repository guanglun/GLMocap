cmake_minimum_required(VERSION 3.5)

project(MultiCam LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_GENERATOR "MinGW Makefiles")

QT5_WRAP_UI( UI_HEADERS 
    src/formMainWindow/mainwindow.ui
    src/formCameraConfig/formcamconfig.ui
)

add_executable(MultiCam 
    
    src/formCameraConfig/formcamconfig.cpp
    src/formMainWindow/mainwindow.cpp
    
    src/main.cpp
    src/winusb/winusbdriver.cpp
    src/winusb/usbthread.cpp
    src/easytool/easytool.cpp
    src/easytool/findstr.cpp
    src/image/image.cpp
    src/drow/glview.cpp
    src/drow/gldrow.cpp
    src/setting/setting.cpp
    src/log/log.cpp

    src/imu/imuso3.cpp
    src/imu/imu.cpp
)

include_directories(
                    src
                    src/winusb
                    src/imu
                    src/log
                    src/setting
                    src/easytool
                    src/drow
                    src/image
                    src/formCameraConfig
                    src/formMainWindow
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