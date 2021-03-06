#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "VBO.h"
#include "VBO_layout.h"
#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"

class model{
private:
    VBO m_vbo;
    VAO m_vao;
    EBO m_ebo;
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;

public:
    shaderClass m_shader;
    void readVBO(std::string fileName);
    void readEBO(std::string fileName);
    void setModelT(glm::mat4 model);
    void setViewT(glm::mat4 view);
    void setProjectionT(glm::mat4 projection);
    void setTransformUniforms();
    void draw();
};