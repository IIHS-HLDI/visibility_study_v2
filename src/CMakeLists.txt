set(PROJECT_NAME VisibilityTool)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Action.h"
    "ApplicationInstance.h"
    "ApplicationState.h"
    "ApplicationWindow.h"
    "Camera2D.h"
    "Combo.h"
    "DisplayGrid.h"
    "Drawable.h"
    "Event.h"
    "EventManager.h"
    "Image.h"
    "NewVehicleProjectWindow.h"
    "Scene.h"
    "SceneWindow.h"
    "SubWindow.h"
    "VehicleInfo.h"
    "VehicleProject.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Action.cpp"
    "ApplicationInstance.cpp"
    "ApplicationWindow.cpp"
    "Camera2D.cpp"
    "Combo.cpp"
    "DisplayGrid.cpp"
    "Drawable.cpp"
    "Event.cpp"
    "EventManager.cpp"
    "Image.cpp"
    "NewVehicleProjectWindow.cpp"
    "Scene.cpp"
    "SceneWindow.cpp"
    "SubWindow.cpp"
    "VehicleProject.cpp"
    "VisibilityTool.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE VisibilityTool)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/imgui/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/glm/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/glfw3/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/glad/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/nfd/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/opencv/include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "GLFW_INCLUDE_NONE;"
        "_CRT_SECURE_NO_WARNINGS;"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)

    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "aded;"
            "IlmImfd;"
            "ippiwd;"
            "ittnotifyd;"
            "libclapackd;"
            "libjpeg-turbod;"
            "libopenjp2d;"
            "libpngd;"
            "libprotobufd;"
            "libtiffd;"
            "libwebpd;"
            "opencv_world500d;"
            "zlibd"
        ">"
        "$<$<CONFIG:Release>:"
            "libopenjp2;"
            "libpng;"
            "libprotobuf;"
            "libtiff;"
            "libwebp;"
            "opencv_world500;"
            "zlib;"
            "ade;"
            "IlmImf;"
            "ippiw;"
            "ittnotify;"
            "libclapack;"
            "libjpeg-turbo"
        ">"
        "nfd;"
        "ippicvmt;"
        "ImGui;"
        "glfw3_mt;"
        "glad"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_SOURCE_DIR}//dependencies/glfw3/lib;"
            "${CMAKE_SOURCE_DIR}//dependencies/glad/lib/debug;"
            "${CMAKE_SOURCE_DIR}//dependencies/imgui/lib/release;"
            "${CMAKE_SOURCE_DIR}//dependencies/opencv/lib/debug;"
            "${CMAKE_SOURCE_DIR}/dependencies/nfd/lib/debug"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_SOURCE_DIR}/dependencies/glad/lib/release;"
            "${CMAKE_SOURCE_DIR}/dependencies/glfw3/lib;"
            "${CMAKE_SOURCE_DIR}/dependencies/imgui/lib/release;"
            "${CMAKE_SOURCE_DIR}/dependencies/opencv/lib/release;"
            "${CMAKE_SOURCE_DIR}/dependencies/nfd/lib/release"
        ">"
    )
endif()

