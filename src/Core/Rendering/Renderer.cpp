#include "./Renderer.hpp"
#include <glad/glad.h>


namespace BL
{
    
    Renderer::Renderer()
    : m_rotation(0)
    {

        float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };

        unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };  

        m_shader.initialize();

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_VBO);  

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &m_EBO);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    }
    Renderer::~Renderer()
    {
        
    }

    void Renderer::UpdateProjection(int fwidth, int fheight)
    {
        m_FWidth = fwidth;
        m_FHeight = fheight;

        glViewport(0, 0, m_FWidth, m_FHeight);

        m_proj = Mat4::orthographic(0.0f, fwidth, 0.0f, fheight, 100.0f, -100.0f);
    }


    void Renderer::ComputeComponent(Component* parent, Component* child, StyleCollection* stylecollection, float dt)
    {
        int width;
        int height;

        int left;
        int top;

        Color background_color;

        float opacity;

        if(parent == nullptr)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            width = m_FWidth;
            height = m_FHeight;

            left = 0;
            top = 0;

            background_color = Color::rgb(250, 198, 14);

            opacity = 1;
        }
        else
        {
            std::vector<std::string> component_names;
            std::vector<Style> component_styles;

            std::string full_name = child->getName();

            size_t pos = 0;
            std::string token;
            while ((pos = full_name.find(" ")) != std::string::npos) {
                token = full_name.substr(0, pos);
                component_names.push_back(token);
                full_name.erase(0, pos + 1);
            }
            component_names.push_back(full_name);


            for (int i = 0; i < component_names.size(); i++)
            {
                component_styles.push_back(stylecollection->getStyle(component_names[i]));
            }

            width = component_styles[0].width;
            height = component_styles[0].height;

            left = component_styles[0].left;
            top = component_styles[0].top;

            background_color = component_styles[0].background_color;

            opacity = component_styles[0].opacity;
        }   



        // std::cout << parent->m_name << std::endl;

        // if(m_rotation >= 360)
        // {
        //     m_rotation = 0;
        // }

        // m_rotation += 250 * dt;

        m_shader.bind();

        Vec3 s(width, height, 1);
        Vec3 t(left + (width / 2), top + (height / 2), 0);
        Vec3 r(0, 0, 0);

        Mat4 model = Mat4::transform(r, s, t);
        Mat4 mvp = m_proj.multiply(model);

        m_shader.setUniformVec4f("color", background_color.getFormattedRGB().x, background_color.getFormattedRGB().y, background_color.getFormattedRGB().z, opacity);
        m_shader.setUniformMat4("mvp", mvp);


        Draw();


        for(Component& child_ : child->getChildren())
        {
            ComputeComponent(child, &child_, stylecollection, dt);
        }
    }

    void Renderer::Draw()
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    }
}