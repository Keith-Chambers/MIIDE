#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include <functional>
#include <optional>
#include <iostream>

#include <imgui.h>

namespace anagami {

    class UIComponent
    {
    public:
        UIComponent(const std::string& label);

        void setRemoveCallback(std::function<void(void)> removeCallback);
        virtual void display()=0;
        virtual void displayInWindow(ImGuiWindowFlags windowFlags)=0;

        virtual ~UIComponent();

        const std::string mLabel;

    protected:
        std::optional< std::function<void(void)> > mRemoveCallback = std::nullopt;
    };
}

#endif // UICOMPONENT_H
