#pragma once

#include<curl/curl.h>
#include<glad/glad.h>
#include<iostream>
#include<vector>
#include <string>

#include <locale>
#include <codecvt>

#include "Ingredient.h"

struct CreateAnAnswer
{
    std::string recipeName;
    std::string summary;
    std::string protein;
    std::string carbs;
    std::string fat;
    std::string calories;
    std::string link;
    std::vector<Ingredient> ingredients;
    std::vector<std::string> instructions;
    GLuint texture;


    CreateAnAnswer();
    CreateAnAnswer(std::string name, std::string summary, std::string protein, std::string carbs,
        std::string fat, std::string calories, std::string link,
        std::vector<Ingredient> ingredients,
        std::vector<std::string> instructions, const char* textureURL);

private:
    //every 20 words adds a new line
    std::string fixString(std::string toFix);

};

//loads an image from the file system
unsigned char* loadImage(const std::wstring& filename, int& width, int& height);

GLuint loadATexture(unsigned char* image, int width, int height);