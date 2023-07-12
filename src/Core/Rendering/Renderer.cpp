#include "./Renderer.hpp"
#include <glad/glad.h>


namespace BL
{
    
    Renderer::Renderer()
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

    void Renderer::Update(Component* page, StyleCollection* stylecollection, float dt)
    {
        page_layout.children.clear();
        page_layout = ComputeComponent(nullptr, page, stylecollection, dt);
        DrawPage();
    }


    void Renderer::UpdateProjection(int fwidth, int fheight)
    {
        m_FWidth = fwidth;
        m_FHeight = fheight;

        glViewport(0, 0, m_FWidth, m_FHeight);

        m_proj = Mat4::orthographic(0.0f, fwidth, 0.0f, fheight, 100.0f, -100.0f);
    }


    ComponentLayout Renderer::ComputeComponent(Component* parent, Component* child, StyleCollection* stylecollection, float dt)
    {
        ComponentLayout component_layout;

        int width;
        int height;

        int left;
        int top;

        Color background_color;
        float opacity;

        for(Component& child_ : child->getChildren())
        {
            component_layout.children.push_back(ComputeComponent(child, &child_, stylecollection, dt));
        }

        
        if(parent == nullptr)
        {
            width = m_FWidth;
            height = m_FHeight;

            left = 0;
            top = 0;

            component_layout.background_color = Color::rgb(250, 198, 14);

            component_layout.opacity = 1;

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


            Style default_style;
            component_styles.push_back(default_style);
            for (int i = 0; i < component_names.size(); i++)
            {
                component_styles.push_back(stylecollection->getStyle(component_names[i]));
            }

            for(Style &style: component_styles)
            {
                width = style.width;
                height = style.height;

                left = style.left;
                top = style.top;

                component_layout.background_color = style.background_color;

                component_layout.opacity = style.opacity;
            }
        }  

        Vec3 s(width, height, 1);
        Vec3 t(left + (width / 2.0f), top + (height / 2.0f), 0);
        Vec3 r(0, 0, 0);

        Mat4 model = Mat4::transform(r, s, t);
        component_layout.mvp = m_proj.multiply(model);

        
        return component_layout;
    }

    void Renderer::DrawPage()
    {

        m_shader.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawElement(page_layout);
    }

    void Renderer::DrawElement(ComponentLayout& component_layout)
    {
        m_shader.setUniformVec4f("color", component_layout.background_color.getFormattedRGB().x, component_layout.background_color.getFormattedRGB().y, component_layout.background_color.getFormattedRGB().z, component_layout.opacity);
        m_shader.setUniformMat4("mvp", component_layout.mvp);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        for(ComponentLayout& child_layout : component_layout.children)
        {
            DrawElement(child_layout);
        }

    }
}