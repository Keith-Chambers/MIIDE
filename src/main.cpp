#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <array>
#include <locale>
#include <memory>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <ImGuiColorTextEdit/TextEditor.h>

#include <uifileselector.h>
#include <app.h>
#include <keybindings.h>

static App app;

static int termEnterCallback(ImGuiInputTextCallbackData *data)
{
    std::array<char, 128> resultBuffer;

    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(data->Buf, "r"), pclose);
    if (!pipe) {
        std::cout << "Error executing command" << std::endl;
    }

    while (fgets(resultBuffer.data(), static_cast<int32_t>(resultBuffer.size()), pipe.get()) != nullptr) {
        result += resultBuffer.data();
    }

    std::cout << "SH: " << result << std::endl;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static std::string binaryString(int32_t val)
{
    std::string result;
    for(int i = 0; i < 32; i++) {
        result += (val & (1 << i)) ? '1' : '0';
    }

    return result;
}

static std::string binaryString(int64_t val)
{
    std::string result;
    for(int i = 0; i < 64; i++) {
        result += (val & (1 << i)) ? '1' : '0';
    }

    return result;
}

static void keyCallback(App app, GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action != GLFW_PRESS) return;

//    std::cout << "key: " << key << " " << binaryString(key) << std::endl;
//    std::cout << "mod: " << mods << " " << binaryString(mods) << std::endl;

    int64_t packedKeyModCombo = int64FromInt32(key, mods);

//    printf("packed: %ld %s\n", packedKeyModCombo, binaryString(packedKeyModCombo).c_str());
//    fflush(stdout);

    if(action == GLFW_PRESS)
    {
        if(pressBindings.find(packedKeyModCombo) == pressBindings.end())
        {
//            std::cout << "Unmapped key code -> " << packedKeyModCombo << std::endl;
            return;
        }

        pressBindings[packedKeyModCombo](app);
    }

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    keyCallback(app, window, key, scancode, action, mods);
}

bool loadFile(TextEditor &editor, const std::string & fileName)
{
    std::ifstream t(fileName);
    
    if(! t.good()) 
    {
        return false;
    }

    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    editor.SetText(str);
    
    return true;
}

static std::string PATH = "/home/keith/Code/app_development/imgui_ide/";

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    static std::string str;

    wchar_t a = codepoint;
    str = static_cast<char>(a);
    app.editor.InsertText(str);

    std::wcout << a;
}

int main()
{
    setlocale(LC_ALL, "");

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Make Window Unresizeable

    // Create window with graphics context
    app.window = glfwCreateWindow(1280, 720, " ImGui IDE", nullptr, nullptr);
    if (app.window == nullptr)
        return 1;

//    runTests();
    setupBindings();
    
    glfwMakeContextCurrent(app.window);
    glfwSwapInterval(1); // Enable vsync

    glfwSetKeyCallback(app.window, key_callback);
    //glfwSetCharCallback(app.window, character_callback);

    if ( gl3wInit() != 0 )
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    ImGui::StyleColorsDark();

    /* Set font */

    ImFont* font1 = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/Inconsolata-Regular.ttf", 14);

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(app.window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Set language syntax to C++
    app.editor.SetLanguageDefinition( TextEditor::LanguageDefinition::CPlusPlus() );
       
    app.editor.SetHandleKeyboardInputs(true);
    app.editor.SetHandleMouseInputs(true);
    
    ImVec2 vec(900, 500);
    
    app.loadFile(PATH + "testfile.txt");
	
	bool selectFile = false;
	
    anagami::UIFileSelector loadProjectSelector("Load Project", [&](std::filesystem::path path){

        std::cout << "opening -> " << path << std::endl;
        selectFile = false;
        loadFile(app.editor, path);
        app.openFileLocation = path;
    
    }, ".", false );

    loadProjectSelector.attachOnCloseCallback([](){ ImGui::CloseCurrentPopup(); });

    auto projectFileSelectedCallback = [&](std::filesystem::path path)
    {
        std::cout << "Changing to " << path.filename() << std::endl;
        app.loadFile(path);
    };

    anagami::UIFileSelector projectFileTree("Project File Tree",
                                            projectFileSelectedCallback,
                                            ".");

    while (!glfwWindowShouldClose(app.window))
    {
        //glfwPollEvents();
        glfwWaitEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(font1);
        
        auto cpos = app.editor.GetCursorPosition();
        ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);
        
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
                if(ImGui::MenuItem("Open")) 
                {
                         selectFile = true;
                         loadProjectSelector.displayInWindow(0);
                }
                
                if (ImGui::MenuItem("Save", "Ctrl-S"))
				{
                    app.save();
				}
				if (ImGui::MenuItem("Quit", "Alt-F4"))
					break;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
                bool ro = app.editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                    app.editor.SetReadOnly(ro);
				ImGui::Separator();

                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && app.editor.CanUndo()))
                    app.editor.Undo();
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && app.editor.CanRedo()))
                    app.editor.Redo();

				ImGui::Separator();

                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, app.editor.HasSelection()))
                    app.editor.Copy();
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && app.editor.HasSelection()))
                    app.editor.Cut();
                if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && app.editor.HasSelection()))
                    app.editor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                    app.editor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
                    app.editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(app.editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
                    app.editor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
                    app.editor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
                    app.editor.SetPalette(TextEditor::GetRetroBluePalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

        if(ImGui::Button("Run"))
        {
            std::cout << "Running" << std::endl;
        }

        ImGui::SameLine();

        if(ImGui::Button("Build"))
        {
            std::cout << "Building" << std::endl;
        }

        ImGui::SameLine();

        if(ImGui::Button("Format"))
        {
            std::cout << "Formatting" << std::endl;
        }

        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, app.editor.GetTotalLines(),
            app.editor.IsOverwrite() ? "Ovr" : "Ins",
            app.editor.CanUndo() ? "*" : " ",
            app.editor.GetLanguageDefinition().mName.c_str(), app.openFileLocation.c_str());
        
        if(selectFile == true) 
        {
            loadProjectSelector.displayInWindow(0);
        }
        
        app.editor.Render("TextEditor");

        ImGui::PopFont();

		ImGui::End();

        ImGui::Begin("File Tree", nullptr, ImGuiWindowFlags_NoTitleBar);

        projectFileTree.displayInTree(3);

        ImGui::End();

        ImGui::Begin("###Embeded Terminal", nullptr, ImGuiWindowFlags_NoTitleBar);

        static char command[256];
        std::array<char, 128> resultBuffer;

        uint32_t commandSize = IM_ARRAYSIZE(command);
        if(ImGui::InputTextMultiline("###Term input", command, commandSize, ImVec2(400, 100), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine))
        {
            std::cout << "Running '" << command << "'" << std::endl;
            std::string result;
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
            if (!pipe) {
                std::cout << "Error executing command" << std::endl;
            }

            while (fgets(resultBuffer.data(), static_cast<int32_t>(commandSize), pipe.get()) != nullptr) {
                result += resultBuffer.data();
            }

            std::cout << "SH: " << result << std::endl;
        }

        for(uint16_t i = commandSize; i < commandSize; i++)
        {
            if(false && command[i] == '\n') // TODO:
            {
                std::string result;
                std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
                if (!pipe) {
                    std::cout << "Error executing command" << std::endl;
                }

                while (fgets(resultBuffer.data(), static_cast<int32_t>(commandSize), pipe.get()) != nullptr) {
                    result += resultBuffer.data();
                }

                std::cout << "SH: " << result << std::endl;
            }

            // typedef int (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData *data);
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(app.window);
        glfwGetFramebufferSize(app.window, &display_w, &display_h);
        
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(app.window);
        glfwSwapBuffers(app.window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(app.window);
    glfwTerminate();

    return 0;
}
