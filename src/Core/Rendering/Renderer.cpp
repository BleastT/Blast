#include "./Renderer.hpp"
#include <GL/glew.h>


namespace BL
{
    
    Renderer::Renderer()
    {

        float vertices[] = {
            1.0f,  2.0f, 0.0f,  // top right
            1.0f, -2.0f, 0.0f,  // bottom right
            -1.0f, -2.0f, 0.0f,  // bottom left
            -1.0f,  2.0f, 0.0f   // top left 
        };

        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };  


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


        m_shader.initialize();
        m_shader.bind();
        
        m_shader.setUniformVec3f("color", 0.5f, 0.5f, 0.5f);
    }
    Renderer::~Renderer()
    {

    }

    void Renderer::UpdateProjection(int fwidth, int fheight)
    {
        m_FWidth = fwidth;
        m_FHeight = fheight;
        m_proj = Mat4::orthographic(0.0f, fwidth, 0.0f, fheight, -1000.0f, 1000.0f);
    }

    void Renderer::Update(Component* page)
    {
        Draw();
    }


    void Renderer::ComputeComponent(Component* child, Component* parent)
    {

    }

    void Renderer::Draw()
    {
        Vec3 t(20, 0, 0);
        Vec3 s(100, 100, 100);
        Vec3 r(0, 0, 0);
        Mat4 model = Mat4::transform(r, s, t);
        Mat4 mvp = m_proj.multiply(model);

        m_shader.setUniformMat4("mvp", mvp);
        // mvp.print("MVP");
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}