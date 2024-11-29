#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>

struct Vertex {
    float x, y, z;
    Vertex() = default;
    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

    glm::vec3 toVec3() const { return glm::vec3(x, y, z); }
};

struct TextureCoord {
    float u, v, w;
};

struct Normal {
    float nx, ny, nz;
};

struct Face {
    unsigned int v1, v2, v3;  // ���� �ε���
    unsigned int t1, t2, t3;  // �ؽ�ó ��ǥ �ε��� (������)
    unsigned int n1, n2, n3;  // ���� ���� �ε���
};

struct Material {
    std::string name;
    glm::vec3 Ka;  // ȯ�汤 ���
    glm::vec3 Kd;  // ���ݻ籤 ���
    glm::vec3 Ks;  // �ݻ籤 ���
    float Ns;      // ��¦�� ����

    std::string map_Ka; // ȯ�汤 �ؽ�ó ���� ���
    std::string map_Kd; // ���ݻ籤 �ؽ�ó ���� ���
    std::string map_Ks; // �ݻ籤 �ؽ�ó ���� ���

    GLuint ambientTextureID = 0;  // ȯ�汤 �ؽ�ó ID
    GLuint diffuseTextureID = 0;  // ���ݻ籤 �ؽ�ó ID
    GLuint specularTextureID = 0; // �ݻ籤 �ؽ�ó ID

    //GLuint textureID;  // �ؽ�ó ID �ʵ� �߰�
    bool hasTexture;  // �ؽ�ó�� �ִ��� ����
};

class Model {
public:
    std::vector<Vertex> vertices;  // ���� �迭
    std::vector<TextureCoord> texCoords;  // �ؽ�ó ��ǥ �迭 (�߰�)
    std::vector<Normal> normals;   // ���� ���� �迭
    std::vector<Face> faces;       // �� �迭

    glm::mat4 matrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix = glm::mat4(1.0f);  // ���� �̵��� ��ġ�� ������ ����
    glm::mat4 rotateMatrix = glm::mat4(1.0f);    // ���� �ʱ� ȸ�� ���

    Material material;

    std::string name;
    std::string type; // �� ���� Ÿ�� ex) box, sphere, cylinder

    GLuint textureID;  // �ؽ�ó ID �ʵ� �߰�

    bool model_status = true;
    bool rigid_status = true;

    GLuint vao;
    GLuint vbos[4];

    virtual void load_obj(std::string name, std::string obj_name, std::string obj_type, glm::mat4 start_matrix = glm::mat4(1.0f)) = 0;
    virtual const void draw(GLint shaderProgramID, bool (*isKeyPressed_s)(const char&)) = 0;
    virtual void initBuffer() = 0;

    virtual ~Model() = default; // ���� �Ҹ���
};