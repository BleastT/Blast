#pragma once

#include <iostream>

#include "./Shader.hpp"
#include "../../Math/Mat4.hpp"
#include "../../Math/Vec3.hpp"
#include "../../Components/Component.hpp"
#include "../../Styling/StyleCollection.hpp"
#include "../../Styling/Color.hpp"

namespace BL
{
    struct ComponentLayout
    {
        Mat4 mvp;
        Color background_color;
        float opacity;

        std::vector<ComponentLayout> children;
    };

    class Renderer
    {
        // Variables
        public:
            ComponentLayout page_layout;
        private:
            int m_FWidth; // F stands for frame buffer
            int m_FHeight;

            Mat4 m_proj;
            Shader m_shader;

            unsigned int m_VBO;
            unsigned int m_VAO;
            unsigned int m_EBO;


        // Functions
        public:
            Renderer();
            ~Renderer();

            void Update(Component* page, StyleCollection* stylecollection, float dt);
            void UpdateProjection(int fwidth, int fheight);

        private:

            void DrawPage();
            void DrawElement(ComponentLayout& component_layout);
            ComponentLayout ComputeComponent(Component* parent, Component* child, StyleCollection* stylecollection, float dt);
    };
}