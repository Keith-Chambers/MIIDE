#ifndef APPCONTROLS_H
#define APPCONTROLS_H

#include <iostream>

#include <GLFW/glfw3.h>
#include <app.h>

namespace controls
{
    void exitApplication(App app);
    void copySelection(App app);
    void cutSelection(App app);
    void pasteSelection(App app);
    void saveFile(App app);
    void selectAll(App app);
    void backspace(App app);
}



#endif // APPCONTROLS_H
