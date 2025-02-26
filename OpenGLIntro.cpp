
// OpenGLIntro.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <GL/glew.h>    
#include <GLFW/glfw3.h> 

//==========(Optional) to rotate
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//==============================

/*Defining vertex shaders sources.
Line by line description of vertex source:
Creating GLSL program stored as string lateral.
Indicate OpenGL version (3.30)
3 coord vector aPos bound to location 0
const 4*4 matrice variable delcared, named transform
the main loop calculates the following:
    gl_position = final vertex position; transform * vec4(aPos, 1.0) = applies transformation using transform matrix to aPos;
    final vertex position = applied transformation, using transform matrix, on aPos*/
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 transform;
    void main() {
        gl_Position = transform * vec4(aPos, 1.0);
    }
)glsl";

/*Defining fragment shaders sources.
Line by line description of vertex source:
Creating GLSL program stored as string lateral.
Indicate OpenGL version (3.30)
Define 3 coord vector as color
The main loop enforces the color vector (later read in RGB)*/
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec3 color;
    void main(){
      color = vec3(1.0,0.0,0.0);
    }
)glsl";

int main(){
    //======================WINDOW======================
    //Initializing GLFW
    if (!glfwInit()){
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    //GLFW Initialized. Adjusting context window.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Not old openGL

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

    //Set up capturing sticky keys
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //======================SHADERS======================
    //Create vertex shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Compile vertex shaders
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr); //Set to source
    glCompileShader(vertexShader); //Compile

    //Create fragment shaders
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Compile fragment shaders
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr); //Set to source
    glCompileShader(fragmentShader); //Compile

    //Create shader program
    unsigned int shaderProgram = glCreateProgram();

    //Add shaders to program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //Finalize and link to openGL
    glLinkProgram(shaderProgram);

    //Delete unused shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //======================SHAPE======================
    /*Pyramid vertices
    Translating the triangle into a pyramid is doable by shifting the base points of the 2D plane along the z-axis equaly on both sides:
    (-0.5,-0.5,0.5), (0.5,-0.5,0.5) & (-0.5,-0.5,-0.5), (0.5,-0.5,-0.5) with apex (0,0.5,0)*/
    float verticesPyramid[] = {
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.0f, 0.5f, 0.0f
    };

    //Defining the indices of the pyramid where each num represents the index of a vertice in verticesPyramid
    unsigned int indices[] = {
        0, 1, 2, //Base 1
        1, 3, 2, //Base 2
        0, 1, 4,
        1, 3, 4,
        3, 2, 4,
        2, 0, 4
    };

    //Declare Vertex Buffer Object, Vertex Array Object & Element Buffer Object
    unsigned int VBO[2], VAO[2], EBO;

    //Genereate Vertex array and bind array to openGL
    glGenVertexArrays(2, VAO); 
    glBindVertexArray(VAO[0]);

    //Generate buffers
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    //Bind VBO buffer to openGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //Fill buffer with pyramid vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPyramid), verticesPyramid, GL_STATIC_DRAW);

    //Bind EBO buffer to openGL
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Fill buffer with pyramid indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Explain how to interpret vertices data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Create identity matrix as baseline for shader
    glUseProgram(shaderProgram);
    int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    float identity[] = {
         1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    };
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, identity);

    //======================MAIN LOOP======================
    do {
        //Clear screen and set color
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Run shaders program
        glUseProgram(shaderProgram);

        //(OPTIONAL) ROTATION TO TEST========================
        float timeValue = glfwGetTime();
        glm::mat4 trans = glm::rotate(glm::mat4(1.0f), timeValue, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        //==================================================

        //Bind Vertex Array
        glBindVertexArray(VAO[0]);

        //Draw element bases on elements array and object array, 18 vertices
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } //Check if exit key is pressed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    //======================EXIT======================
    //Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}

    