#include "UseImgui.h"
#include <ShlObj.h>

using namespace std;


void CreateDesktopShortcut(const wchar_t* targetPath, const wchar_t* shortcutName, const wchar_t* iconPath) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr)) {
		IShellLink* pShellLink;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pShellLink));

		if (SUCCEEDED(hr)) {
			pShellLink->SetPath(targetPath);
			pShellLink->SetDescription(L"Recipe ideas!");
			pShellLink->SetIconLocation(iconPath, 0);

			IPersistFile* pPersistFile;
			hr = pShellLink->QueryInterface(IID_PPV_ARGS(&pPersistFile));

			if (SUCCEEDED(hr)) {
				WCHAR lnkPath[MAX_PATH];
				ExpandEnvironmentStringsW(L"%USERPROFILE%\\Desktop\\", lnkPath, MAX_PATH);
				wcscat_s(lnkPath, MAX_PATH, shortcutName);
				wcscat_s(lnkPath, MAX_PATH, L".lnk");

				hr = pPersistFile->Save(lnkPath, TRUE);
				pPersistFile->Release();
			}

			pShellLink->Release();
		}

		CoUninitialize();
	}
}



int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
	{

	if (!glfwInit())
		return 1;


	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


	GLFWwindow* window = glfwCreateWindow(1280, 720, "FOOD MMM", NULL, NULL);
	if (window == NULL)
		return 1;


	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); 

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw("Unable to context to OpenGL");

	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);


    UseImgui myImgui;
    myImgui.Init(window,glsl_version);

	//Adding font:
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.0f);
	io.FontDefault = io.Fonts->Fonts.back();
	ImGuiStyle& style = ImGui::GetStyle();


	//getting the full path of the icons
	wchar_t  buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring fullPath(buffer);
	std::wstring folderPath = fullPath.substr(0, fullPath.find_last_of(L"\\/"));
	std::wstring iconPath = folderPath.append(L"\\donut.ico");
	folderPath = fullPath.substr(0, fullPath.find_last_of(L"\\/"));
	std::wstring logoPath = folderPath.append(L"\\donut.png");
	folderPath = fullPath.substr(0, fullPath.find_last_of(L"\\/"));
	std::wstring cakePath = folderPath.append(L"\\cat.png");

	//loading the icons
	int width, height;
	unsigned char* pixels = loadImage((logoPath.data()), width, height);

	int width2, height2;
	unsigned char* cake = loadImage((cakePath.data()), width2, height2);


	if (pixels)
	{
		GLFWimage image;
		image.width = width;
		image.height = height;
		image.pixels = pixels;
		glfwSetWindowIcon(window, 1, &image);
	}

	//setting colors for UI
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.98f, 0.96f, 0.93f, 1.00f);     // Light cream background color;
	style.Colors[ImGuiCol_Header] = ImVec4(0.88f, 0.55f, 0.37f, 1.00f);  // Header color (orange/brown);
	style.Colors[ImGuiCol_Text] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);    // Text color (dark grey);
	style.Colors[ImGuiCol_Button] = ImVec4(0.94f, 0.61f, 0.45f, 1.00f);   // Button color (light orange);

	style.WindowRounding = 4.0f;
	style.FrameRounding = 2.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.FramePadding = ImVec2(6.0f, 4.0f);
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 4.0f;

	//main while loop
    while (!glfwWindowShouldClose(window))
    {
		glfwPollEvents();
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		myImgui.NewFrame();
        myImgui.Update(cake, width2, height2);
        myImgui.Render(window);
		glfwSwapBuffers(window);
    }

    myImgui.Shutdown();


	
	CreateDesktopShortcut(buffer, L"FOOD MMMM", iconPath.data());
    return 0;

}



