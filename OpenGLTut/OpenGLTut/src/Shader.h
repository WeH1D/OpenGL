#pragma once
#include <iostream>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int _rendererID;
    std::string _filename;
public:
    Shader(const std::string& filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4) const;
	void SetUniform2f(const std::string& name, float v0, float v1) const;
    void SetUniformMatrix4fv(const std::string& name, bool transpose, float* v) const;
private:
    ShaderProgramSource ParseShader(const std::string& filepath);
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
