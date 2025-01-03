#pragma once

#include <vector>
#include <functional>


#include <GLFW/glfw3.h>

struct WindowResizeEvent
{
    int width;
    int height;
};

struct KeyInputEvent
{
    int key;
    int scancode;
    int action;
    int modifier;
};

struct MouseMoveEvent
{
    double xPos; 
    double yPos;
};

struct MouseButtonEvent
{
    int button; 
    int action; 
    int modifier;
};

struct MouseScrollEvent
{
    double xOffset; 
    double yOffset;
};
   
// EventDispatcher singleton
template<typename T>
class EventDispatcher
{
public:

    static void cursorPositionCallback(
        GLFWwindow* window,
        double mouseX,
        double mouseY) {

        MouseMoveEvent newEvent = {mouseX, mouseY};
        dispatch(newEvent);
    }

    // static void mouseButtonCallback(
    //     GLFWwindow* window,
    //     int button,
    //     int action,
    //     int modifier) {

    //     MouseButtonEvent newEvent = {button, action, modifier};

    //     dispatch(newEvent);
    // }

    // static void scrollCallback(
    //     GLFWwindow* window,
    //     double deltaX,
    //     double deltaY) {

    //     MouseScrollEvent newEvent = {deltaX, deltaY};

    //     dispatch((MouseScrollEvent){
    //             .xOffset = deltaX, 
    //             .yOffset = deltaY});
    // }

    // static void keypressCallback(
    //     GLFWwindow* window,
    //     int key,
    //     int scancode,
    //     int action,
    //     int mods) {

    //     KeyInputEvent newEvent = {key, scancode, action, mods};

    //     dispatch((KeyInputEvent){key, scancode, action, mods});

    // }

    // static void resizeCallback(
    //     GLFWwindow* window,
    //     int newWidth,
    //     int newHeight) {

    //     dispatch((WindowResizeEvent){newWidth, newHeight});
    // }

    static void registerMemberHandler(const std::function<void(T)>& eventHandler)
    {
        get().handlers.push_back(eventHandler);
    }


    // static void registerHandler(const std::function<void(T)>& eventHandler)
    // {
    //     get().handlers.push_back(eventHandler);
    // }

    static void dispatch(T newEvent) 
    {        
        for(const std::function<void(T)>& eventHandler : get().handlers) {
            eventHandler(newEvent);
        }
    }

private:
    std::vector<std::function<void(T)>> handlers;
    
    static EventDispatcher& get() 
    {
		static EventDispatcher instance;
        return instance;
    }

    // delete constructor / copy constructor / assignment operator
    EventDispatcher() {}
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator= (const EventDispatcher&) = delete;
};

