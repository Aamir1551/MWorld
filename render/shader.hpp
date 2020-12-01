#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <util_functions.h>

class Shader
{

private:
    void static compile_code(int shaderID, std::string sourcecode, char const *shaderName)
    {

        char const *c = sourcecode.c_str();
        glShaderSource(shaderID, 1, &(c), NULL);
        glCompileShader(shaderID);

        int success;
        char infoLog[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }

    void static link_shaders(unsigned int &shader_id, unsigned int vertex_shader, unsigned int frag_shader)
    {
        shader_id = glCreateProgram();

        int success;
        char infoLog[512];
        glAttachShader(shader_id, vertex_shader);
        glAttachShader(shader_id, frag_shader);
        glLinkProgram(shader_id);

        glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader_id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }
    }

public:
    unsigned int shader_id;

    Shader(std::string vert_file, std::string frag_file)
    {
        std::string vertex_shader_sourcecode;
        std::string frag_shader_sourcecode;

        util_functions::read_from_file(vert_file, vertex_shader_sourcecode);
        util_functions::read_from_file(frag_file, frag_shader_sourcecode);

        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        char const *vp = "vertex";
        compile_code(vertex_shader, vertex_shader_sourcecode, vp);

        unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        char const *fp = "fragment";
        compile_code(frag_shader, frag_shader_sourcecode, fp);

        link_shaders(shader_id, vertex_shader, frag_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(frag_shader);
    };

    void use()
    {
        glUseProgram(shader_id);
    };

    // utility uniform functions
    void set_bool(const std::string &name, bool a) const
    {
        glUniform1i(glGetUniformLocation(shader_id, name.c_str()), (int)a);
    };
    void set_int(const std::string &name, int a) const
    {
        glUniform1i(glGetUniformLocation(shader_id, name.c_str()), a);
    };
    void set_float(const std::string &name, float a) const
    {
        glUniform1f(glGetUniformLocation(shader_id, name.c_str()), a);
    };
    void set_float3(const std::string &name, float a, float b, float c)
    {
        glUniform3f(glGetUniformLocation(shader_id, name.c_str()), a, b, c);
    }

    ~Shader()
    {
        glDeleteShader(shader_id);
    }
};