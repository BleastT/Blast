#pragma once

#include <iostream>
#include <string>
#include <map>
#include <sstream>

#include "../../Math/Mat4.hpp"

namespace BL
{
    class Shader
    {
            // Variables
        public:
        private:
            unsigned int m_Shader;
            std::map<std::string, int> m_Uniforms;

        // Functions
        public:
            Shader();
            ~Shader();

            void initialize();

            void bind();
            void unbind();

            void setUniformVec2i(std::string name, int v1, int v2);
            void setUniformVec3i(std::string name, int v1, int v2, int v3) ;
            void setUniformVec4i(std::string name, int v1, int v2, int v3, int v4);

            void setUniformVec2f(std::string name, float v1, float v2);
            void setUniformVec3f(std::string name, float v1, float v2, float v3);
            void setUniformVec4f(std::string name, float v1, float v2, float v3, float v4);

            void setUniformMat4(std::string name, Mat4& mat);
        private:

            unsigned int compile(const std::string& source, unsigned int type);
            unsigned int link(unsigned int vertex, unsigned int fragment);

            void findUniforms(const std::string& source);
            
    };
}
