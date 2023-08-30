#pragma once
#include<string>


struct Ingredient {
    std::string ingredientName;
    float amount = 0;
    std::string unit;
    Ingredient() : ingredientName("") {};
    Ingredient(std::string name, float amt, std::string unit = "") : ingredientName(name),
        amount(amt), unit(unit) {};
};