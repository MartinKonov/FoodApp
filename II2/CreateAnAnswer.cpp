#include "CreateAnAnswer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

CreateAnAnswer::CreateAnAnswer()
    : recipeName(""), summary(""), protein(""), carbs(""),
    fat(""), calories(""),link(""), ingredients(), texture(0) {}


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::vector<unsigned char>* data) {
    size_t totalSize = size * nmemb;
    data->insert(data->end(), static_cast<unsigned char*>(contents), static_cast<unsigned char*>(contents) + totalSize);
    return totalSize;
}


CreateAnAnswer::CreateAnAnswer(std::string name, std::string summary,std::string protein,std::string carbs,
    std::string fat, std::string calories, std::string link,
    std::vector<Ingredient> ingredients, std::vector<std::string> instr, const char* textureURL)
{
	this->recipeName = fixString(name);
	this->summary = fixString(summary);
    this->protein = protein;
    this->carbs = carbs;
    this->fat = fat;
	this->calories = calories;
    this->link = link;
	this->ingredients = ingredients;

    for (size_t i = 0; i < instr.size(); i++)
    {
        this->instructions.push_back(fixString(instr[i]));
    }

    std::vector<unsigned char> imageData;
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, textureURL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageData);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to download image: " << curl_easy_strerror(res) << std::endl;
            return;
        }
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return;
    }
    
    int width=0, height=0, channels;
    unsigned char* image_data = stbi_load_from_memory(imageData.data(), imageData.size(), &width, &height ,&channels, 0);
    if (!image_data) {
        std::cerr << "Failed to load image: " << textureURL << std::endl;
        return;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);


}

std::string CreateAnAnswer::fixString(std::string toFix)
{
    size_t wordCount = 0;
    for (size_t i = 0; i < toFix.size(); i++)
    {
        if (toFix[i] == ' ')
        {
            wordCount++;
            if (wordCount == 20)
            {
                toFix[i] = '\n';
                wordCount = 0;
            }
        }
    }

    return toFix;
}

unsigned char* loadImage(const std::wstring& filename, int& width, int& height)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string narrowFilename = converter.to_bytes(filename);

    int channels;
    return stbi_load(narrowFilename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
}

GLuint loadATexture(unsigned char* image, int width, int height)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}
