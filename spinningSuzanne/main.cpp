#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "initHelper.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VBO_layout.h"
#include "VAO.h"
#include "EBO.h"

#include "model.h"

int main(){
    // === Set up a GLFW window, and init GLAD ===
    char windowName[] = "3D spinning cube";
    GLFWwindow* window = glInitHelper::setup(windowName); // Setup function exists just to move all the boilerplate crap out of sight
    glEnable(GL_DEPTH_TEST);

    // === Defining the geometry ===
    std::vector<float> vertices = { // This is a cube
     0.5f,  0.5f, 0.5f,  1.0f, 0.0f, 0.0f, // top right
     0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // bottom left
    -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, // top left 

     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // top right
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // bottom left
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // top left 
    };
    std::vector<unsigned int> indices = {  // note that we start from 0!
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        0, 4, 1,
        1, 4, 5,
        2, 6, 3,
        3, 6, 7,
        0, 4, 7,
        0, 7, 3,
        1, 5, 6,
        1, 6, 2
    };

    model cube;
    cube.vertices = vertices;
    cube.m_layout.pushFloat(3); // position
    cube.m_layout.pushFloat(3); // colour
    cube.indices = indices;
    cube.generate();

    model cube2;
    cube2.vertices = vertices;
    cube2.m_layout.pushFloat(3); // position
    cube2.m_layout.pushFloat(3); // colour
    cube2.indices = indices;
    cube2.generate();

    // === Create shader program ===
    shaderClass shader("GLSL/shader.vert.glsl", "GLSL/shader.frag.glsl");

    // === Define transforms ===
    int m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    glm::mat4 model = glm::mat4(1.0f); // Define a model matrix for the square
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
    glm::mat4 view = glm::mat4(1.0f); // Define a view matrix for the scene
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f)); // note that we're translating the scene in the reverse direction of where we want to move
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)m_viewport[2] / (float)m_viewport[3], 0.1f, 100.0f); // Define the perspective projection matrix



    glm::mat4 model2 = glm::mat4(1.0f); // Test purposes
    model2 = glm::rotate(model2, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // === Main loop === //
    while (!glfwWindowShouldClose(window)){
        // === Unbind/reset stuff (best practice or something) ===
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // === Render ===
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Set the background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer
        
        shader.use(); // Use the shader program
        // Do transform stuff for shader:
        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.25f, 0.5f, 0.75f)); // Rotate the square
        glGetIntegerv(GL_VIEWPORT, m_viewport); // Get the viewport size (maybe code to only do upon resize?)
        perspective = glm::perspective(glm::radians(45.0f), (float)m_viewport[2] / (float)m_viewport[3], 0.1f, 100.0f); // Update the perspective projection matrix
        shader.setUniformMat4fv("model", glm::value_ptr(model)); // Set the model matrix
        shader.setUniformMat4fv("view", glm::value_ptr(view)); // Set the view matrix
        shader.setUniformMat4fv("projection", glm::value_ptr(perspective)); // Set the projection matrix

        // Actualy draw some triangles
        cube.draw();

        shader.use(); // Use the shader program
        shader.setUniformMat4fv("model", glm::value_ptr(model2)); // Set the model matrix
        cube2.draw();

        // === Swap buffers ===
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
