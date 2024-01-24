#include "Shader.h"
#include "Utils.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

Shader::Shader(const std::string&  filePath)
    : _filename(filePath), _rendererID(0)
{
    ShaderProgramSource source = ParseShader(filePath);
    _rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(_rendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(_rendererID));
}

void Shader::Unbind() const
{
    GLCall(glDeleteProgram(_rendererID));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    GLCall(int location = glGetUniformLocation(_rendererID, name.c_str()));
    GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1) const
{
    GLCall(int location = glGetUniformLocation(_rendererID, name.c_str()));
    GLCall(glUniform2f(location, v0, v1));
}
 

ShaderProgramSource Shader::ParseShader(const std::string & filepath) {
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    // Creates a shader object
    GLCall(unsigned int shaderId = glCreateShader(type));

    const char* src = source.c_str();
    // Copies the source code provided to the shader, any source code prevously stored in the shader is removed
    // It doesnt compile the source code, it simply copies it to the shader
    GLCall(glShaderSource(shaderId, 1, &src, nullptr));
    // Actually compiles the source code strings that have been copied into the shader
    // Any compilation states (success, error) will be stored in the shader objects state
    GLCall(glCompileShader(shaderId));

    int shaderCompileStatus;
    // Returns a parameter from the shader object (in this case, a compile status),
    GLCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompileStatus));
    if (shaderCompileStatus != GL_TRUE) {
        int shaderLogLength;
        GLCall(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &shaderLogLength));


        char* message = (char*)alloca(shaderLogLength * sizeof(char));
        // Returns the information log for a shader object
        GLCall(glGetShaderInfoLog(shaderId, shaderLogLength, &shaderLogLength, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(shaderId));
        return 0;
    }

    return shaderId;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // Reference: https://open.gl/drawing

    // Creates an empty program object
    GLCall(unsigned int program = glCreateProgram());

    unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
   
    // Attaches actual shader object to the program
    GLCall(glAttachShader(program, vShader));
    GLCall(glAttachShader(program, fShader));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vShader));
    GLCall(glDeleteShader(fShader));

    return program;
};
