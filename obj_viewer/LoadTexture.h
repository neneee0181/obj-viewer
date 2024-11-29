#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <gl/glm/glm/glm.hpp>
#include <gl/glew.h>
#include <gl/stb_image.h>


/// �ؽ�ó �ε� �Լ�: OpenGL �ؽ�ó�� �����ϰ� ���Ͽ��� �����͸� �ε�
/// @param path �ؽ�ó ���� ���
/// @return OpenGL �ؽ�ó ID
GLuint load_texture(const std::string& path) {
    int width, height, nrChannels; // �ؽ�ó �̹����� �ʺ�, ����, ä�� �� ���� ����

    // �ؽ�ó �ε� �� Y���� ������ (OpenGL �ؽ�ó ��ǥ��� �̹��� ��ǥ���� ���̸� ���߱� ����)
    stbi_set_flip_vertically_on_load(true);

    // �̹��� ������ �ε��Ͽ� �ؽ�ó �����͸� ����
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    // �ؽ�ó �ε� ���� �� ó��
    if (!data) {
        std::cerr << "[ERROR] Failed to load texture: " << path << std::endl; // ���� �޽��� ���
        throw std::runtime_error("Failed to load texture");                  // ���� �߻�
    }

    // OpenGL �ؽ�ó ��ü ����
    GLuint textureID;
    glGenTextures(1, &textureID);                  // �ؽ�ó ID ����
    glBindTexture(GL_TEXTURE_2D, textureID);      // ������ �ؽ�ó�� 2D �ؽ�ó�� ���ε�

    // �ؽ�ó ����(Wrapping) �ɼ� ���� (S��� T�� ��� �ݺ�)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // U�� �ݺ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // V�� �ݺ�

    // �ؽ�ó ���͸� �ɼ� ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // ��� �� Mipmap + ���� ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);              // Ȯ�� �� ���� ����

    // �ؽ�ó�� ���� ����: 3ä���̸� RGB, 4ä���̸� RGBA
    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;

    // �ؽ�ó �����͸� OpenGL�� ���ε�
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D); // Mipmap ���� (���� �ػ� ����)

    // CPU �޸𸮿��� �ؽ�ó ������ ���� (GPU�� ���ε尡 �Ϸ�Ǿ����Ƿ� ���ʿ�)
    stbi_image_free(data);

    // �ؽ�ó ID ��ȯ
    return textureID;
}