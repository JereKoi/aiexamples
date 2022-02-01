# Specify minimum supported cmake version
cmake_minimum_required(VERSION 3.5)

# set the project name
project(GameAIExamples)

# add external pixelgridworld library to the project
# We need to use add_subdircetory, which finds CMakeLists.txt file from
# that folder and adds definitions from there to use of this project.
add_subdirectory("ext/pixelgridworld")
include_directories("ext/pixelgridworld/include")
include_directories(".")
# Lisätty astar_exercise vuoksi.
include_directories("${GLFW_SOURCE_DIR}/deps")

# add the executable, which only uses single file "main.cpp"
add_executable(hello_world main_hello_world.cpp)
add_executable(game_object main_game_object.cpp)
add_executable(turnstile_fsm main_turnstile_fsm.cpp)

add_executable(pixel_example main_pixel_example.cpp StateMachine.h LinearAlgebra.h Agent.h)
target_link_libraries(pixel_example PUBLIC pixelgridworld PUBLIC glfw)

# Random walk example
add_executable(random_walk_example main_random_walk_example.cpp)
target_link_libraries(random_walk_example PUBLIC pixelgridworld PUBLIC glfw)

# Djikstra walk example
add_executable(djikstra_example main_djikstra_example.cpp)
target_link_libraries(djikstra_example PUBLIC pixelgridworld PUBLIC glfw)

# Astar
add_executable(astar_example main_astar_example.cpp)
target_link_libraries(astar_example PUBLIC pixelgridworld PUBLIC glfw)

# A* Exercise
add_executable(astar_exercise main_exercise_pathfinding.cpp)
target_link_libraries(astar_exercise PUBLIC pixelgridworld PUBLIC glfw)





