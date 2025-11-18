cmake_minimum_required(VERSION 3.20)
project(mini_sistema_rh)

# ==============================================================
#  Compiler Settings
# ==============================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>, 
        $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>, 
        $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
    )
endif()

# ==============================================================
#  Executable
# ==============================================================

add_executable(mini_sistema_rh)

# ==============================================================
#  Source Files
# ==============================================================

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/models/**/*.cpp
    ${CMAKE_SOURCE_DIR}/models/**/*.hpp
    ${CMAKE_SOURCE_DIR}/controllers/**/*.cpp
    ${CMAKE_SOURCE_DIR}/controllers/**/*.hpp
    ${CMAKE_SOURCE_DIR}/views/**/*.cpp
    ${CMAKE_SOURCE_DIR}/views/**/*.hpp
)

target_sources(mini_sistema_rh PRIVATE ${SOURCES})

# ==============================================================
#  Include Directories
# ==============================================================

target_include_directories(mini_sistema_rh PRIVATE ${CMAKE_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR})

# ==============================================================
#  Dependencies
# ==============================================================

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(mini_sistema_rh PRIVATE nlohmann_json::nlohmann_json)

find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(mini_sistema_rh PRIVATE unofficial-sodium::sodium)

# ==============================================================
#  Copy Folder
# ==============================================================

add_custom_command(
    TARGET mini_sistema_rh POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/files"
        "$<TARGET_FILE_DIR:mini_sistema_rh>/files"
)
