cmake_minimum_required(VERSION 3.10)
project(FuckProctoring)


set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Multimedia MultimediaWidgets)



# set(qt_include_path C:/msys64/ucrt64/include/qt6 C:/msys64/ucrt64/include/qt6/QtWidgets C:/msys64/ucrt64/include/qt6/QtCore C:/msys64/ucrt64/include/qt6/QtGui)
# include_directories(${qt_include_path}) # -I ...
# set(qt_lib_path C:/msys64/ucrt64/lib/Qt6Widgets) # -L
# set(qt_lib "C:/msys64/ucrt64/lib/libQt6Core.dll.a;C:/msys64/ucrt64/lib/libQt6Widgets.dll.a;C:/msys64/ucrt64/lib/libQt6Gui.dll.a") # -l



set(boost_include_path D:/Libs++/BOOST)
include_directories(${boost_include_path}) # -I D:/Libs++/BOOST
set(boost_lib_path "D:/Libs++/BOOST/stage/lib") # -L 
set(boost_lib wsock32 ws2_32 ${boost_lib_path}/libboost_timer-vc143-mt-gd-x32-1_91.lib ${boost_lib_path}/libboost_timer-vc143-mt-gd-x64-1_91.lib ${boost_lib_path}/libboost_timer-vc143-mt-x32-1_91.lib ${boost_lib_path}/libboost_timer-vc143-mt-x64-1_91.lib)



set(source Interface/Window.cpp Client/Client.cpp Server/Server.cpp Application/FuckProctoring.cpp Protocol/Protocol.cpp)
add_executable(main main.cpp ${source})

target_link_libraries(main PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::MultimediaWidgets
    Qt6::Multimedia
)


target_link_directories( # -L
    main 
    PRIVATE ${boost_lib_path}
)
target_link_libraries( # -l
    main 
    PRIVATE ${boost_lib}
)