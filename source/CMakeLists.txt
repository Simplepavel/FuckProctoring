cmake_minimum_required(VERSION 3.5)
project(FuckProctoring)


set(qt_include_path C:/msys64/ucrt64/include/qt6 C:/msys64/ucrt64/include/qt6/QtWidgets C:/msys64/ucrt64/include/qt6/QtCore C:/msys64/ucrt64/include/qt6/QtGui)
include_directories(${qt_include_path}) # -I


set(qt_lib_path C:/msys64/ucrt64/lib/Qt6Widgets) # -L
set(qt_lib "C:/msys64/ucrt64/lib/libQt6Core.dll.a;C:/msys64/ucrt64/lib/libQt6Widgets.dll.a;C:/msys64/ucrt64/lib/libQt6Gui.dll.a") # -l


set(source Window/Window.cpp Application/FuckProctoring.cpp)

add_executable(main main.cpp ${source})


target_link_directories(main PRIVATE ${qt_lib_path})
target_link_libraries(main PRIVATE ${qt_lib})