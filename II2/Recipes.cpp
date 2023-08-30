#include "Recipes.h"
#include<algorithm>
#include <vector>
#include <iostream>
#include <string>



size_t curlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

void FilterRecipes::getListOfIngredients(std::vector<Ingredient> inputTexts)
{

    for (int i = 0; i < this->countOfProducts; i++) {

        std::string ingredientName = inputTexts[i].ingredientName;
        float amount = inputTexts[i].amount;

        for (size_t i = 0; i < ingredientName.size(); i++)
        {
            if (ingredientName[i] == ' ')
            {
                ingredientName.replace(i, 1 , "%20");
 
            }
        }
        
        Ingredient ingredient(ingredientName, amount);
        ingredients.push_back(ingredient);
    }


}

int FilterRecipes::getSizeIDS()
{
    return ids.size();
}

FilterRecipes::FilterRecipes(std::vector<Ingredient> inputTexts)
{

	this->countOfProducts = inputTexts.size();
    std::string apiKey = "e98ba5dba07b4a9e91ca69ca02026c7e"; //"1f6ba6dcd97045f18035e73bc56c2888";
	std::string apiUrl = "https://api.spoonacular.com/recipes/findByIngredients";

    getListOfIngredients(inputTexts);
    std::string availableProducts = "";
    for (int i = 0; i < countOfProducts; i++) {
        availableProducts = availableProducts + ingredients[i].ingredientName;
        if (i < countOfProducts - 1) {
            availableProducts += ",";
        }
    }

    this->requestUrl = apiUrl + "?ingredients=" + availableProducts + "&apiKey=" + apiKey;
}



void FilterRecipes::searchIDS()
{

    CURL* curl = curl_easy_init();

    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);


        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


        CURLcode res = curl_easy_perform(curl);

 
        if (res == CURLE_OK) {

            Json::Reader read;
            Json::Value recipes;
            read.parse(response, recipes);



            int n = recipes.size();
            for (int j = 0; j < n; j++) {
                
                ids.push_back(recipes[j]["id"].asInt());

            }
        }

        else {
            std::cerr << "Api error request: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
}


std::vector<CreateAnAnswer> FilterRecipes::TraverseIDS()
{
    std::vector<CreateAnAnswer> listOfResults;
    for (int i = 0; i < ids.size(); i++) {
        Json::Reader read;
        Json::Value recipeInfo;
        std::string newRequestUrl = "https://api.spoonacular.com/recipes/" + std::to_string(ids[i]) + "/information?includeNutrition=true&apiKey=e98ba5dba07b4a9e91ca69ca02026c7e"; //1f6ba6dcd97045f18035e73bc56c2888
        CURL* curl = curl_easy_init();
        if (curl) {
            
            curl_easy_setopt(curl, CURLOPT_URL, newRequestUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);

            std::string response;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                
                read.parse(response, recipeInfo);

                std::string recipeName = recipeInfo["title"].asString();
                std::string protein = std::to_string(recipeInfo["nutrition"]["nutrients"][8]["amount"].asInt());
                std::string carbs = std::to_string(recipeInfo["nutrition"]["nutrients"][3]["amount"].asInt());
                std::string fat = std::to_string(recipeInfo["nutrition"]["nutrients"][1]["amount"].asInt());
                std::string calories = std::to_string(recipeInfo["nutrition"]["nutrients"][0]["amount"].asInt());
                std::string summary = recipeInfo["summary"].asString();
                std::string link = recipeInfo["spoonacularSourceUrl"].asString();
         
                std::vector<Ingredient> ingredients;
                std::vector<std::string> instructions;



                if (recipeInfo.isMember("extendedIngredients") && recipeInfo["extendedIngredients"].isArray())
                {
                    Json::Value& ingrArray = recipeInfo["extendedIngredients"];
                    for (Json::Value& element : ingrArray)
                    {
                        std::string ingredient = element["name"].asString();
                        float amoutPerServing = (int)(element["amount"].asFloat() * 100 + .5);
                        std::string unit = element["unit"].asString();

                        ingredients.push_back({ ingredient,(float)amoutPerServing / 100, unit});
                    }


                }

                if (recipeInfo.isMember("analyzedInstructions") && recipeInfo["analyzedInstructions"].isArray())
                {
                    const Json::Value& instr = recipeInfo["analyzedInstructions"];

                    for (const Json::Value& element : instr)
                    {
                        if (element.isMember("steps") && element["steps"].isArray())
                        {
                            const Json::Value& stepsArray = element["steps"];

                            for (const Json::Value& step : stepsArray)
                            {
                                if (step.isMember("step") && step["step"].isString())
                                {
                                    instructions.push_back(step["step"].asString());
                                }
                            }
                        }
                    }
                }


                CreateAnAnswer* currentRecipeAnswer = new CreateAnAnswer(recipeName, summary , protein, carbs,
                    fat, calories, link, ingredients, instructions ,recipeInfo["image"].asCString());

                listOfResults.push_back(*currentRecipeAnswer);

            }
            else {
                std::cerr << "API error: " << curl_easy_strerror(res) << std::endl;
            }
        }

    }
    return listOfResults;
}