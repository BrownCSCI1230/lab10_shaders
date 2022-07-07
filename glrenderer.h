#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_1>
#include "glm.hpp"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_1
{
public:
    GLRenderer(QWidget *parent = nullptr);
    ~GLRenderer();

protected:
    void initializeGL() override;   //Called once at the start of the program
    void paintGL() override;    //Called every frame in a loop
    void resizeGL(int width, int height) override;  //Called when window size changes

private:
    GLuint m_shader;    //Stores id for shader program
    GLuint m_vbo;   //Stores id for vbo
    GLuint m_vao;   //Stores id for vao
};

#endif // GLRENDERER_H
