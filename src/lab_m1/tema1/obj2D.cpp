#include "obj2D.h"

#include <vector>
#include <stb/stb_image_write.h>

#include "core/engine.h"
#include "iostream"
#include "math.h"
#include "utils/gl_utils.h"


Mesh* obj2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* obj2D::CreateHeart(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 bottom = leftBottomCorner;

    std::vector<VertexFormat> vertices = {
        VertexFormat(bottom, color),
        VertexFormat(bottom + glm::vec3(length, length, 0), color),
        VertexFormat(bottom + glm::vec3(length/2, length + (length/2), 0), color),
        VertexFormat(bottom + glm::vec3(0, length, 0), color),
        VertexFormat(bottom + glm::vec3(-length/2, length + (length/2), 0), color),
        VertexFormat(bottom + glm::vec3(-length, length, 0), color)
    };

    Mesh* heart = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 };

    if (!fill) {
        heart->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    heart->InitFromData(vertices, indices);
    return heart;
}

Mesh* obj2D::CreateDiamond(const std::string& name, glm::vec3 centerPos, float length, glm::vec3 color, bool fill)
{
    glm::vec3 center = centerPos;
    glm::vec3 bottom = center - glm::vec3(0, 2 * length, 0);

    std::vector<VertexFormat> vertices = {
        VertexFormat(bottom, color),
        VertexFormat(bottom + glm::vec3(length - 10, 2 * (length - 10), 1), color),
        VertexFormat( bottom + glm::vec3(length + 20, 2 * (length - 10), 1), color),
        VertexFormat( bottom + glm::vec3(length + 20, 2 * (length + 10), 1), color),
        VertexFormat( bottom + glm::vec3(length - 10, 2 * (length + 10), 1), color),
        VertexFormat( bottom + glm::vec3(0, 4 * length, 1), color),
        VertexFormat( bottom + glm::vec3(-length, 2 *length, 1), color),
    };

    // std::vector<VertexFormat> vertices = {
    //     VertexFormat(center, color),
    //     VertexFormat(center + glm::vec3(0, -2 * length, 0), color),
    // };

    Mesh* diamond = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 4, 1, 2, 3 , 1,3,4,0, 4, 5, 0, 5, 6};

    if (!fill) {
        diamond->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    diamond->InitFromData(vertices, indices);
    return diamond;
}

Mesh* obj2D::CreateStar(const std::string& name, glm::vec3 center, float length, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices = {
        VertexFormat(center , color),
        VertexFormat(center + glm::vec3(length, 0, 3), color),};

    for (int i = 1 ; i < 5 ; i++) {
        vertices.push_back(VertexFormat(center + glm::vec3(length * glm::cos(i * 72 * M_PI / 180), length * glm::sin(i * 72 * M_PI / 180), 3), color));
    }
    
    Mesh* star = new Mesh(name);
    std::vector<unsigned> indices = {0,1,3,0,1,4,2,0,4,5,0,3,2,0,5};

    if (!fill) {
        star->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* obj2D::CreateHexagon(const std::string& name, glm::vec3 center, float length, glm::vec3 color_outer , glm::vec3 color_inner, bool fill)
{
    float inner_length = length * 0.65f;
    
    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color_outer),
        VertexFormat(center + glm::vec3(0 , length, 1), color_outer),
        VertexFormat(center + glm::vec3(-(length*glm::sqrt(3))/2 , length/2, 1), color_outer),
        VertexFormat(center + glm::vec3(-(length*glm::sqrt(3))/2 , -length/2, 1), color_outer),
        VertexFormat(center + glm::vec3(0 , -length, 1), color_outer),
        VertexFormat(center + glm::vec3((length*glm::sqrt(3))/2 , -length/2, 1), color_outer),
        VertexFormat(center + glm::vec3((length*glm::sqrt(3))/2 , length/2, 1), color_outer),
        
        VertexFormat(center + glm::vec3(0 , inner_length, 2), color_inner),
        VertexFormat(center + glm::vec3(-(inner_length*glm::sqrt(3))/2 , inner_length/2, 2), color_inner),
        VertexFormat(center + glm::vec3(-(inner_length*glm::sqrt(3))/2 , -inner_length/2, 2), color_inner),
        VertexFormat(center + glm::vec3(0 , -inner_length, 2), color_inner),
        VertexFormat(center + glm::vec3((inner_length*glm::sqrt(3))/2 , -inner_length/2, 2), color_inner),
        VertexFormat(center + glm::vec3((inner_length*glm::sqrt(3))/2 , inner_length/2, 2), color_inner),
    };

    Mesh* hexagon = new Mesh(name);
    std::vector<unsigned int> indices = {1,2,3,1,3,4,1,4,5,1,5,6,7,8,9,7,9,10,7,10,11,7,11,12};

    if (!fill) {
        hexagon->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    hexagon->InitFromData(vertices, indices);
    return hexagon;
}



