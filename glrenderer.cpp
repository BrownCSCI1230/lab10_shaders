#include "glrenderer.h"

#include <QCoreApplication>
#include "CS1230Lib/resourceloader.h"

GLRenderer::GLRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

GLRenderer::~GLRenderer()
{
    makeCurrent();
    doneCurrent();
}

void GLRenderer::initializeGL()
{
    // Setting up OpenGL for Qt Creator //
    QSurfaceFormat fmt;
    fmt.setVersion(3, 1);
    QOpenGLContext::currentContext()->setFormat(fmt);
    initializeOpenGLFunctions();

    //TASK 2: Set the clear color here

    m_shader = ResourceLoader::createShaderProgram("Resources/Shaders/default.vert", "Resources/Shaders/default.frag"); //Shader setup (DO NOT EDIT)


    // Vertex Buffer Objects //

    //TASK 3: Generate a VBO here and store it in m_vbo

    //TASK 4: Bind the VBO you created here

    //TASK 6: Construct your std::vector of triangle data here | TASK 8: Add colors to your triangle here

    //TASK 7: Pass the triangle vector into your VBO here


    // Vertex Array Objects //

    //TASK 9: Generate a VAO here and store it in m_vao

    //TASK 10: Bind the VAO you created here

    //TASK 11: Add position and color attributes to your VAO here


    // Returning to Default State //

    //TASK 12: Unbind your VBO and VAO here
}

void GLRenderer::paintGL()
{
    //TASK 13: Clear the screen here

    glUseProgram(m_shader);

    //TASK 14: Bind your VAO here

    //TASK 15: Draw your VAO here

    //TASK 16: Unbind your VAO here

    glUseProgram(0);
}

void GLRenderer::resizeGL(int w, int h)
{

}
