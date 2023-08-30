#include "UseImgui.h"

void UseImgui::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsLight();
}

void UseImgui::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

int numButtons;
std::vector<int> amounts(numButtons);
std::vector<Ingredient> inputTexts(numButtons, {"",0});


std::vector<CreateAnAnswer> recipesOutput;

bool UseImgui::IsClickedText(const char* label)
{
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 2.0f, 2.0f), label);
	return ImGui::IsItemClicked();
}

void UseImgui::Update(unsigned char* cake, int width, int height)
{
	static bool openFirstWindow = true;
	static bool openSecondWindw = false;
	static bool error = false;

	ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
	ImGui::SetNextWindowSize(viewportSize);
	ImGui::SetNextWindowPos(ImVec2(0,0), 0, ImVec2(0, 0));
	if (openFirstWindow)
	{
		ImGui::Begin("First window", &openFirstWindow, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

		ImGui::InputInt("How many ingredients do you have?", &numButtons);
		if (numButtons < 0)
			numButtons = 0;
		inputTexts.resize(numButtons);
		amounts.resize(numButtons);
		for (int i = 0; i < numButtons; ++i) {

			ImGui::PushID(i);

			char buffer[256];

			ImGui::Text("Ingredient name:");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(250);

			strncpy_s(buffer, inputTexts[i].ingredientName.c_str(), sizeof(buffer));

			if (ImGui::InputText("##Ingredient name", buffer, sizeof(buffer))) {
				inputTexts[i].ingredientName = buffer;
				std::cout << "Button " << i << " text: " << inputTexts[i].ingredientName << std::endl;
			}


			ImGui::PopID();
		}

		if (ImGui::Button("Enter", ImVec2(100, 40)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
			std::cout << "Enter button clicked!" << std::endl;

			FilterRecipes recipes(inputTexts);
			recipes.searchIDS();
			if (recipes.getSizeIDS() == 0)
				error = true;

			recipesOutput = recipes.TraverseIDS();
			openFirstWindow = false;
			openSecondWindw = true;
		}

		GLuint cakeTexture = loadATexture(cake, width, height);
		ImVec2 windowSize = ImGui::GetIO().DisplaySize;
		ImVec2 centerPos = ImVec2((windowSize.x - width) * 0.5f, (windowSize.y - height) * 0.5f);

		ImGui::SetCursorPos(centerPos);
		ImGui::Image((void*)(intptr_t)cakeTexture, ImVec2(static_cast<float>(width), static_cast<float>(height)));

		ImGui::End();
	}
	if (openSecondWindw)
	{

		ImGui::Begin("Second window", &openSecondWindw, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

		if (!error)
		{
			for (const auto& recipe : recipesOutput)
			{
				if (ImGui::TreeNode(recipe.recipeName.c_str()))
				{
					ImGui::Image((void*)(intptr_t)recipe.texture, ImVec2(400, 300));

					if (ImGui::TreeNode("Summary"))
					{
						ImGui::Text(recipe.summary.c_str());
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Ingredients"))
					{
						for (size_t i = 0; i < recipe.ingredients.size(); i++)
						{
							char buffer[256];
							snprintf(buffer, sizeof(buffer), "Name: %s, needed amount: %f %s", recipe.ingredients[i].ingredientName.c_str(),
								recipe.ingredients[i].amount, recipe.ingredients[i].unit.c_str());

							ImGui::BulletText("%s", buffer);

						}
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Instructions"))
					{
						for (size_t i = 0; i < recipe.instructions.size(); i++)
						{
							ImGui::BulletText("%s", recipe.instructions[i].c_str());
						}
						ImGui::TreePop();
					}

					ImGui::BulletText("Calories: %s", recipe.calories.c_str());
					ImGui::BulletText("Protein: %s g.", recipe.protein.c_str());
					ImGui::BulletText("Carbs: %s g.", recipe.carbs.c_str());
					ImGui::BulletText("Fat: %s g.", recipe.fat.c_str());

					if (IsClickedText("		Click here to open the recipe page in your browser"))
					{
						std::cout << "Link clicked" << std::endl;

						ShellExecuteA(NULL, "open", recipe.link.c_str(), NULL, NULL, SW_SHOWNORMAL);

					}


					ImGui::TreePop();
				}
			}
		}
		else {
			ImGui::Text("A recipe with the given ingredients does not exist. Try again by pressing return!", 19.0f);

		}
		if (ImGui::Button("return", ImVec2(100, 40))) {
			std::cout << "return button clicked!" << std::endl;
			error = false;
			openFirstWindow = true;
			openSecondWindw = false;
		}
		ImGui::End();

	}


}

void UseImgui::Render(GLFWwindow* window)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UseImgui::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
