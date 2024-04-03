# В современном CMake рекомендуется сразу задавать нужную версию CMake.
cmake_minimum_required(VERSION 3.28)

# Говорим CMake что за проект.
project(args_parse_lab LANGUAGES CXX)

# Назначаем нужный нам стандарт C++.
set(CMAKE_CXX_STANDARD 17)
# Указываем, что нужен именно этот стандарт, на предыдущие откатываться нельзя.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Запрещаем использование нестандартных расширений.
set(CMAKE_CXX_EXTENSIONS OFF)

# Определяем куда будут скидываться результаты компиляции.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Дополнительные настройки для компилятора VC++
if (MSVC)
    # Строгое следование стандарту.
    add_compile_options(/permissive-)
    # Повышаем уровень предупреждений.
    add_compile_options(/W3)
    # И исходники в UTF-8, и строки в результирующем файле в UTF-8.
    add_compile_options(/utf-8)
endif()

# Нам нужен Catch2.
# ПРИМЕЧАНИЕ: опция CONFIG здесь нужна обязательно, без нее при компиляции
# под Windows будут недоступны заголовочные файлы Catch2.
find_package(Catch2 CONFIG REQUIRED)

# Подключаем подпроекты.
add_subdirectory(args_parse)
add_subdirectory(demo)

# Разрешаем поддержку unit-тестов.
include(CTest)
include(Catch)
enable_testing()

# Подключаем проект с тестами.
add_subdirectory(test)

