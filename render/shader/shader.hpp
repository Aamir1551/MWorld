#ifndef MWORLD_RENDER_SHADER_SHADER_H
#define MWORLD_RENDER_SHADER_SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <util_functions.h>

namespace render_utils
{

    class Shader
    {

    private:
        void static CompileCode(int shaderID, const std::string& sourcecode, char const *shaderName)
        {
            // Compiling code
            char const *c = sourcecode.c_str();
            int success;
            char infoLog[512];

            glShaderSource(shaderID, 1, &(c), NULL);
            glCompileShader(shaderID);

            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }

        void static LinkShaders(unsigned int &shader_id, unsigned int vertex_shader, unsigned int frag_shader)
        {
            int success;
            char infoLog[512];

            shader_id = glCreateProgram();

            glAttachShader(shader_id, vertex_shader);
            glAttachShader(shader_id, frag_shader);
            glLinkProgram(shader_id);

            glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader_id, 512, nullptr, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }
        }

    public:
        unsigned int shader_id = 0;

        Shader(std::string vert_file, std::string frag_file)
        {
            std::string vertex_shader_sourcecode;
            std::string frag_shader_sourcecode;

            util_functions::read_from_file(vert_file, vertex_shader_sourcecode);
            util_functions::read_from_file(frag_file, frag_shader_sourcecode);

            unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            char const *vp = "vertex";
            CompileCode(vertex_shader, vertex_shader_sourcecode, vp);

            unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
            char const *fp = "fragment";
            CompileCode(frag_shader, frag_shader_sourcecode, fp);

            LinkShaders(shader_id, vertex_shader, frag_shader);

            glDeleteShader(vertex_shader);
            glDeleteShader(frag_shader);
        };

        void Use()
        {
            glUseProgram(shader_id);
        };

        void SetBool(const std::string &name, bool a) const
        {
            glUniform1i(glGetUniformLocation(shader_id, name.c_str()), (int)a);
        };
        void SetInt(const std::string &name, int a) const
        {
            glUniform1i(glGetUniformLocation(shader_id, name.c_str()), a);
        };
        void SetFloat(const std::string &name, float a) const
        {
            glUniform1f(glGetUniformLocation(shader_id, name.c_str()), a);
        };
        void SetFloat3(const std::string &name, float a, float b, float c)
        {
            glUniform3f(glGetUniformLocation(shader_id, name.c_str()), a, b, c);
        }

        ~Shader()
        {
            glDeleteShader(shader_id);
        }
    };
} // namespace render_utils
#endif