#ifndef UIFILESELECTOR_H
#define UIFILESELECTOR_H

#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include <algorithm>
#include <optional>

#include "uicomponent.h"

#include <imgui.h>

namespace anagami {

    class UIFileSelector : public anagami::UIComponent
    {
    public:
        UIFileSelector( const std::string& label,
                        const std::function<void(std::filesystem::path)>& callback,
                        const std::filesystem::path& startPath = ".",
                        bool showHiddenFiles = false,
                        const std::vector<std::string>& acceptedExtensions = {});

        void display();
        void displayInTree(uint16_t depth);
        void constructTreeLevel(std::filesystem::path path, uint16_t depth);
        void displayInWindow(ImGuiWindowFlags windowFlags);

        void setFileColor( ImColor color );
        void setFolderColor( ImColor color );

        void attachOnCloseCallback(std::function<void(void)> onCloseCallback);

    private:
        std::function<void(std::filesystem::path)> mFileSelectedCallback;
        std::filesystem::path mStartPath;
        bool mShowHiddenFiles;
        std::vector<std::string> mAcceptedExtensions;

        std::filesystem::path mCurrentPath;

        std::optional<std::function<void(void)>> mOnCloseCallback;

        ImColor mFolderColor = { 255, 255, 255 };   /* White */
        ImColor mFileColor = { 102, 255, 51 };      /* Light Green */

        bool validType( std::filesystem::directory_entry entry ) const;
    };
}

#endif // UIFILESELECTOR_H
