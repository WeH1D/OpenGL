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

    // By specifing which vertices will be used in the creation of the triangles
    // we can avoid having to duplicate the vertices in the 'positions'/vertex array
    unsigned int indexBuffer[] = {
        0, 1, 2, // first triangle
        2, 3, 0 // second triangle
    };

    // Creating and binding a vertex array object to save the vertex buffer, index buffer data ...
/*    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
 */   
    VertexArray va;
    
    // ++++++++++++++++++++ SUMMARY ON BUFFER BINDING ++++++++++++++++++++++++++++++++++++++++++++
    //
    // 1. The vertex buffer (buffer) needs to be bound before calling glVertexAttribPointer 
    //    and enabling the vertex attribute array (glEnableVertexAttribArray).
    // 2. The index buffer (iBuffer) needs to be bound before calling glDrawElements.
    //
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //=================================== CREATING A VERTEX BUFFER =============================================================
    //unsigned int buffer;
    //// Create a vertexx buffer object (how many buffers, pointer to int where to save the id/name of the buffer)
    //glGenBuffers(1, &buffer);
    //// We need to make the buffer an active object in order to store data to it
    //glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //// Now we can copy data to the buffer, it needs a size in bytes, data and usage pattern
    //glBufferData(GL_ARRAY_BUFFER, 8  * sizeof(float), positions, GL_STATIC_DRAW);

    VertexBuffer vb(positions, sizeof(float) * 8);
  
       //==================================== CREATING A INDEX BUFFER ============================================================
   //// Binding the index buffer so it can be used 
   // unsigned int iBuffer;
   // glGenBuffers(1, &iBuffer);
   // // A change here when compared to the vertex array buffer is the use of 'GL_ELEMENT_ARRAY_BUFFER'
   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW);

    IndexBuffer ib(indexBuffer, 6);

    VertexBufferLayout layout;
    // We create a definition of the attributes of our vertex buffer, 
    // in this case our vb only has a position attrib thats represented by 3 floats
    layout.Push<float>(2);
    va.AddLayout(vb, ib, layout);
     // Reference: https://docs.gl/gl4/glVertexAttribPointer
    // https://learnopengl.com/Getting-started/Hello-Triangle#:~:text=The%20function%20glVertexAttribPointer%20has%20quite%20a%20few%20parameters%20so%20let%27s%20carefully%20walk%20through%20them%3A
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    //// In order for our gpu to understafnd the structure of our vertex data, we need to describe them
    //glEnableVertexAttribArray(0);


    //==================================== CREATING SHADERS ============================================================

    
    // if we use a relative path to a file like this, its relative to the working directory of the project
    // that can be seen under project -> settings -> debugging -> Working Directory
   /* ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);*/
    Shader shader("res/shaders/Basic.shader"); 

    //**************************************************************************************//
    // unbind everything now that we defined, bound and stored it in the VAO
    va.Unbind();
    vb.Unbind();
    ib.Unbind();

    //// We have to get the uniform location from the already bound shader in order to assign it a value
    //int colorUniformLocation = glGetUniformLocation(shader, "u_Color");
    //// Setting the uniform value
    //glUniform4f(colorUniformLocation, 0.149f, 0.749f, 0.498f, 1.0);
 

    float r = 0.0f;
    float increment = 0.05f;

    // enable wireframe mode, use GL_FILL for regural mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // render primitives from array data
        // Reference: https://docs.gl/gl4/glDrawArrays
        //glDrawArrays(GL_TRIANGLES, 0, 6);


        //glUseProgram(shader);
        //glUniform4f(colorUniformLocation, r, 0.749f, 0.498f, 1.0);
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.749f, 0.498f, 1.0);

        // because we used the VAO to store the vertex and index buffers now we only need to bind
        // the corresponding VAO and not the VBO/EBO as well
        //glBindVertexArray(vao);
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