# Visibility Tool

## Getting Started

### Setup Instructions for Visual Studio 2022

Prior to building the app, install Visual Studio 2022 with the "Desktop development with C++" component.

Once Visual Studio is installed, clone the git repository using the following command.
```sh
git clone git@ssh.dev.azure.com:v3/iihs/VRC-Research/Visibility_Tool_V2
```

Open the repository folder in Visual Studio. Visual Studio should automatically detect the CMakeLists.txt configuration file and begin configuring build targets. This can take several seconds. The output window should display "CMake generation finished." once the configuration is complete. 

After the CMake configuration is done, the VisibilityTool.exe target may automatically be selected. If not, change to the "CMake Targets" view in the Solution Explorer window and manually set VisibilityTool.exe to the VisibilityTool target.

To build the application, use Build>Build VisibilityTool.exe (CTRL+B). Alternatively, use the play button in the Visual Studio top bar to build and run. Building the app will generate a VisibilityTool.exe executable file, which can be shared as-is.

## Licensing

This software is licensed under the terms of the Apache License, version 2.0. 

This software was built with appreciation using the following open source projects, without modification to their original source code:

* [Boxer](https://github.com/aaronmjacobs/Boxer) [(licensing)](https://github.com/aaronmjacobs/Boxer/blob/master/LICENSE)
* [Cereal](https://USCiLab.github.io/cereal) [(licensing)](https://github.com/USCiLab/cereal/blob/master/LICENSE)
* [glad](https://github.com/Dav1dde/glad) [(licensing)](https://github.com/Dav1dde/glad/blob/glad2/LICENSE)
* [GLFW](www.glfw.org) [(licensing)](https://www.glfw.org/license.html)
* [GLM](https://github.com/g-truc/glm)  [(licensing)](https://github.com/g-truc/glm/blob/master/copying.txt)
* [ImGui](https://www.dearimgui.com/)  [(licensing)](https://github.com/ocornut/imgui/blob/master/LICENSE.txt)
* [nfd](https://github.com/btzy/nativefiledialog-extended) [(licensing)](https://github.com/btzy/nativefiledialog-extended/blob/master/LICENSE)
* [OpenCV](https://opencv.org/) [(licensing)](https://opencv.org/license/)
* [stb](https://github.com/nothings/stb) [(licensing)](https://github.com/nothings/stb/blob/master/LICENSE)


