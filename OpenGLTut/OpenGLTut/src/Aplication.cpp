#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    ShaderType shaderType = ShaderType::NONE;
    std::stringstream stringStream[2];

    // Loop through each line of the file
    while (getline(stream, line)) {
        // Every line in the file gets stored to either the vertex or fragment string
        // which will be used as the source strings in our shaders
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                shaderType = ShaderType::VERTEX;
            if (line.find("fragment") != std::string::npos)
                shaderType = ShaderType::FRAGMENT;
        }
        else {
            stringStream[(int)shaderType] << line << "\n";
        }
    }

    return { 
        stringStream[(int)ShaderType::VERTEX].str(), 
        stringStream[(int)ShaderType::FRAGMENT].str() 
    };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    // Creates a shader object
    unsigned int shaderId = glCreateShader(type);

    const char* src = source.c_str();
    // Copies the source code provided to the shader, any source code prevously stored in the shader is removed
    // It doesnt compile the source code, it simply copies it to the shader
    glShaderSource(shaderId, 1, &src, nullptr);
    // Actually compiles the source code strings that have been copied into the shader
    // Any compilation states (success, error) will be stored in the shader objects state
    glCompileShader(shaderId);

    int shaderCompileStatus;
    // Returns a parameter from the shader object (in this case, a compile status),
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompileStatus);
    if (shaderCompileStatus != GL_TRUE) {
        int shaderLogLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &shaderLogLength);


        char* message = (char*)alloca(shaderLogLength * sizeof(char));
        // Returns the information log for a shader object
        glGetShaderInfoLog(shaderId, shaderLogLength, &shaderLogLength, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // Reference: https://open.gl/drawing

    // Creates an empty program object
    unsigned int program = glCreateProgram();

    unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
   
    // Attaches actual shader object to the program
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

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

    // ++++++++++++++++++++ SUMMARY ON BUFFER BINDING ++++++++++++++++++++++++++++++++++++++++++++
    //
    // 1. The vertex buffer (buffer) needs to be bound before calling glVertexAttribPointer 
    //    and enabling the vertex attribute array (glEnableVertexAttribArray).
    // 2. The index buffer (iBuffer) needs to be bound before calling glDrawElements.
    //
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    //=================================== CREATING A BUFFER =============================================================
    unsigned int buffer;
    // Create a vertexx buffer object (how many buffers, pointer to int where to save the id/name of the buffer)
    glGenBuffers(1, &buffer);
    // We need to make the buffer an active object in order to store data to it
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Now we can copy data to the buffer, it needs a size in bytes, data and usage pattern
    glBufferData(GL_ARRAY_BUFFER, 8  * sizeof(float), positions, GL_STATIC_DRAW);
  

    // In order for our gpu to understafnd the structure of our vertex data, we need to describe them
    glEnableVertexAttribArray(0);
    // Reference: https://docs.gl/gl4/glVertexAttribPointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    //==================================== CREATING A INDEX BUFFER ============================================================
   // Binding the index buffer so it can be used 
    unsigned int iBuffer;
    glGenBuffers(1, &iBuffer);
    // A change here when compared to the vertex array buffer is the use of 'GL_ELEMENT_ARRAY_BUFFER'
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW);

    //================================================================================================

    
    // if we use a relative path to a file like this, its relative to the working directory of the project
    // that can be seen under project -> settings -> debugging -> Working Directory
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    //**************************************************************************************//


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // render primitives from array data
        // Reference: https://docs.gl/gl4/glDrawArrays
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        // In order to use index array, we need to use glDrawElements insted of glDrawArrays
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}