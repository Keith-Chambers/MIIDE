#ifndef APP_H
#define APP_H

#include <fstream>

#include <ImGuiColorTextEdit/TextEditor.h>
#include <GLFW/glfw3.h>

class App
{
public:
    App();

    bool loadFile(std::string path)
    {
        std::ifstream t(path);

        if(! t.good())
        {
            return false;
        }

        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        editor.SetText(str);
        openFileLocation = path;

        return true;
    }

    bool save()
    {
        auto textToSave = editor.GetText();

        std::ofstream currentFile;
        currentFile.open(openFileLocation);

        if(! currentFile.is_open()) return false;

        currentFile << textToSave;
        currentFile.close();

        return true;
    }

    bool saveAs(std::string path)
    {
        auto textToSave = editor.GetText();

        std::ofstream currentFile;
        currentFile.open(path);

        if(! currentFile.is_open()) return false;

        currentFile << textToSave;
        currentFile.close();

        openFileLocation = path;

        return true;
    }

    TextEditor editor;
    GLFWwindow* window;

    std::string selection;
    std::string openFileLocation;
    int cursorIndex = 0;
};

#endif // APP_H
