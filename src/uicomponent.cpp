#include <uicomponent.h>

namespace anagami {

    UIComponent::UIComponent(const std::string& label)
        : mLabel { label }
    {

    }

    void UIComponent::setRemoveCallback(std::function<void(void)> removeCallback)
    {
        mRemoveCallback = removeCallback;
    }

    UIComponent::~UIComponent()
    {
    }
}
