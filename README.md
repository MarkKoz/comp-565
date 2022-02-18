# COMP 565 - Assignment 1: OpenGL Basics

Course Title: Advanced Computer Graphics<br/>
Semester: Spring 2022<br/>

## Objective
The objective of this homework is to give you a hands-on approach to OpenGL and the basics of Computer Graphics. You're assigned to create a triangle that will interpolate its vertices between the colors of RGB. You are provided with the initial code framework for drawing a triangle which will give you the following output:

![Triangle initial output](https://user-images.githubusercontent.com/1515135/154543120-548669d9-2e14-4b45-8d01-b67eca588377.png)

Output of your program should look like the following:

https://user-images.githubusercontent.com/1515135/154543945-708f4017-1d4b-468d-ac03-267006eb5bc4.mp4

## Implementation
You will need to read chapters/sections 4, 5 and 6 from the book _Learn OpenGL_ in order to complete the assignment.

Your vertex data and attributes are configured as such:

```c
// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float vertices[] = {
    // positions        // colors
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
};
```

The top vertex starts out Blue, the bottom right starts out Red, and the bottom left starts out Green. You will be interpolating the vertex colors between the R, G, and B spectrum for each vertex.

**HINT:** You will need to update your while loop so that at each time the state of each vertex is updated with the next color value.

## Write-up
You will need to have a write-up for the homework, describing the problem and the objective of the assignment. Within your write-up you will discuss how OpenGL graphics pipeline works, vertex inputs, shader program and related parts. What was the most challenging part or the homework? What did you learn?

## Submission
Report and code.

## Building from Source

### Prerequisites
First, ensure a C compiler and CMake are installed and available on your `PATH`. GCC 11.2 and MSVC from VS 2022 have been tested. Clang will probably work too.

### Obtaining the Source Code
Clone the repository with Git along with submodules.
```shell
git clone --depth 1 git@github.com:MarkKoz/comp-565.git
cd comp-565
git submodule init
git submodule update --depth 1
```

Alternatively, extract an archive of the repository and submodules.

### Building
Once all the source files are present in a directory, `cd` into the directory and use CMake to build.

```shell
mkdir -p cmake-build-release
cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Executing
The output binary is named `output` or `output.exe` on Windows. With GCC, the binary will be in `cmake-build-release/output`. With MSVC, it will be in `cmake-build-release/Release/output.exe`.
