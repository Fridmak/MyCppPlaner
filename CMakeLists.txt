cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MyCppPlanner")

set(CMAKE_PREFIX_PATH "C:/Program Files/6.5.2/msvc2019_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Добавляем поддержку UI файлов
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable (MyCppPlanner 
    "src/main.cpp" 
    "include/windows/main_window.h" 
    "src/windows/main_window.cpp"
    "include/windows/register_window.h"
    "src/windows/register_window.cpp"   "src/windows/managers/password_manager.cpp")

# Добавляем линковку с Qt
target_link_libraries(MyCppPlanner PRIVATE
    Qt6::Core
    Qt6::Widgets
)

# Добавляем пути к заголовочным файлам
target_include_directories(MyCppPlanner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MyCppPlanner PROPERTY CXX_STANDARD 20)
endif()