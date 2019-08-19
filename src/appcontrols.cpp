#include "appcontrols.h"

void controls::exitApplication(App app)
{
    glfwSetWindowShouldClose(app.window, 1);
}

void controls::copySelection(App app)
{
    app.editor.Copy();
}

void controls::pasteSelection(App app)
{
    app.editor.Paste();
}

void controls::cutSelection(App app)
{
    app.editor.Cut();
}

void controls::saveFile(App app)
{
    std::cout << "Saving file.." << std::endl;

    app.save();
}

void controls::selectAll(App app)
{
    app.editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(app.editor.GetTotalLines(), 0));
}

void controls::backspace(App app)
{
    std::cout << "Working" << std::endl;
//    app.editor.Backspace();
}
