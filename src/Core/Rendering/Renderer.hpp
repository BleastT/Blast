#pragma once

#include <iostream>

#include "./Shader.hpp"
#include "../../Math/Mat4.hpp"
#include "../../Math/Vec3.hpp"
#include "../../Components/Component.hpp"

namespace BL
{
    class Renderer
    {
        // Variables
        public:
        private:
            int m_FWidth; // F stands for frame buffer
            int m_FHeight;

            Mat4 m_proj;
            Shader m_shader;

            unsigned int m_VBO;
            unsigned int m_VAO;
            unsigned int m_EBO;

            int m_rotation;

        // Functions
        public:
            Renderer();
            ~Renderer();

            void UpdateProjection(int fwidth, int fheight);

            void Update(Component* page);

        private:

            void ComputeComponent(Component* child, Component* parent);
            void Draw();
    };
}