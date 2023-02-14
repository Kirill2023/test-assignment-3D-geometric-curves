#include "OGL.h"







VAO::VAO(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& texture) : size(vertices.size())
{
    vbo = std::vector<GLuint>(4, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.front()) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    /*
    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal.front()) * normal.size(), &normal.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribDivisor(1, vertices.size() / normal.size());
    glEnableVertexAttribArray(1);
    */
    glGenBuffers(1, &vbo[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture.front()) * texture.size(), &texture.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    /*
    glGenBuffers(1, &vbo[3]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color.front()) * color.size(), &color.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribDivisor(3, vertices.size() / color.size());
    glEnableVertexAttribArray(3);
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}

VAO::VAO(const std::vector<glm::vec3>& vertices) : size(vertices.size())
{
    vao = 0;
    vbo = std::vector<GLuint>(4, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.front()) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    //std::cout << "Create: " << vao << "\n";
}


VAO::VAO(VAO&& r) noexcept :vao(r.vao), vbo(std::move(r.vbo)), size(r.size)
{
    r.vao = 0;
}

VAO& VAO::operator = (VAO&& r) noexcept
{
    vao = r.vao;
    vbo = std::move(r.vbo);
    size = r.size;
    r.vao = 0;
    return *this;
}

VAO::~VAO()  noexcept(false)
{
    //std::cout << "Delete: " << vao << "\n";
    if (!vao)
    {
        //std::cerr << "Error: vao = " << vao << "\n";
        //throw "Error: vao";
    }
    else
    {
        glDeleteBuffers(vbo.size(), &vbo.front());
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}


void VAO::Activate()
{
    if (vao == 0)
    {
        std::cerr << "Error: vao = " << vao << "\n";
        throw "Error: vao";
    }
    glBindVertexArray(vao);
}








GLint OGL::color;
GLint OGL::modelLoc;
GLint OGL::viewLoc;
GLint OGL::projLoc;
Shader OGL::ourShader;


    void OGL::Init(GLint screenWidth, GLint screenHeight)
    {
        // Initialize GLEW to setup the OpenGL Function pointers
        glewExperimental = GL_TRUE;
        glewInit();

        // Define the viewport dimensions
        glViewport(0, 0, screenWidth, screenHeight);

        // Setup some OpenGL options
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Setup and compile our shaders
        ourShader = { "Path/To/Shaders/transform.vs", "Path/To/Shaders/transform.frag" };

        OGL::color = glGetUniformLocation(ourShader.Program, "_color");
        OGL::modelLoc = glGetUniformLocation(ourShader.Program, "model");
        OGL::viewLoc = glGetUniformLocation(ourShader.Program, "view");
        OGL::projLoc = glGetUniformLocation(ourShader.Program, "projection");
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

        glPointSize((GLfloat)10);
        glLineWidth((GLfloat)3);
    }


    void OGL::Draw(const spVAO spvao, const spTEX sptex, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
    {

        // Clear the colorbuffer
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw our first triangle
        ourShader.Use();

        // Pass the matrices to the shader
        glUniformMatrix4fv(OGL::modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(OGL::viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(OGL::projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        if (sptex != nullptr)
            sptex->Activate();

        spvao->Activate();

        //glDrawArrays(GL_TRIANGLES, NULL, spvao->getSize());
        glDrawArrays(GL_LINE_STRIP, NULL, spvao->getSize());

        glBindVertexArray(0);
    }



    void OGL::Draw(VAO& vao, glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, uint32_t type,  glm::vec4 color)
    {

        // Clear the colorbuffer
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //static glm::vec4 color = glm::vec4{ 1.0f, 0.5f, 0.8f, 1.0f };
        // Draw our first triangle
        ourShader.Use();

        // Pass the matrices to the shader
        glUniform4fv(OGL::color, 1, glm::value_ptr(color));
        glUniformMatrix4fv(OGL::modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(OGL::viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(OGL::projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //glColor3d(0, 1, 0);
       
        

        vao.Activate();



        //glDrawArrays(GL_TRIANGLES, NULL, spvao->size);
        glDrawArrays(type, NULL, vao.getSize());

        glBindVertexArray(0);
    }