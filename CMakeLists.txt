cmake_minimum_required(VERSION 3.5)

project(MultiCam LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL Network REQUIRED)
find_package(OpenCV 4.5.3 REQUIRED)
find_package(OpenGL REQUIRED)

if (MSVC)
add_compile_options(/wd4200 /wd4267 /wd4102 /wd4309 /wd4305)
endif()

add_compile_options(-Wno-attributes)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

QT5_WRAP_UI( UI_HEADERS 
    ${CMAKE_SOURCE_DIR}/src/formCameraConfig/formcamconfig.ui
    ${CMAKE_SOURCE_DIR}/src/formCameraWindow/formcamwindow.ui
    ${CMAKE_SOURCE_DIR}/src/formVioWindow/formviowindow.ui
    ${CMAKE_SOURCE_DIR}/src/formCvWindow/formcvwindow.ui
    ${CMAKE_SOURCE_DIR}/src/formVisionWindow/formvisionwindow.ui
    ${CMAKE_SOURCE_DIR}/src/form3DViewWindow/form3dviewwindow.ui
    ${CMAKE_SOURCE_DIR}/src/formLogWindow/formlogwindow.ui
    ${CMAKE_SOURCE_DIR}/src/px4/formpx4window.ui
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
    src/vision/MultipleViewTriangulation.cpp
    src/vision/MultipleViewTriangulationCommon.cpp
    src/vision/OptimalCorrection.cpp
    src/vision/CameraPointMap.cpp
    src/firmwareUpgrade/firmwareUpgrade.cpp
    src/firmwareUpgrade/upgradeRecvThread.cpp
    src/firmwareUpgrade/upgradeThread.cpp
    src/formCameraWindow/MuItemCam.cpp
    src/MuItemCtrl/MuItemCtrl.cpp
    src/MuItemCtrl/MuItemCtrlThread.cpp
    src/process/CamProcess.cpp
    src/process/VisionProcess.cpp
    src/px4/formpx4window.cpp
    src/PathPlan/PlanPoint.cpp
    src/PathPlan/MuItemPoint.cpp

    src/vision/GLPoint.cpp

    src/main.cpp
    src/openvio/openvio.cpp
    src/winusb/winusbdriver.cpp
    src/winusb/usbthread.cpp
    src/easytool/easytool.cpp
    src/easytool/findstr.cpp
    src/process/CtrlProcess.cpp
    src/process/PlanFlyProcess.cpp
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

    src/px4/PX4Thread/PX4Thread.cpp

    src/drow/model.cpp
    src/drow/mesh.cpp
    src/drow/glwidget.cpp
    src/drow/camera.cpp
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
                    src/px4/mavlink
                    src/px4/mavlink/common
                    src/px4/PX4Thread
                    src/px4
                    src/PathPlan
                    lib/eigen-3.2.10
                    lib/libusb/include/libusb-1.0
                    lib/assimp/include
                    ${OpenCV_INCLUDE_DIRS}
)

link_directories(
                    lib/eigen-3.2.10
                    )

if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO /NODEFAULTLIB:${CMAKE_SOURCE_DIR}/lib/usb/libusb-1.0.lib")
endif()           

target_link_libraries(MultiCam 
                        Qt5::Widgets
                        Qt5::Core
                        Qt5::Gui
                        Qt5::Network
                        Qt5::OpenGL
                        ${OPENGL_LIBRARIES}
                        ${CMAKE_SOURCE_DIR}/lib/libusb/MinGW64/dll/libusb-1.0.dll.a
                        ${CMAKE_SOURCE_DIR}/lib/assimp/lib/libIrrXML.a
                        ${CMAKE_SOURCE_DIR}/lib/assimp/lib/libassimp.dll.a
                        ${OpenCV_LIBS}
                        
)