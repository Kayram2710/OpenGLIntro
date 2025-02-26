// OpenGLIntro.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <GL/glew.h>    
#include <GLFW/glfw3.h>  

int main()
{
    //Initializing GLFW
    if (!glfwInit()){
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    //GLFW Initialized. Adjusting context window.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Not old openGL
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Uncomment to make window unresizable

    //Create GLFW window
    GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGLIntro", nullptr, nullptr);

    //Error handling
    if (window == NULL) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    //Initialize GLEW
    glfwMakeContextCurrent(window);

    //Error handling and intiate
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return -1;
    }

    //Set up capturing exit key: "ESC"
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do {
        //Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        //Drawing Area

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } //Check if exit key is pressed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    //Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}

    