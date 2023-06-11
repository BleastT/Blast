#include "./Renderer.hpp"
#include <GL/glew.h>


namespace BL
{
    
    Renderer::Renderer()
    : m_rotation(0)
    {

        float vertices[] = {
            0.5f,  0.5f, 1.0f,  // top right
            0.5f, -0.5f, 1.0f,  // bottom right
            -0.5f, -0.5f, 1.0f,  // bottom left
            -0.5f,  0.5f, 1.0f   // top left 
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
        m_proj = Mat4::orthographic(0.0f, fwidth, 0.0f, fheight, 100.0f, -100.0f);
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
        if(m_rotation >= 360)
        {
            m_rotation = 0;
        }

        m_rotation += 1;

        m_shader.bind();

        Vec3 s(100, 100, 1);
        Vec3 t(m_FWidth / 2.0f, m_FHeight / 2.0f, 0);
        Vec3 r(0, 0, m_rotation);

        Mat4 model = Mat4::transform(r, s, t);
        Mat4 mvp = m_proj.multiply(model);

        m_shader.setUniformVec4f("color", 0.2f, 0.3f, 0.5f, 1.0f);
        m_shader.setUniformMat4("mvp", mvp);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        // Clear the color buffer
        glClearColor(0.3f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    }
}