#include "obj_loader.hpp"

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

#include "model.hpp"
#include "loader.hpp"

std::vector<std::string> splitLine(std::string line, char* splitChar) {
    std::string lineSegment;
    std::vector<std::string> splitLine;

    for (char c: line) {
        if (c == *splitChar) {
            splitLine.push_back(lineSegment);
            lineSegment = "";
        }
        else {
            lineSegment.append(std::string(1, c));
        }
    }
    splitLine.push_back(lineSegment);

    return splitLine;
}

Model loadObjModel(std::string path, Loader& loader) {
    std::ifstream file(path);
    if (!file.is_open())
        return Model();
    
    std::string line;

    std::list<glm::vec3> vertices;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<int> indices;

    std::vector<float> textureArray;
    std::vector<float> normalArray;

    int counter = 0;

    try {
        while (true) {
            std::getline(file, line);
            std::vector<std::string> currentLine = splitLine(line, (char*)" ");

            //std::cout << counter << ": " << currentLine.size() << "\n";

            if (line.substr(0, 2) == "v ") {
                glm::vec3 vertex(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
                vertices.push_back(vertex);
            }
            else if (line.substr(0, 3) == "vt ") {
                glm::vec2 texture(std::stof(currentLine[1]), std::stof(currentLine[2]));
                textures.push_back(texture);
            }
            else if (line.substr(0, 3) == "vn ") {
                glm::vec3 normal(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
                normals.push_back(normal);
            }
            else if (line.substr(0, 2) == "f ") {
                textureArray = std::vector<float>(vertices.size() * 2);
                normalArray = std::vector<float>(vertices.size() * 3);
                break;
            }

            counter++;
        }

        while (line != "") {
            if (line.substr(0, 2) != "f ") {
                std::getline(file, line);
                continue;
            }

            std::vector<std::string> currentLine = splitLine(line, (char*)" ");

            std::vector<std::string> vertex1 = splitLine(currentLine[1], (char*)"/");
            std::vector<std::string> vertex2 = splitLine(currentLine[2], (char*)"/");
            std::vector<std::string> vertex3 = splitLine(currentLine[3], (char*)"/");

            processVertex(vertex1, indices, textures, normals, textureArray, normalArray);
            processVertex(vertex2, indices, textures, normals, textureArray, normalArray);
            processVertex(vertex3, indices, textures, normals, textureArray, normalArray);

            std::getline(file, line);
        }
        file.close();
    }
    catch (std::exception e) {
        std::cout << e.what() << "\n";
    }

    std::vector<float> verticesArray(vertices.size() * 3);
    std::vector<int> indicesArray(indices.size());

    int vertexPointer = 0;

    for (glm::vec3 vertex: vertices) {
        verticesArray[vertexPointer++] = vertex.x;
        verticesArray[vertexPointer++] = vertex.y;
        verticesArray[vertexPointer++] = vertex.z;
    }

    for (unsigned int i = 0; i < indices.size(); i++) 
        indicesArray[i] = indices[i];

    return loader.generateModel(verticesArray, textureArray, normalArray, indicesArray);
}

void processVertex(std::vector<std::string> vertexData, std::vector<int>& indices, std::vector<glm::vec2> textures, std::vector<glm::vec3> normals,
    std::vector<float>& textureArray, std::vector<float>& normalArray) {

    int currentVertex = std::stoi(vertexData[0]) - 1;
    indices.push_back(currentVertex);

    glm::vec2 currentTex;

    if (vertexData[1] == "")
        currentTex = glm::vec2(0, 0);
    else
        currentTex = textures[std::stoi(vertexData[1]) - 1];

    textureArray[currentVertex * 2] = currentTex.x;
    textureArray[currentVertex * 2 + 1] = currentTex.y;

    glm::vec3 currentNorm = normals[std::stoi(vertexData[2]) - 1];
    normalArray[currentVertex * 3] = currentNorm.x;
    normalArray[currentVertex * 3 + 1] = currentNorm.y;
    normalArray[currentVertex * 3 + 2] = currentNorm.z;
}