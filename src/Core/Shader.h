#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

class Shader
{
private:
    unsigned int id_;
    unsigned int createShader(GLenum type, const std::string& sourcePath);
    void createProgram(const std::vector<unsigned int>& shaders);

    using variableToLocation = std::unordered_map<std::string, int>;
    variableToLocation uniformMap_;
public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& tesellationCtrlShaderPath, const std::string& tesellationEvalShaderPath);
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();

    void bind() const;
    void unbind() const;
    void bindTexture() const;
    GLint getUniformLocation(const std::string& name);
    void setMatrix4f(const std::string& name, const glm::mat4& val);
    void setVec3f(const std::string& name, const glm::vec3& val);
    void setFloat1f(const std::string& name, const float& val);
    void setInt(const std::string& name, const int& val);
};

#endif //SHADER_H
