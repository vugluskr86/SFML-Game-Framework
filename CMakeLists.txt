cmake_minimum_required(VERSION 3.10)

#Set up project
project(GAME_NAME VERSION 1.0)

#Set executable
add_executable(${PROJECT_NAME}     
    src/main.cpp
    src/Game.cpp

    src/GUI/Button.cpp
    src/GUI/StackMenu.cpp
    src/GUI/Textbox.cpp
    src/GUI/Widget.cpp
    
    src/ResourceManager/ResourceHolder.cpp

    src/States/StatePlaying.cpp

    src/Util/Animation.cpp
    src/Util/FileUtil.cpp
    src/Util/FPSCounter.cpp
    src/Util/Maths.cpp
    src/Util/Maths.cpp
 "src/Game/Systems/RenderSystem.h" "src/Game/BaseSystem.h" "src/Game/Systems/RenderSystem.cpp" "src/Game/Systems/MoveSystem.h" "src/Game/Systems/MoveSystem.cpp"  "src/Game/Systems/PlayerInputSystem.h" "src/Game/Systems/PlayerInputSystem.cpp" "src/Game/Systems/MobAISystem.h" "src/Game/Systems/MobAISystem.cpp")

#Set C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

#Set flags
if(MSVC)
  	target_compile_options(${PROJECT_NAME} PRIVATE 
    	/W3 /WX)
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE 
		-Wall -Wextra -pedantic -Werror)		#Warning flags
endif()

#Set module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

#Find libraries
find_package(Threads)
find_package(OpenGL REQUIRED)
find_package(SFML REQUIRED audio network graphics window system)

include_directories(${SFML_INCLUDE_DIR})

add_subdirectory(deps/imgui)
add_subdirectory(deps/imgui_sfml)

add_subdirectory(deps/glm)
add_subdirectory(deps/entt)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    deps
)

#Finally
target_link_libraries(${PROJECT_NAME} 
    imgui
    imgui_sfml
    Threads::Threads 
    ${OPENGL_LIBRARIES} 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
)
