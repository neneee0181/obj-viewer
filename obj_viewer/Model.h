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

struct Texture {
    std::string type;  // 텍스처 유형 (e.g., diffuse, specular, normal, ambient)
    GLuint id;         // OpenGL 텍스처 ID
    std::string path;  // 텍스처 파일 경로
};

struct TextureCoord {
    float u, v, w;
};

struct Normal {
    float nx, ny, nz;
};

struct Face {
    unsigned int v1, v2, v3;  // 정점 인덱스
    unsigned int t1, t2, t3;  // 텍스처 좌표 인덱스 (선택적)
    unsigned int n1, n2, n3;  // 법선 벡터 인덱스
    std::string materialName; // Face가 사용하는 재질 이름 (usemtl)
};

struct Material {
    std::string name;
    glm::vec3 Ka;  // 환경광 계수
    glm::vec3 Kd;  // 난반사광 계수
    glm::vec3 Ks;  // 반사광 계수
    float Ns;      // 반짝임 강도

    std::string map_Ka; // 환경광 텍스처 파일 경로
    std::string map_Kd; // 난반사광 텍스처 파일 경로
    std::string map_Ks; // 반사광 텍스처 파일 경로

    GLuint ambientTextureID = 0;  // 환경광 텍스처 ID
    GLuint diffuseTextureID = 0;  // 난반사광 텍스처 ID
    GLuint specularTextureID = 0; // 반사광 텍스처 ID

    std::vector<Texture> textures;  // 여러 텍스처 관리

    bool hasTexture_s = false;  // 텍스처가 있는지 여부

    // 텍스처가 있는지 확인
    bool hasTexture() const { return !textures.empty(); }
};

class Model {
public:
    std::vector<Vertex> vertices;  // 정점 배열
    std::vector<TextureCoord> texCoords;  // 텍스처 좌표 배열 (추가)
    std::vector<Normal> normals;   // 법선 벡터 배열
    std::vector<Face> faces;       // 면 배열

    glm::mat4 matrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix = glm::mat4(1.0f);  // 모델이 이동한 위치를 저장할 벡터
    glm::mat4 rotateMatrix = glm::mat4(1.0f);    // 모델의 초기 회전 행렬

    std::unordered_map<std::string, Material> materials;

    std::string name;
    std::string type; // 모델 도형 타입 ex) box, sphere, cylinder

    GLuint textureID;  // 텍스처 ID 필드 추가

    bool model_status = true;
    bool rigid_status = true;

    GLuint vao;
    GLuint vbos[4];
    // 텍스처별로 Face를 그룹화
    std::unordered_map<std::string, std::vector<unsigned int>> textureGroups;
    std::unordered_map<std::string, GLuint> textureEBOs;

    // 재질과 텍스처 매핑
    std::unordered_map<std::string, Texture> materialTextureMap;

    virtual void load_obj(std::string name, std::string obj_name, std::string obj_type, glm::mat4 start_matrix = glm::mat4(1.0f)) = 0;
    virtual const void draw(GLint shaderProgramID, bool (*isKeyPressed_s)(const char&)) = 0;
    virtual void initBuffer() = 0;

    virtual ~Model() = default; // 가상 소멸자
};