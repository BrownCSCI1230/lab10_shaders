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

    void mousePressEvent(QMouseEvent *e) override; // used for camera movement
    void mouseMoveEvent(QMouseEvent *e) override; // used for camera movement
    void wheelEvent(QWheelEvent *e) override;    // used for camera movement
    void rebuildMatrices();                     // used for camera movement

private:
    GLuint m_shader;    //Stores id for shader program
    GLuint m_sphere_vbo;   //Stores id for vbo
    GLuint m_sphere_vao;   //Stores id for vao
    std::vector<float> m_sphereData; //cpu sphere data

    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_proj;


    QPoint m_prevMousePos;
    float m_angleX;
    float m_angleY;
    float m_zoom;
};

#endif // GLRENDERER_H
