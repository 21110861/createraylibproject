#include <fstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

void TạoThưMụcSRC();
void TạoThưMụcLIB();
void TạoThưMụcInclude();
void TạoThưMụcResources();
void TạoThưMụcBuild();
void TạoFileCMakeLists();
void TạoThưMụcVScode();

int main()
{

    TạoThưMụcSRC();
    TạoThưMụcLIB();
    TạoThưMụcInclude();
    TạoThưMụcResources();
    TạoThưMụcBuild();
    TạoThưMụcVScode();
    TạoFileCMakeLists();
}

void TạoFileCMakeLists()
{
    puts("Tao file CMakeLists.txt");
    std::ofstream fcout{"CMakeLists.txt"};

    fcout << R"(cmake_minimum_required(VERSION 3.11)

# Use the workspace folder name as the project name
get_filename_component(ProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectName ${ProjectName})

project(${ProjectName} CXX)

# Set the path to raylib
set(RAYLIB_PATH C:/raylib/raylib)

# Set the generator explicitly if needed
set(CMAKE_GENERATOR "MinGW Makefiles")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

# Set the include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${RAYLIB_PATH}/src
    ${RAYLIB_PATH}/src/external
)

# Set the library directories
link_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${RAYLIB_PATH}/src
)

# Collect all source files in the src directory
file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Set the executable output directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Set compiler flags based on build mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -g -O0)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -s -O1)
endif()

# Link against raylib and other necessary libraries
target_link_libraries(${PROJECT_NAME}
    raylib
    opengl32
    gdi32
    winmm
)

# Set the working directory for VSCode launch configurations
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Copy resources to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

# Add a "resources" target for VSCode tasks
add_custom_target(resources
    COMMAND ${CMAKE_COMMAND} -E echo "Copy resources..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMENT "Copying resources"
)

# Set the start-up project for VSCode
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

)";
}

void TạoThưMụcVScode()
{
    puts("Tao thu muc .vscode");
    std::filesystem::create_directory(".vscode");

    puts("Tao file c_cpp_properties.json");
    std::ofstream fcout{".vscode/c_cpp_properties.json"};
    fcout << R"({
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "C:/raylib/raylib/src/**",
                "${workspaceFolder}/**",
                "${workspaceFolder}/include",
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE",
                "GRAPHICS_API_OPENGL_33",
                "PLATFORM_DESKTOP"
            ],
            "compilerPath": "C:/raylib/w64devkit/bin/g++.exe",
            "cStandard": "c99",
            "cppStandard": "c++20",
            "intelliSenseMode": "gcc-x64",
            "configurationProvider": "ms-vscode.cpptools"
        }
    ],
    "version": 4
})";

    fcout.close();
    fcout = std::ofstream(".vscode/launch.json");
    fcout << R"({
    "version": "0.2.0",
    "configurations": [
      {
        "name": "Debug",
        "type": "cppdbg",
        "request": "launch",
        "program": "${workspaceFolder}/build/debug/bin/${workspaceFolderBasename}.exe",
        "args": [],
        "stopAtEntry": false,
        "cwd": "${workspaceFolder}",
        "environment": [],
        "externalConsole": false,
        "MIMode": "gdb",
        "setupCommands": [
          {
            "description": "Enable pretty-printing for gdb",
            "text": "-enable-pretty-printing",
            "ignoreFailures": false
          }
        ],
        "windows": {
          "miDebuggerPath": "C:/raylib/w64devkit/bin/gdb.exe"
        },
        "preLaunchTask": "make-debug"
      },
      {
        "name": "Run",
        "type": "cppdbg",
        "request": "launch",
        "program": "${workspaceFolder}/build/release/bin/${workspaceFolderBasename}.exe",
        "args": [],
        "stopAtEntry": false,
        "cwd": "${workspaceFolder}",
        "environment": [],
        "externalConsole": false,
        "MIMode": "gdb",
        "windows": {
          "miDebuggerPath": "C:/raylib/w64devkit/bin/gdb.exe"
        },
        "preLaunchTask": "make-release"
      }
    ]
  })";

    fcout.close();

    fcout = std::ofstream(".vscode/settings.json");
    fcout << R"({
	"files.exclude": {
		"**/.git": true,
		"**/.svn": true,
		"**/.hg": true,
		"**/CVS": true,
		"**/.DS_Store": true,
		"**/*.o": true,
		"**/*.exe": true,
		
	},
	//"cmake.cmakePath": "D:/Cmake/bin/cmake"
	
})";
    fcout.close();

    fcout = std::ofstream(".vscode/tasks.json");
    fcout << R"({
  "version": "2.0.0",
  "tasks": [
    {
      "label": "clean-debug-folder",
      "type": "shell",
      "command": "Remove-Item",
      "args": ["-Recurse", "${workspaceFolder}\\build\\debug"],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "presentation": {
        "reveal": "always",
        "panel": "new"
      }
    },
    {
      "label": "create-debug-folder",
      "type": "shell",
      "command": "mkdir",
      "args": ["${workspaceFolder}\\build\\debug"],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "presentation": {
        "reveal": "always",
        "panel": "new"
      },
      "dependsOn": ["clean-debug-folder"]
    },
    {
      "label": "cmake-debug",
      "type": "shell",
      "command": "D:/Cmake/bin/cmake",
      "args": [
        "-B", "${workspaceFolder}/build/debug",
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DCMAKE_C_COMPILER=C:/raylib/w64devkit/bin/gcc.exe",
        "-DCMAKE_CXX_COMPILER=C:/raylib/w64devkit/bin/g++.exe",
        "-DCMAKE_LINKER=C:/raylib/w64devkit/bin/ld.exe",
        "-G",
        "MinGW Makefiles",
        "${workspaceFolder}"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "options": {
        "cwd": "${workspaceFolder}/build/debug"
      },
      "problemMatcher": [
        "$gcc"
      ],
      "presentation": {
        "reveal": "always",
        "panel": "new"
      },
      "dependsOn": ["create-debug-folder"]
    },
    {
      "label": "make-debug",
      "type": "shell",
      "args": [
        "--debug",
        // ... other arguments
    ],
      "command": "mingw32-make",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "options": {
        "cwd": "${workspaceFolder}/build/debug"
      },
      "problemMatcher": [
        "$gcc"
      ],
      "presentation": {
        "reveal": "always",
        "panel": "new"
      },
      "dependsOn": ["cmake-debug"]
    },
    {
      "label": "clean-release-folder",
      "type": "shell",
      "command": "Remove-Item",
      "args": ["-Recurse", "${workspaceFolder}\\build\\release"],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "presentation": {
        "reveal": "always",
        "panel": "new"
      }
    },
    {
      "label": "create-release-folder",
      "type": "shell",
      "command": "mkdir",
      "args": ["${workspaceFolder}\\build\\release"],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "presentation": {
        "reveal": "always",
        "panel": "new"
      },
      "dependsOn": ["clean-release-folder"]
    },
    {
      "label": "cmake-release",
      "type": "shell",
      "command": "D:/Cmake/bin/cmake",
      "args": [
        "-B", "${workspaceFolder}/build/release",
        "-DCMAKE_BUILD_TYPE=Release",
        "-DCMAKE_C_COMPILER=C:/raylib/w64devkit/bin/gcc.exe",
        "-DCMAKE_CXX_COMPILER=C:/raylib/w64devkit/bin/g++.exe",
        "-DCMAKE_LINKER=C:/raylib/w64devkit/bin/ld.exe",
        "-G",
        "MinGW Makefiles",
        "${workspaceFolder}"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "options": {
        "cwd": "${workspaceFolder}/build/release"
      },
      "problemMatcher": [
        "$gcc"
      ],
      "presentation": {
        "reveal": "always",
        "panel": "new"
      },
      "dependsOn": ["create-release-folder"]
    },
    {
      "label": "make-release",
      "type": "shell",
      "command": "mingw32-make",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "options": {
        "cwd": "${workspaceFolder}/build/release"
      },
      "problemMatcher": [
        "$gcc"
      ],
      "presentation": {
        "reveal": "always",
        "panel": "new"
      },
      "dependsOn": ["cmake-release"]
    }
  ]
}
)";
    fcout.close();
}

void TạoThưMụcSRC()
{
    puts("Tao thu muc src");
    std::filesystem::create_directory("src");
    std::ofstream fcout {"src/main.cpp"};
    fcout<<R"(
int main(){
    
}
    )";
}

void TạoThưMụcLIB()
{
    puts("Tao thu muc lib");
    std::filesystem::create_directory("lib");
}

void TạoThưMụcInclude()
{
    puts("Tao thu muc include");
    std::filesystem::create_directory("include");
}

void TạoThưMụcResources()
{
    puts("Tao thu muc resources");
    std::filesystem::create_directory("resources");
}

void TạoThưMụcBuild()
{
    puts("Tao thu muc build");
    std::filesystem::create_directory("build");

    puts("Tao thu muc build/debug");
    std::filesystem::create_directory("build/debug");

    puts("Tao thu muc build/release");
    std::filesystem::create_directory("build/release");
}
