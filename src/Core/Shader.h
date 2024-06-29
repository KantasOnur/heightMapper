#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
private:
    unsigned int id_;
    unsigned int createShader(GLenum type, const std::string& sourcePath);
    void createProgram(const std::vector<unsigned int>& shaders);
public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& tesellationCtrlShaderPath, const std::string& tesellationEvalShaderPath);
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();
    void bind() const;
};

#endif //SHADER_H
