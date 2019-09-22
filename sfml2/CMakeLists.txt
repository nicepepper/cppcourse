# Минимальная версия CMake: 3.8
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# Имя проекта: sfml-lab-2
project(sfml-lab-2)

# Подкаталоги содержит ещё один CMakeLists.txt
add_subdirectory(sfml2.1)
add_subdirectory(sfml2.2)