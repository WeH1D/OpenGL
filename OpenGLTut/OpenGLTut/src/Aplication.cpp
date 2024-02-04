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
    const int WINDOW_HEIGHT = 1080;

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


    const int NUM_OF_POSITIONS = 180;
    // pos.x, pos.y, pos.z, texture.x, texture.y
    float positions[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    VertexBuffer vb(positions, sizeof(float) * NUM_OF_POSITIONS);
    VertexBufferLayout layout;
    // We create a definition of the attributes of our vertex buffer, 
    // in this case our vb only has a position attrib thats represented by 3 floats
    layout.Push<float>(3);
    layout.Push<float>(2);

    //IndexBuffer ib(indexBuffer, NUM_OF_INDICES);

    VertexArray va;
    va.AddLayout(vb, layout, nullptr);


    Shader shader("res/shaders/Basic.shader");

    Texture texture("./res/textures/brick_texture.jpeg", 1024, 1024, 3);
    //Texture texture("./res/textures/cube.jpg", 813, 610, 3);

    va.Unbind();
    vb.Unbind();
    //ib.Unbind();


    float modelPositionValues[] = {0.0, 0.0, 0.0};
    float modelRotationValues[] = {0.0, 0.0, 0.0};
    float cameraPositionValues[] = { 0.0f, 0.0f, 0.0f };
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    float cameraFOV = 45.0f;


    // enable wireframe mode, use GL_FILL for regural mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    Renderer renderer;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        glEnable(GL_DEPTH_TEST);  
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        glm::mat4 modelMatrix(1.0);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(modelPositionValues[0], modelPositionValues[1], modelPositionValues[2]));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(modelRotationValues[0]), glm::vec3(1.0, 0.0, 0.0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(modelRotationValues[1]), glm::vec3(0.0, 1.0, 0.0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(modelRotationValues[2]), glm::vec3(0.0, 0.0, 1.0));
        
        glm::mat4 viewMatrix;
        glm::vec3 cameraPos(cameraPositionValues[0], cameraPositionValues[1], cameraPositionValues[2]);
        glm::vec3 cameraDirection(cameraPos[0], cameraPos[1], cameraPos[2] - 5);
        viewMatrix = glm::lookAt(cameraPos, cameraDirection, worldUp);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(cameraFOV), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

        shader.Bind();

        shader.SetUniform4f("u_Color", 0.0f, 0.749f, 0.498f, 1.0);
        shader.SetUniformMatrix4fv("u_projection", false, glm::value_ptr(projectionMatrix));
        shader.SetUniformMatrix4fv("u_view", false, glm::value_ptr(viewMatrix));
        shader.SetUniformMatrix4fv("u_model", false, glm::value_ptr(modelMatrix));

        texture.Bind();

        ImGui::Begin("Hello, world!");                          

        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        ImGui::Text("Model Position");
        ImGui::SliderFloat3("Model Position", modelPositionValues, -10.0, 10.0);
        ImGui::Text("Model Rotation");
        ImGui::SliderFloat3("Model Rotation", modelRotationValues, 0.0, 360.0);
        ImGui::Text("Camera Position");
        ImGui::SliderFloat3("Camera Position", cameraPositionValues, -10.0, 10.0);
        ImGui::Text("Camera FOV");
        ImGui::SliderFloat("Camera FOV", &cameraFOV, 0.0, 180.0);
           
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        renderer.Draw(DrawMode::ARRAYS, va, 180, shader);

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