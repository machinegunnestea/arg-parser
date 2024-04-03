# В современном CMake рекомендуется сразу задавать нужную версию CMake.
cmake_minimum_required(VERSION 3.28)

# Говорим CMake что за проект.
project(args_parse_test_app LANGUAGES CXX)

# Определяем исполнимый файл и из чего он состоит.
add_executable(_unit_test_args_parse main.cpp)

target_link_libraries(_unit_test_args_parse
    PRIVATE
        # Библиотека args_parse должна быть прилинкована к этому исполнимому файлу.
        args_parse
        # Библиотека Catch2 должна быть прилинкована к этому исполнимому файлу.
        Catch2::Catch2WithMain
)

# Посредством этой функции мы сообщаем CTest, что у нас есть еще один тест.
catch_discover_tests(_unit_test_args_parse)
