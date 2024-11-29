#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Model.h"
#include "LoadTexture.h"

// MTL ������ �о�ͼ� ���� ������ �Ľ��ϴ� �Լ�
void read_mtl_file(const std::string& filename, Material& material) {
    std::ifstream file("obj/" + filename);  // MTL ���� ����
    if (!file.is_open()) {  // ���� ���� ���� �� ���� ó��
        //throw std::runtime_error("Error opening MTL file: " + filename);
        cout << "mtl file open error" << endl;
        return;
    }

    std::string line;
    bool hasTexture = false;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "Ka") {  // ȯ�汤 ����
            ss >> material.Ka.x >> material.Ka.y >> material.Ka.z;
        }
        else if (prefix == "Kd") {  // ���ݻ籤 ����
            ss >> material.Kd.x >> material.Kd.y >> material.Kd.z;
        }
        else if (prefix == "Ks") {  // �ݻ籤 ����
            ss >> material.Ks.x >> material.Ks.y >> material.Ks.z;
        }
        else if (prefix == "Ns") {  // ��¦�� ����
            ss >> material.Ns;
        }
        else if (prefix == "map_Ka") {  // ȯ�汤 �ؽ�ó ���
            ss >> material.map_Ka;
            material.ambientTextureID = load_texture("obj/" + material.map_Ka);
            if (material.ambientTextureID != 0) {
                std::cout << "Ambient texture loaded: " << material.map_Ka << std::endl;
            }
            else {
                std::cerr << "Failed to load ambient texture: " << material.map_Ka << std::endl;
            }
            hasTexture = true;
        }
        else if (prefix == "map_Kd") {  // ���ݻ籤 �ؽ�ó ���
            ss >> material.map_Kd;
            material.diffuseTextureID = load_texture("obj/" + material.map_Kd);
            if (material.diffuseTextureID != 0) {
                std::cout << "Diffuse texture loaded: " << material.map_Kd << std::endl;
            }
            else {
                std::cerr << "Failed to load diffuse texture: " << material.map_Kd << std::endl;
            }
            hasTexture = true;
        }
        else if (prefix == "map_Ks") {  // �ݻ籤 �ؽ�ó ���
            ss >> material.map_Ks;
            material.specularTextureID = load_texture("obj/" + material.map_Ks);
            if (material.specularTextureID != 0) {
                std::cout << "Specular texture loaded: " << material.map_Ks << std::endl;
            }
            else {
                std::cerr << "Failed to load specular texture: " << material.map_Ks << std::endl;
            }
            hasTexture = true;
        }
    }

    material.hasTexture = hasTexture;  // �ؽ�ó�� �ִ��� ���� �÷��� ����
    file.close();  // ���� �ݱ�
}