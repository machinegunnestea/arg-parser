# В современном CMake рекомендуется сразу задавать нужную версию CMake.
cmake_minimum_required(VERSION 3.27)

# Говорим CMake что за проект.
project(directory_app LANGUAGES CXX)

# Определяем исполнимый файл и из чего он состоит.
add_executable(directory main.cpp )

# Библиотека args_parse должна быть прилинкована к этому исполнимому файлу.
target_link_libraries(directory PRIVATE args_parse)

