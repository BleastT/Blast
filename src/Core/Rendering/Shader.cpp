#include "./Shader.hpp"
#include <glad/glad.h>


namespace BL
{
    Shader::Shader()
    {

    }

    Shader::~Shader()
    {
        unbind();
        glDeleteProgram(m_Shader);
    }


    void Shader::initialize()
    {
        std::string vertexSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            
            uniform mat4 mvp;

            void main()
            {
                gl_Position = mvp * vec4(aPos, 1.0f);
            }
        )";

        std::string fragmentSource = R"(
            #version 330 core

            uniform vec4 color;

            void main()
            {
                gl_FragColor = color;
            } 
        )";

        unsigned int vertexShader = compile(vertexSource, GL_VERTEX_SHADER);
        unsigned int fragmentShader = compile(fragmentSource, GL_FRAGMENT_SHADER);

        m_Shader = link(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);  

        findUniforms(vertexSource);
        findUniforms(fragmentSource);
    }


    void Shader::bind()
    {
        glUseProgram(m_Shader);
    }

    void Shader::unbind()
    {
        glUseProgram(0);
    }


    unsigned int Shader::compile(const std::string& source, unsigned int type)
    {
        const char *c_str = source.c_str();

        unsigned int shader;
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &c_str, NULL);
        glCompileShader(shader);

        int  success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR while compile shader         =>          {\n" << source << "\n}         :" << infoLog << std::endl;
        }

        return shader;

    }
    
    unsigned int Shader::link(unsigned int vertex, unsigned int fragment)
    {
        unsigned int program;
        program = glCreateProgram();

        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        int  success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);        
        if(!success)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "ERROR while linking the shader        =>      :" << infoLog << std::endl;
        }

        return program;
    }



    void Shader::findUniforms(const std::string& source)
    {
        std::stringstream s(source);
        std::string line;

        while (getline(s, line))
        {
            std::string tempName;
            std::string finalName;
            int location;
            if (line.find("uniform") != std::string::npos)
            {
                for (int i = 0; i < line.length(); i++)
                {
                    if (line[i] == ' ')
                        tempName = "";
                    else
                        tempName += line[i];
                }

                for (int i = 0; i < tempName.length(); i++)
                {
                    if (tempName[i] != ';')
                        finalName += tempName[i];
                }

                // find uniform Location
                int location = glGetUniformLocation(m_Shader, finalName.c_str());
                if (location == -1)
                {
                    std::cout << "The uniform '" << finalName << "' is either unused or starts with 'gl_' which is an invalid syntax for a uniform's name." << std::endl;
                    break;
                }

                // update the uniforms vector
                m_Uniforms[finalName] = location;
            }
        }

    }


    void Shader::setUniformVec2i(std::string name, int v1, int v2) {if(!uniformExists(name)) return;  glUniform2i(m_Uniforms[name], v1, v2);}
    void Shader::setUniformVec3i(std::string name, int v1, int v2, int v3) {if(!uniformExists(name)) return;  glUniform3i(m_Uniforms[name], v1, v2, v3);}
    void Shader::setUniformVec4i(std::string name, int v1, int v2, int v3, int v4) { if(!uniformExists(name)) return; glUniform4i(m_Uniforms[name], v1, v2, v3, v4);}

    void Shader::setUniformVec2f(std::string name, float v1, float v2) {if(!uniformExists(name)) return;  glUniform2f(m_Uniforms[name], v1, v2);}
    void Shader::setUniformVec3f(std::string name, float v1, float v2, float v3) {if(!uniformExists(name)) return;  glUniform3f(m_Uniforms[name], v1, v2, v3);}
    void Shader::setUniformVec4f(std::string name, float v1, float v2, float v3, float v4) {if(!uniformExists(name)) return; glUniform4f(m_Uniforms[name], v1, v2, v3, v4);}

    void Shader::setUniformMat4(std::string name, Mat4& mat) {if(!uniformExists(name)) return; glUniformMatrix4fv(m_Uniforms[name], 1, GL_TRUE, mat.mat);}

}