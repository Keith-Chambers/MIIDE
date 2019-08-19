cmake_minimum_required(VERSION 3.8)

# Change below values for project
project(miide)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_PATH ${CMAKE_SOURCE_DIR}/src)
set(LIBS_PATH ${CMAKE_SOURCE_DIR}/libs)
set(IMGUI_PATH ${LIBS_PATH}/ImGui)
set(GLFW_PATH ${LIBS_PATH}/gl3w/)

add_executable(   ${PROJECT_NAME}  
        
                                    # Project source files
                                    ${SRC_PATH}/main.cpp 
                                    ${SRC_PATH}/uifileselector.cpp
                                    ${SRC_PATH}/uicomponent.cpp
                                    ${SRC_PATH}/keybindings.cpp
                                    ${SRC_PATH}/app.cpp
                                    ${SRC_PATH}/appcontrols.cpp
                                    ${SRC_PATH}/uifiletreepanel.cpp
                                    
                                    # ImGuiColorTextEdit
                                    ${LIBS_PATH}/ImGuiColorTextEdit/TextEditor.cpp
                                
                                    # Dear Imgui 
                                    ${IMGUI_PATH}/imgui_impl_opengl3.cpp 
                                    ${IMGUI_PATH}/imgui.cpp 
                                    ${IMGUI_PATH}/imgui_draw.cpp
                                    ${IMGUI_PATH}/imgui_widgets.cpp 
                                    ${IMGUI_PATH}/imgui_impl_glfw.cpp
                                    
                                    # GL3W
                                    ${GLFW_PATH}/GL/gl3w.c
                                )
                                
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_PATH}/)
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBS_PATH}/ImGuiColorTextEdit/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBS_PATH}/gl3w/)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBS_PATH}/glfw/include/)

target_link_libraries(${PROJECT_NAME} "-ldl")
target_link_libraries(${PROJECT_NAME} "-lGL")
target_link_libraries(${PROJECT_NAME} "-lX11")
target_link_libraries(${PROJECT_NAME} "-lpthread")
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/libs/glfw/libglfw3.a) # static link




