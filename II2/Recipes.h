#pragma once

#include<curl/curl.h>
#include<json/json.h>
#include<json/value.h>


#include<algorithm>
#include <vector>
#include <iostream>
#include <string>
#include"CreateAnAnswer.h"




size_t curlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* response);

class FilterRecipes
{

public:
	FilterRecipes(std::vector<Ingredient> inputTexts);

    //gets the ID's of recipes made with the products that the user inputs
	void searchIDS();

    //looks through the extended information for every recipe
    std::vector<CreateAnAnswer> TraverseIDS();

    void getListOfIngredients(std::vector<Ingredient> inputTexts);
    int getSizeIDS();

private:

    int countOfProducts;
	std::vector<int>ids;
    std::vector<Ingredient>ingredients;
    std::string requestUrl;
};
