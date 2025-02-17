cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MyCppPlanner")

set(CMAKE_PREFIX_PATH "C:\\Cpp\\6.8.2\\mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
 
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable (MyCppPlanner 
    "src/main.cpp"

    "include/managers/app_state_manager.h"
    "include/managers/notification_manager.h"
    "include/managers/password_manager.h"
    "include/managers/settings_manager.h"
    "include/managers/tasks_manager.h"
    "include/managers/window_manager.h"

    
    

    "src/managers/app_state_manager.cpp"
    "src/managers/notification_manager.cpp"
    "src/managers/password_manager.cpp"
    "src/managers/settings_manager.cpp"
    "src/managers/tasks_manager.cpp"
    "src/managers/window_manager.cpp"

    
    
 "include/windows/register_window.h" "src/windows/register_window.cpp")

#Qt lib
target_link_libraries(MyCppPlanner PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(MyCppPlanner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MyCppPlanner PROPERTY CXX_STANDARD 20)
endif()