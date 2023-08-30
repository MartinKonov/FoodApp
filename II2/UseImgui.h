#pragma once
#include <imgui.h>
#include<Windows.h>
#include "Recipes.h"
#include <GLFW/glfw3.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include <stdio.h>
#include<iostream>
#include<vector>
#include <string>


class UseImgui
{
    bool IsClickedText(const char* label);
public:
    void Init(GLFWwindow* window, const char* glsl_version);
    void NewFrame();
    void Update(unsigned char* cake, int widht, int height);
    void Render(GLFWwindow* window);
    void Shutdown();
};

