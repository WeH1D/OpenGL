#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    // Create a vertexx buffer object (how many buffers, pointer to int where to save the id/name of the buffer)
    glGenBuffers(1, &buffer);
    // We need to make the buffer an active object in order to store data to it
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Now we can copy data to the buffer, it needs a size in bytes, data and usage pattern
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    
    // In order for our gpu to understafnd the structure of our vertex data, we need to describe them
    glEnableVertexAttribArray(0);
    // Reference: https://docs.gl/gl4/glVertexAttribPointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


    //**************************************************************************************//
    // The shader source code obviously will be read from a file and passed to the shader
    // this approach highlights how the shader itself works

    // Vertex shader runs for every vertex defined and outputs its final coordinates and to pass any data
    // the fragment shader might need.
    std::string vertexShader =
        "#version 330 core \n"
        "\n"
        "layout(location = 0) in vec4 position;\n" // in -> input, vertex shader recieves an attribute we defined on the vertex
        "\n"
        "void main(){\n"
        "   gl_Position = position;\n" // vertex shader assignes a gl_Position to every vertex
        "}\n";

    // The output from the vertex shader is interpolated, and for every pixel thats covered with a primitive
    // Fragment shader runs its source code and decides that pixels colour
    std::string fragmentShader =
        "#version 330 core \n"
        "\n"
        "layout(location = 0) out vec4 color;\n" // out -> mandatory output of the fragment shader, a final color of the pixel
        "\n"
        "void main(){\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    //**************************************************************************************//


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}