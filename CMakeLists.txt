cmake_minimum_required(VERSION 3.5)

project(MultiCam LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
find_package(OpenCV 4.5.3 REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_GENERATOR "MinGW Makefiles")

QT5_WRAP_UI( UI_HEADERS 
    ${CMAKE_SOURCE_DIR}/src/formCameraConfig/formcamconfig.ui
    ${CMAKE_SOURCE_DIR}/src/formCameraWindow/formcamwindow.ui
    ${CMAKE_SOURCE_DIR}/src/formVioWindow/formviowindow.ui
    ${CMAKE_SOURCE_DIR}/src/formCvWindow/formcvwindow.ui
    ${CMAKE_SOURCE_DIR}/src/formVisionWindow/formvisionwindow.ui
    ${CMAKE_SOURCE_DIR}/src/form3DViewWindow/form3dviewwindow.ui
    ${CMAKE_SOURCE_DIR}/src/formLogWindow/formlogwindow.ui
)

add_executable(MultiCam 

    ${UI_HEADERS}
    src/resource.qrc
    
    src/formCameraConfig/formcamconfig.cpp
    src/formCameraWindow/formcamwindow.cpp
    src/formVioWindow/formviowindow.cpp
    src/formVisionWindow/formvisionwindow.cpp
    src/form3DViewWindow/form3dviewwindow.cpp
    src/formlogWindow/formlogwindow.cpp
    src/formCvWindow/formcvwindow.cpp
    src/vision/multipleViewTriangulation.cpp
    src/firmwareUpgrade/firmwareUpgrade.cpp
    src/firmwareUpgrade/upgradeRecvThread.cpp
    src/firmwareUpgrade/upgradeThread.cpp
    src/formCameraWindow/MuItemCam.cpp
    src/MuItemCtrl/MuItemCtrl.cpp
    src/MuItemCtrl/MuItemCtrlThread.cpp
    src/process/CamProcess.cpp
    src/process/VisionProcess.cpp

    src/main.cpp
    src/openvio/openvio.cpp
    src/winusb/winusbdriver.cpp
    src/winusb/usbthread.cpp
    src/easytool/easytool.cpp
    src/easytool/findstr.cpp
    src/process/CtrlProcess.cpp
    src/image/image.cpp
    src/drow/camview.cpp
    src/drow/glview.cpp
    src/drow/gldrow.cpp
    src/setting/setting.cpp
    src/log/log.cpp
    
    src/imu/imuso3.cpp
    src/imu/imu.cpp

    src/gl_protocol/frame_send.c
    src/gl_protocol/frame_parse.c

    src/gl_protocol/frame_send.c
    src/gl_protocol/frame_parse.c
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
                    src/formCameraWindow
                    src/formVioWindow
                    src/formVisionWindow
                    src/form3DViewWindow
                    src/formLogWindow
                    src/formCvWindow
                    src/firmwareUpgrade
                    src/gl_protocol
                    src/process
                    src/MuItemCtrl
                    src/vision
                    src/openvio
                    lib/eigen-3.2.10
                    lib/glut
                    ${OpenCV_INCLUDE_DIRS}
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
                        -lglut32
                        ${OpenCV_LIBS}
                        
)