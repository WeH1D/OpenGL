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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

	const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT= 1080;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
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


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
   // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    
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

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  0.0f, 0.0f),
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 2.0f),
    /*    glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)*/
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


    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 viewMatrix= glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -4.0f));



    glm::vec3 cameraPos(2.0f, 2.0f, 3.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);

    glm::vec3 cameraDirection(glm::normalize(cameraTarget - cameraPos ));

    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight(glm::cross(worldUp, cameraDirection));

    glm::vec3 cameraUp(glm::cross(cameraDirection, cameraRight));

    glm::mat4 cameraLookAt(glm::lookAt(cameraPos, cameraDirection, worldUp));
    
    float r = 0.0f;
    float increment = 0.05f;

    // enable wireframe mode, use GL_FILL for regural mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    Renderer renderer;
    
    float radius = 7.0f;
    float scale[] = { 0.5, 0.5, 0.5 };
    
    float camX = 1.0;
    float camZ = 1.0;



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        glEnable(GL_DEPTH_TEST);  
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.749f, 0.498f, 1.0);
        shader.SetUniformMatrix4fv("u_view", false, glm::value_ptr(viewMatrix));
        shader.SetUniformMatrix4fv("u_projection", false, glm::value_ptr(projectionMatrix));

        texture.Bind();

        static float f = 0.0f;
        static int counter = 0;
              
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &radius, 0.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("camX", &camX, 0.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("camZ", &camZ, 0.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat3("scale", scale, 0.0f, 10.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();


        for (int i = 0; i < 4; i++) {
            // Creates an identity matrix with a diagonal values of 1.0f
            glm::mat4 transformationMatrix = glm::mat4(1.0f);
            transformationMatrix = glm::translate(transformationMatrix,cubePositions[i]);
            glm::vec3 scaleTransform = glm::vec3(scale[0], scale[1], scale[2]);
            // Scales a provided matrix by a provided value in x, y and z axis
            transformationMatrix = glm::scale(transformationMatrix, scaleTransform);
            // Rotates a provided matrix by 90 deg on the z axis
           // transformationMatrix = glm::rotate(transformationMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            
            cameraLookAt = glm::lookAt(glm::vec3(float(camX), 2.0f, float(camZ)), cameraDirection, worldUp);

            transformationMatrix =  cameraLookAt * transformationMatrix;

            shader.SetUniformMatrix4fv("u_model", false, glm::value_ptr(transformationMatrix));
            shader.SetUniformMatrix4fv("u_Transform", false, glm::value_ptr(transformationMatrix));

            renderer.Draw(va, ib.getCount(), shader);
        }

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}