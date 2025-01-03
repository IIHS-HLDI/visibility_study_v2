set(DEPENDENCY_DIR "${CMAKE_SOURCE_DIR}/dependencies")

cmake_minimum_required(VERSION 3.10)

project(VisibilityTool VERSION 1.0)


if(MSVC)
    add_definitions(/MP)

    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# set_property(TARGET VisibilityTool PROPERTY
#              MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(VisibilityTool ${CMAKE_SOURCE_DIR}/src/Main.cpp)
target_sources(VisibilityTool PRIVATE ${SRC_FILES})

target_compile_features(VisibilityTool PRIVATE cxx_std_20)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
    set_target_properties(VisibilityTool PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(VisibilityTool PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
endif()


#set(CMAKE_CXX_FLAGS_RELEASE "/MT")
#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

target_include_directories(VisibilityTool PRIVATE ${CMAKE_SOURCE_DIR}/src)

# cereal
set(BUILD_DOC OFF)
set(BUILD_SANDBOX OFF)
set(SKIP_PERFORMANCE_COMPARISON YES)
add_subdirectory(${DEPENDENCY_DIR}/cereal)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/cereal/include)
target_link_libraries(VisibilityTool cereal)

# glad
add_subdirectory(${DEPENDENCY_DIR}/glad)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/glad/include)
target_link_libraries(VisibilityTool glad)

# glfw3
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(${DEPENDENCY_DIR}/glfw3)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/glfw3/include)
target_link_libraries(VisibilityTool glfw)

# glm
set(GLM_ENABLE_CXX_17 ON)
add_subdirectory(${DEPENDENCY_DIR}/glm)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/glm/include)
target_link_libraries(VisibilityTool glm)

# imgui
add_subdirectory(${DEPENDENCY_DIR}/imgui)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/imgui/include)
target_link_libraries(VisibilityTool imgui)

# stb image
# add_subdirectory(${DEPENDENCY_DIR}/stb)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/stb/include)

# nfd, for cross-platform file/folder selection windows
add_subdirectory(${DEPENDENCY_DIR}/nfd)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/nfd/src/include)
target_link_libraries(VisibilityTool nfd)

# boxer, for cross-platform message boxes
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(${DEPENDENCY_DIR}/boxer)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/boxer/include)
target_link_libraries(VisibilityTool Boxer)

# opencv
set(OpenCV_STATIC ON)
set(BUILD_opencv_world ON)

set(BUILD_opencv_videoio OFF)
set(BUILD_opencv_highgui OFF)
set(BUILD_opencv_video OFF)
set(BUILD_opencv_objdetect OFF)
set(BUILD_opencv_dnn OFF)
set(BUILD_opencv_ml OFF)
set(BUILD_opencv_photo OFF)
set(BUILD_opencv_gapi OFF)
set(BUILD_opencv_ts OFF)
set(BUILD_opencv_stitching OFF)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_JAVA OFF)
set(BUILD_opencv_python3 OFF)
set(BUILD_opencv_python2 OFF)
set(BUILD_OBJC OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTS OFF)
set(BUILD_PERF_TESTS OFF)
set(BUILD_opencv_apps OFF)
set(BUILD_opencv_js OFF)
set(BUILD_DOCS OFF)

set(VIDEOIO_ENABLE_PLUGINS OFF)
set(HIGHGUI_ENABLE_PLUGINS OFF)
set(DNN_ENABLE_PLUGINS OFF)
set(BUILD_ANDROID_EXAMPLES OFF)
set(BUILD_ANDROID_PROJECTS OFF)

add_subdirectory(${DEPENDENCY_DIR}/opencv)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/opencv/include)
target_include_directories(VisibilityTool PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/opencv/modules/core/include)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/opencv/modules/imgproc/include)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/opencv/modules/imgcodecs/include)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/opencv/modules/calib3d/include)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/opencv/modules/features2d/include)
target_include_directories(VisibilityTool PRIVATE ${DEPENDENCY_DIR}/opencv/modules/flann/include)

target_link_libraries(VisibilityTool opencv_world)

# Resource Manager
#add_subdirectory("${DEPENDENCY_DIR}/ResourceManager-1.0")
#target_include_directories(VisibilityTool PRIVATE "${DEPENDENCY_DIR}/ResourceManager-1.0/include")



#target_link_libraries(VisibilityTool ade)
#target_link_libraries(VisibilityTool IlmImf)
#target_link_libraries(VisibilityTool ippicvmt)
#target_link_libraries(VisibilityTool ippiw)
#target_link_libraries(VisibilityTool ittnotify)
#target_link_libraries(VisibilityTool libclapack)
#target_link_libraries(VisibilityTool libjpeg-turbo)
#target_link_libraries(VisibilityTool libopenjp2d)
#target_link_libraries(VisibilityTool libprotobuf)
#target_link_libraries(VisibilityTool libtiff)
#target_link_libraries(VisibilityTool libwebp)
#target_link_libraries(VisibilityTool zlib)
#target_link_libraries(VisibilityTool opencv_world)



