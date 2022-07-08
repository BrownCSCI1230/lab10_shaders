#include "glrenderer.h"

#include <QCoreApplication>
#include "CS1230Lib/resourceloader.h"
#include "glm.hpp"
#include "glm/gtc/constants.hpp"

GLRenderer::GLRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{

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

            std::cout << iTheta << iPhi <<std::endl;

            pushVec3(p1,&data);
            pushVec3(p2,&data);
            pushVec3(p3,&data);

            pushVec3(p1,&data);
            pushVec3(p3,&data);
            pushVec3(p4,&data);
        }
    }

    std::cout << data.size() <<std::endl;

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
    glClearColor(0,0,0.1,1);

    m_shader = ResourceLoader::createShaderProgram("Resources/Shaders/default.vert", "Resources/Shaders/default.frag"); //Shader setup (DO NOT EDIT)

    glGenBuffers(1, &m_sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphere_vbo);

    m_sphereData = generateSphereData(5,3);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shader);
    glBindVertexArray(m_sphere_vao);



    glDrawArrays(GL_TRIANGLES, 0, m_sphereData.size() / 3);
    glBindVertexArray(0);
    glUseProgram(0);
}

void GLRenderer::resizeGL(int w, int h)
{

}
