#include <uifileselector.h>

namespace anagami {

    UIFileSelector::UIFileSelector( const std::string& label,
                    const std::function<void(std::filesystem::path)>& callback,
                    const std::filesystem::path& startPath,
                    bool showHiddenFiles,
                    const std::vector<std::string>& acceptedExtensions)
        : anagami::UIComponent (label),
          mFileSelectedCallback { callback },
          mStartPath { startPath },
          mShowHiddenFiles { showHiddenFiles },
          mAcceptedExtensions { acceptedExtensions },
          mCurrentPath { startPath }
    {

    }

    bool UIFileSelector::validType( std::filesystem::directory_entry entry ) const
    {
        if(! mShowHiddenFiles && entry.path().filename().string()[0] == '.')
            return false;

        if(entry.is_directory())
            return true;

        if(mAcceptedExtensions.empty())
            return true;

        for(const std::string& extension : mAcceptedExtensions)
            if( extension == entry.path().extension() )
                return true;

        return false;
    }

    bool directoryEntryCompare( const std::filesystem::directory_entry& first, const std::filesystem::directory_entry second )
    {
        if( first.is_directory() && ! second.is_directory() )
            return true;

        if( ! first.is_directory() && second.is_directory() )
            return false;

        return first < second;
    }

    void UIFileSelector::attachOnCloseCallback(std::function<void(void)> onCloseCallback)
    {
        mOnCloseCallback = std::make_optional<std::function<void(void)>>( onCloseCallback );
    }

    void UIFileSelector::constructTreeLevel(std::filesystem::path path, uint16_t depth = 3)
    {
        if(depth <= 0) return;

        ImGui::PushID(depth);

        ImGui::Columns(1, "Project Files Column", true);

        std::filesystem::directory_iterator itr(path);

        std::vector<std::filesystem::directory_entry> fileNames;
        for(auto& entry: itr)
            fileNames.push_back( entry );

        std::sort( fileNames.begin(), fileNames.end(), directoryEntryCompare );

        uint16_t id = 0;

        for(auto& entry : fileNames)
        {
            id++;

            if( validType(entry))
            {
                ImColor color = ( entry.is_directory() ) ? mFolderColor : mFileColor;
                ImGui::PushStyleColor(ImGuiCol_Text, color.Value);

                ImGui::PushID(id);

                if(entry.is_directory() && ImGui::TreeNode(entry.path().filename().c_str()))
                {
                    constructTreeLevel(entry.path(), depth - 1);
                    ImGui::TreePop();
                }

                ImGui::PopID();

                if(!entry.is_directory() && ImGui::Selectable(entry.path().filename().c_str(), false, ImGuiSelectableFlags_DontClosePopups) )
                {

                    if(! entry.is_directory() )
                    {
                        mFileSelectedCallback( entry.path() );
                    }
                }

                ImGui::PopStyleColor();

                ImGui::NextColumn();
            }
        }

        ImGui::Columns();

        ImGui::PopID();
    }

    void UIFileSelector::displayInTree(uint16_t depth = 3)
    {
        if(depth <= 0) return;

        constructTreeLevel(mCurrentPath, depth);
    }

    void UIFileSelector::display()
    {

        if( ImGui::Button(" UP DIR ") && mCurrentPath.has_parent_path())
            mCurrentPath = mCurrentPath.parent_path();

        if( mOnCloseCallback.has_value() )
        {
            ImGui::SameLine();
            ImGui::Indent( 200.0f );

            if( ImGui::Button(" X ") )
                mOnCloseCallback.value()();

            ImGui::Unindent( 200.0f );
        }

        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Columns(1, "FileListColumn", true);

        std::filesystem::directory_iterator itr(mCurrentPath);

        std::vector<std::filesystem::directory_entry> fileNames;
        for(auto& entry: itr)
            fileNames.push_back( entry );

        std::sort( fileNames.begin(), fileNames.end(), directoryEntryCompare );

        for(auto& entry : fileNames)
        {
            if( validType(entry))
            {
                ImColor color = ( entry.is_directory() ) ? mFolderColor : mFileColor;
                ImGui::PushStyleColor(ImGuiCol_Text, color.Value);

                if( ImGui::Selectable(entry.path().filename().c_str(), false, ImGuiSelectableFlags_DontClosePopups) )
                {

                    if(! entry.is_directory() )
                    {
                        ImGui::PopStyleColor();
                        mFileSelectedCallback( entry.path() );
                        break;
                    }

                    mCurrentPath = entry.path();
                    ImGui::PopStyleColor();
                    break;
                }

                ImGui::PopStyleColor();

                ImGui::NextColumn();
            }
        }

        ImGui::Columns();
    }

    void UIFileSelector::setFileColor( ImColor color )
    {
        mFileColor = color;
    }

    void UIFileSelector::setFolderColor( ImColor color )
    {
        mFolderColor = color;
    }

    void UIFileSelector::displayInWindow(ImGuiWindowFlags windowFlags)
    {
        ImGui::Begin( "FileSelector", nullptr, windowFlags );
        display();
        ImGui::End();
    }

}
