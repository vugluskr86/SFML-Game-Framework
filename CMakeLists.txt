cmake_minimum_required(VERSION 3.10)

#Set up project
project(GAME_NAME VERSION 1.0)

#Set executable
add_executable(${PROJECT_NAME}     
	"src/main.cpp" "src/Game.cpp" "src/ResourceManager/ResourceHolder.cpp" "src/States/StatePlaying.cpp" "src/Util/Animation.cpp" "src/Util/FileUtil.cpp" "src/Util/FPSCounter.cpp" "src/Util/Maths.cpp" "src/Util/Maths.cpp" "src/Game/Systems/SpriteRender.h" "src/Game/BaseSystem.h" "src/Game/Systems/SpriteRender.cpp" "src/Game/Systems/SpriteMove.h" "src/Game/Systems/SpriteMove.cpp"  "src/Game/Systems/PlayerInput.h" "src/Game/Systems/PlayerInput.cpp" "src/Game/Systems/MobBehaviour.h" "src/Game/Systems/MobBehaviour.cpp"  "src/Game/Systems/BulletCollision.h" "src/Game/Systems/BulletCollision.cpp" "src/Game/Components/Weapon.h"
 "src/Game/Components/PhysicBody.h" "src/Util/Box2dDebugDraw.h"  "src/Game/eCollideObjectGroups.h"
 )

#Set C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)


set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

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
find_package(box2d CONFIG REQUIRED)
find_package(Threads)
find_package(OpenGL REQUIRED)
find_package(SFML COMPONENTS audio system window graphics CONFIG REQUIRED)

include_directories(
    ${SFML_INCLUDE_DIR}
    ${BOX2D_INCLUDE_DIR}
)

add_subdirectory(deps/imgui)
add_subdirectory(deps/imgui_sfml)
add_subdirectory(deps/glm)
add_subdirectory(deps/entt)
add_subdirectory(deps/optick)

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
    box2d::box2d
    sfml-audio
    sfml-system
    sfml-network
    sfml-graphics
    sfml-window
    ${OPENGL_LIBRARIES} 
)

#if(MSVC)
#    target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/deps/optick/bin/vs2019/x64/Debug/OptickCore.lib")
#endif()