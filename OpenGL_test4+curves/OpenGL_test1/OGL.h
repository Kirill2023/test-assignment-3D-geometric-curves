#pragma once

#include<vector>
#include<memory>
#include<map>
#include<set>
#include<string>
#include<cstdio>
#include<locale>
#include <initializer_list>


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


// GLFW
#include <GLFW/glfw3.h>


// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"

class VAO;
class TEX;

using spVAO = std::shared_ptr<VAO>;
using spTEX = std::shared_ptr<TEX>;


struct RGBA {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};


class VAO
{
private:
    mutable GLuint vao ;
    std::vector<GLuint> vbo;
    GLuint size = 0;

public:

    VAO(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& texture);
    explicit VAO(const std::vector<glm::vec3>& vertices);
    explicit VAO(VAO&& r) noexcept;
    VAO& operator = (VAO&& r) noexcept;
    VAO(const VAO&) = delete;
    VAO& operator = (const VAO&) = delete;
    ~VAO()  noexcept(false);

    inline GLuint getSize() const
    {
        return size;
    }

    void Activate();
};


class TEX
{
private:
    std::vector<GLuint> tex;

public:
    static unsigned char* Load_image(const std::string filename, int* width, int* height, unsigned char Alpha = 255)
    {
        // ������, ����������� �� ��������� BMP-�����
        unsigned char header[54]; // ������ BMP-���� ���������� � ���������, ������ � 54 �����
        unsigned int dataPos;     // �������� ������ � ����� (������� ������)
        //unsigned int width, height;
        unsigned int imageSize;   // ������ ����������� = ������ * ������ * 3
        unsigned int imageSize2;
        // RGB-������, ���������� �� �����


        FILE* file;
        fopen_s(&file, filename.c_str(), "rb");
        if (!file) {
            printf("����������� �� ����� ���� �������n");
            return 0;
        }

        if (fread(header, 1, 54, file) != 54) { // ���� �� ��������� ������ 54 ����, ������ �������� ��������
            printf("������������ BMP-����n");
            return 0;
        }

        if (header[0] != 'B' || header[1] != 'M') {
            printf("������������ BMP-����n");
            return 0;
        }

        // ������ ����������� ������
        dataPos = *(int*)&(header[0x0A]); // �������� ������ ����������� � �����
        imageSize = *(int*)&(header[0x22]); // ������ ����������� � ������
        *width = *(int*)&(header[0x12]); // ������
        *height = *(int*)&(header[0x16]); // ������

        // ��������� BMP-����� ����� ������� ���� imageSize � dataPos, ������� �������� ��
        if (imageSize == 0)    imageSize = *width * *height * 3; // ������ * ������ * 3, ��� 3 - 3 ���������� ����� (RGB)
        if (dataPos == 0)      dataPos = 54; // � ����� ������, ������ ����� ��������� ����� �� ����������
        imageSize2 = imageSize + *width * *height;
        // ������� �����
        unsigned char* data = new unsigned char[imageSize];
        unsigned char* data2 = new unsigned char[imageSize2];

        // ��������� ������ �� ����� � �����
        fread(data, 1, imageSize, file);

        for (uint32_t j = 0, i = 0; i < imageSize; j += 4, i += 3)
        {
            data2[j + 0] = data[i + 2];
            data2[j + 1] = data[i + 1];
            data2[j + 2] = data[i + 0];
            data2[j + 3] = Alpha;
        }
        delete[] data;
        // ��������� ����, ��� ��� ������ �� ��� �� �����
        fclose(file);

        return data2;
    }


    static void Free_image_data(unsigned char* load_image)
    {
        delete[] load_image;
    }


    TEX(std::string name)
    {
        // --== TEXTURE 1 == --
        tex.push_back(0);
        glGenTextures(1, &tex[0]);


        glBindTexture(GL_TEXTURE_2D, tex[0]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Load, create texture and generate mipmaps
        int width, height;
        unsigned char* image = Load_image(name, &width, &height, 255);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        Free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    }


    ~TEX()
    {
        glDeleteTextures(tex.size(), &tex.front());
    }


    void Activate()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex[0]);
    }
};








class OGL
{
    static GLint color;
    static GLint modelLoc;
    static GLint viewLoc;
    static GLint projLoc;
    static Shader ourShader;

public:
    static void Init(GLint screenWidth, GLint screenHeight);
    static void Draw(const spVAO spvao, const spTEX sptex, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
    static void Draw(VAO& vao, glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, uint32_t type, glm::vec4 color);

};

