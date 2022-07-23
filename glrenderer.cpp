#include "glrenderer.h"

#include <QCoreApplication>
#include "CS1230Lib/resourceloader.h"
#include "glm.hpp"
#include <QMouseEvent>
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

GLRenderer::GLRenderer(QWidget *parent)
    : QOpenGLWidget(parent), m_ka(0.1), m_kd(0.8), m_ks(1), m_angleX(6), m_angleY(0), m_zoom(2)
{
    rebuildMatrices();
}

GLRenderer::~GLRenderer()
{
    makeCurrent();
    doneCurrent();
}

glm::vec4 sphericalToCartesian(float phi, float theta) {
    return glm::vec4(glm::cos(theta) * glm::sin(phi),
                     glm::sin(theta) * glm::sin(phi),
                     glm::cos(phi),1);
}

void pushVec3(glm::vec4 vec, std::vector<float>* data) {
    data->push_back(vec.x);
    data->push_back(vec.y);
    data->push_back(vec.z);
}

std::vector<float> generateSphereData(int phiTesselations, int thetaTesselations) {
    std::vector<float> data;

    data.clear();
    data.reserve(phiTesselations * thetaTesselations * 6 * 3);

    for(int iTheta = 0; iTheta < thetaTesselations; iTheta++) {
        for(int iPhi = 0; iPhi < phiTesselations; iPhi++) {
            float phi1 = 1.0  * iPhi / phiTesselations * glm::pi<float>();
            float phi2 = 1.0  * (iPhi + 1) / phiTesselations * glm::pi<float>();

            float the1 = 1.0 * iTheta / thetaTesselations * 2 * glm::pi<float>();
            float the2 = 1.0 * (iTheta + 1) / thetaTesselations * 2 * glm::pi<float>();

            glm::vec4 p1 = sphericalToCartesian(phi1,the1);
            glm::vec4 p2 = sphericalToCartesian(phi2,the1);
            glm::vec4 p3 = sphericalToCartesian(phi2,the2);
            glm::vec4 p4 = sphericalToCartesian(phi1,the2);

            pushVec3(p1,&data);
            pushVec3(p2,&data);
            pushVec3(p3,&data);

            pushVec3(p1,&data);
            pushVec3(p3,&data);
            pushVec3(p4,&data);
        }
    }

    return data;
}

void GLRenderer::initializeGL()
{
    // Setting up OpenGL for Qt Creator //
    QSurfaceFormat fmt;
    fmt.setVersion(3, 1);
    QOpenGLContext::currentContext()->setFormat(fmt);
    initializeOpenGLFunctions();

    // Set Clear Color to black
    glClearColor(0,0,0,1);

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);


    //TASK
    m_shader = ResourceLoader::createShaderProgram("Resources/Shaders/default.vert", "Resources/Shaders/default.frag"); //Shader setup (DO NOT EDIT)


    //vao vbo
    glGenBuffers(1, &m_sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphere_vbo);

    m_sphereData = generateSphereData(10,20);

    glBufferData(GL_ARRAY_BUFFER,m_sphereData.size() * sizeof(GLfloat),m_sphereData.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_sphere_vao);
    glBindVertexArray(m_sphere_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GLfloat),reinterpret_cast<void *>(0));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void GLRenderer::paintGL()
{
    // Clear screen color and depth before painting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TASK
    glUseProgram(m_shader);

    glBindVertexArray(m_sphere_vao);


    //TASK X: get uniform location of Model View Projection matricies
    auto modelLoc = glGetUniformLocation(m_shader, "modelMatrix");
    auto viewLoc  = glGetUniformLocation(m_shader, "viewMatrix");
    auto projLoc  = glGetUniformLocation(m_shader, "projMatrix");

    //TASK X: set uniform values of MVP matricies
    glUniformMatrix4fv(modelLoc,1,GL_FALSE,&m_model[0][0]);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&m_view[0][0]);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&m_proj[0][0]);

    //TASK X: get and set light position and color using uniforms
    glUniform4f(glGetUniformLocation(m_shader, "light.position"),10,0,0,1);
    glUniform3f(glGetUniformLocation(m_shader, "light.color"),1,1,1);

    glUniform1f(glGetUniformLocation(m_shader, "ka"),m_ka);
    glUniform1f(glGetUniformLocation(m_shader, "kd"),m_kd);
    glUniform1f(glGetUniformLocation(m_shader, "ks"),m_ks);


    //Draw Command
    glDrawArrays(GL_TRIANGLES, 0, m_sphereData.size() / 3);

    //Unbind Vertex Array
    glBindVertexArray(0);

    //TASK X: un-use shader program
    glUseProgram(0);
}

void GLRenderer::resizeGL(int w, int h)
{
    m_proj = glm::perspective(45.0,1.0 * w / h,0.01,100.0);
}

//---------------------------------------//
// Camera Movement, Don't worry about it //
//---------------------------------------//

void GLRenderer::mousePressEvent(QMouseEvent *event) {
    // Set initial mouse position
    m_prevMousePos = event->pos();
}

void GLRenderer::mouseMoveEvent(QMouseEvent *event) {
    // update angle member variables based on event parameters
    m_angleX += 10 * (event->position().x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 10 * (event->position().y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void GLRenderer::wheelEvent(QWheelEvent *event) {
    // update zoom based on event parameter
    m_zoom -= event->angleDelta().y() / 100.f;
    rebuildMatrices();
}

void GLRenderer::rebuildMatrices() {
    // update view matrix by rotating eye vector based on x and y angles
    m_view = glm::mat4(1);
    glm::mat4 rot = glm::rotate(-10 * m_angleX,glm::vec3(0,0,1));
    glm::vec3 eye = glm::vec3(2,0,0);
    eye = glm::vec3(rot * glm::vec4(eye,1));

    rot = glm::rotate(-10 * m_angleY,glm::cross(glm::vec3(0,0,1),eye));
    eye = glm::vec3(rot * glm::vec4(eye,1));

    eye = eye * m_zoom;

    m_view = glm::lookAt(eye,glm::vec3(0,0,0),glm::vec3(0,0,1));

    m_proj = glm::perspective(45.0,1.0 * width() / height(),0.01,100.0);

    update();
}
