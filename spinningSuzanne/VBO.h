#pragma once
#include <glad/glad.h>
#include <vector>

class VBO {
private:
    unsigned int ID;
public:
    ~VBO();
    void generate(std::vector<float> vertices, unsigned int size);
    void bind();
};