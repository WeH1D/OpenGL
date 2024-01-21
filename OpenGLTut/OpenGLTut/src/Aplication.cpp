#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"


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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
    
    float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
         -0.5f, 0.5f, // 3
    };

       unsigned int indexBuffer[] = {
        0, 1, 2, // first triangle
        2, 3, 0 // second triangle
    };

    VertexArray va;
    
    VertexBuffer vb(positions, sizeof(float) * 8);
    
    IndexBuffer ib(indexBuffer, 6);

    VertexBufferLayout layout;
    // We create a definition of the attributes of our vertex buffer, 
    // in this case our vb only has a position attrib thats represented by 3 floats
    layout.Push<float>(2);
    va.AddLayout(vb, ib, layout);
    
    
    Shader shader("res/shaders/Basic.shader"); 

    va.Unbind();
    vb.Unbind();
    ib.Unbind();

   

    float r = 0.0f;
    float increment = 0.05f;

    // enable wireframe mode, use GL_FILL for regural mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.749f, 0.498f, 1.0);

        va.Bind();

		// In order to use index array, we need to use glDrawElements insted of glDrawArrays
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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