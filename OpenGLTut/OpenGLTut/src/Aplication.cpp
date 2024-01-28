#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "Utils.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 1024, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Set the fps to match the refresh rate of the monitor (freesync)
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW INIT ERROR!" << std::endl;
    }

    std::cout << "GL VESRION: " << glGetString(GL_VERSION) << std::endl;
    
    const int NUM_OF_POSITIONS = 40;
    // pos.x, pos.y, pos.z, texture.x, texture.y
    float positions[] = {
       -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // 0 
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f, // 1
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f, // 2
       -0.5f,  0.5f, 0.5f,  0.0f, 1.0f, // 3
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 4
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 5
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 6 
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 7
    };

    const int NUM_OF_INDICES = 36;
    unsigned int indexBuffer[] = {
        0, 1, 2, // front
        2, 3, 0, // front
        1, 4, 5, // right
        5, 2, 1, // right
        4, 6, 7, // back
        7, 5, 4, // back
        6, 0, 3, // left
        3, 7, 6, // left
        3, 2, 5, // top
        5, 7, 3, // top
        6, 4, 1, // bottom
        1, 0, 6  // bottom
    };

    
    VertexBuffer vb(positions, sizeof(float) * NUM_OF_POSITIONS);
    VertexBufferLayout layout;
    // We create a definition of the attributes of our vertex buffer, 
    // in this case our vb only has a position attrib thats represented by 3 floats
    layout.Push<float>(3);
    layout.Push<float>(2);
    
    IndexBuffer ib(indexBuffer, NUM_OF_INDICES);
   
    VertexArray va;
    va.AddLayout(vb, ib, layout);
    
    
    Shader shader("res/shaders/Basic.shader"); 

    Texture texture("./res/textures/brick_texture.jpeg", 1024, 1024, 3);
    //Texture texture("./res/textures/cube.jpg", 813, 610, 3);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();

   

    float r = 0.0f;
    float increment = 0.05f;

    // enable wireframe mode, use GL_FILL for regural mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        glEnable(GL_DEPTH_TEST);  

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.749f, 0.498f, 1.0);
        
        // Creates an identity matrix with a diagonal values of 1.0f
        glm::mat4 transformationMatrix = glm::mat4(1.0f);
        // Scales a provided matrix by a provided value in x, y and z axis
        transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.5, 0.5, 0.5));
        // Rotates a provided matrix by 90 deg on the z axis
        transformationMatrix = glm::rotate(transformationMatrix, (float)glfwGetTime(), glm::vec3(0.0, 1.0, 0.0));
        transformationMatrix = glm::rotate(transformationMatrix, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));

        shader.SetUniformMatrix4fv("u_Transform", false, glm::value_ptr(transformationMatrix));

        texture.Bind();

        renderer.Draw(va, ib.getCount(), shader);

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}