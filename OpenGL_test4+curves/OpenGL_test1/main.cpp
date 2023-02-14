
// Std. Includes
#include <string>
#include <vector>
#include <list>
#include<Windows.h>


#include <iostream>
#include <algorithm>
#include <cmath>

#include <cstdint>
#include <typeinfo>
#include <memory>
#include <thread>
#include <mutex>
#include <corecrt_math_defines.h>
#include "Lib/shapeapi.h"


// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Other Libs
#include "Camera.h"
#include "OGL.h"
//#include "Other.h"
#include <ppl.h>


// Properties
GLuint screenWidth = 1600, screenHeight = 900;

const int Num =10;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(3.0f, 3.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 600, lastY = 400;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;





using namespace std;


void Radius(vector< shared_ptr<crvs::Shape>>& vShape, int begin, int end, double* sum); // Высчитывает радиус в заданном диапазоне
vector<uint32_t> Span(uint32_t size, uint32_t div); // Разбивает диапазон на поддиапазоны
int Task_execute(vector< shared_ptr<crvs::Shape>>& vShape, uint32_t div, double* sum); // Создает пул потоков и запускает выполнение


mutex m;



double R(int min, int max)
{
    return ((rand() % ((max - min)*100)) + min*100)/100.0;
}




// The MAIN function, from here we start our application and run our Game loop
int main()
{
    setlocale(LC_ALL, "Russian");

    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "curves", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);


    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);




    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


    vector< shared_ptr<crvs::Shape>> vShape; // Для всех типов кривых
    vector< shared_ptr<crvs::Shape>> vShape2; // Только для окружностей

    const uint32_t quantity = 100;// 10000000; // Количество элементов в векторе
    const uint32_t max = 100; // Максимальное значение параметров
    const uint32_t div = 4; // Количество потоков
    const double t = M_PI / 4; // Параметр


    // 2. Заполняем вектор фигурами выбранными случайным образом со случайными параметрами, кроме параметра t
    for (uint32_t i = 0; i < quantity; ++i)
    {
        switch (rand() % 3)
        {
        case 0: vShape.push_back(make_shared < crvs::Circle>(R(1,10))); break;
        case 1: vShape.push_back(make_shared < crvs::Ellipse>(R(1, 10), R(1, 10))); break;
        case 2: vShape.push_back(make_shared < crvs::Spiral>(R(1, 10), R(1, 10))); break;
        }
    }


    // 3. Выводим все что сгенерили
    cout << endl << "++++++++++++++++++++++++  All  ++++++++++++++++++++++++ " << endl << endl;
    for (const auto& x : vShape)
        x->print(t);

    // 4. Отбираем окружности
    for_each(vShape.cbegin(), vShape.cend(),
        //[&vShape2](const auto& c) {if (c.get()->type == crvs::Type_ID::Circle) vShape2.push_back(dynamic_pointer_cast<crvs::Circle>(c)); });
        [&vShape2](const auto& c) {if (c.get()->type == crvs::Type_ID::Circle) vShape2.push_back(c); });

    // 5. Сортируем новый вектор с окружностями
    sort(vShape2.begin(), vShape2.end(),
        [](const auto& a, const auto& b) {return a->usCircle().get_radius() < b->usCircle().get_radius(); });


    // Выводим все окружности
    cout << endl << "++++++++++++++++++++++++ Circle ++++++++++++++++++++++++ " << endl << endl;

    for (const auto& x : vShape2)
        x->print(t);



    //cout << endl << vShape[0]->usCircle().get_radius();

    // 6, 8. Вычисляем сумму с помощью нескольких потоков
    double sum = 0.0;

    Task_execute(vShape2, div, &sum);

    cout << endl << "sum = " << sum << endl;
    //cin.get();

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    OGL::Init(screenWidth, screenHeight);


    std::vector<glm::vec3> vertices_Axis;
    vertices_Axis.push_back(glm::vec3(-12.0f, 0.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(12.0f, 0.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(0.0f, -12.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(0.0f, 12.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(0.0f, 0.0f, -12.0f));
    vertices_Axis.push_back(glm::vec3(0.0f, 0.0f, 12.0f));

    vertices_Axis.push_back(glm::vec3(12.0f, 0.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(11.0f, 0.50f, 0.0f));
    vertices_Axis.push_back(glm::vec3(12.0f, 0.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(11.0f, -0.5f, 0.0f));

    vertices_Axis.push_back(glm::vec3(0.0f, 12.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(0.50f, 11.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(0.0f, 12.0f, 0.0f));
    vertices_Axis.push_back(glm::vec3(-0.5f, 11.0f, 0.0f));

    vertices_Axis.push_back(glm::vec3(0.0f, 0.0f, 12.0f));
    vertices_Axis.push_back(glm::vec3(0.50f, 0.0f, 11.0f));
    vertices_Axis.push_back(glm::vec3(0.0f, 0.0f, 12.0f));
    vertices_Axis.push_back(glm::vec3(-0.5f, 0.0f, 11.0f));

    VAO vao_Axis(vertices_Axis);


    int size = 1000;
    std::vector<glm::vec3> vertices_temp(size);
    std::vector<glm::vec3> vertices_V_temp(2);
    std::vector<glm::vec3> vertices_p_temp(2);
    crvs::Point p_temp;
    crvs::Point p_V_temp;

    vector< shared_ptr<VAO>> vao_S;
    vector< shared_ptr<VAO>> vao_V;
    vector< shared_ptr<VAO>> vao_p;

    double t_step = 6 * M_PI / size;

    for (int x = 0; x < vShape.size(); ++x)
    {
        for (int t0 = 0; t0 < vertices_temp.size(); ++t0)
        {
            p_temp = vShape[x]->get_point(t0 * t_step);
            vertices_temp[t0] = glm::vec3(float(p_temp.x), float(p_temp.y), float(p_temp.z));
        }

        p_V_temp = vShape[x]->get_vector_D1(t).end;
        vertices_V_temp[0] = glm::vec3(0.0f, 0.0f, 0.0f);
        vertices_V_temp[1] = glm::vec3(float(p_V_temp.x), float(p_V_temp.y), float(p_V_temp.z));

        vertices_p_temp[0] = glm::vec3(float(p_V_temp.x), float(p_V_temp.y), float(p_V_temp.z));
        p_V_temp = vShape[x]->get_point(t);
        vertices_p_temp[1] = glm::vec3(float(p_V_temp.x), float(p_V_temp.y), float(p_V_temp.z));


        vao_S.push_back(make_shared < VAO>(vertices_temp));
        vao_V.push_back(make_shared < VAO>(vertices_V_temp));
        vao_p.push_back(make_shared < VAO>(vertices_p_temp));
    }



    glm::mat4 model(1);
    glm::mat4 view(1);
    glm::mat4 projection(1);


    int dt = 0;
    int ii = 0;
    while (!glfwWindowShouldClose(window))
    {

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        if (!(dt % 20))
        {
            if (ii < vShape.size() - 1)
                ++ii;
        }

        int dx, dy;

        for (int u = 0; u <= ii; ++u)
        {
            dx = u % 10 * 25;
            dy = (u / 10) * 25;

            OGL::Draw(vao_Axis, glm::translate(model, glm::vec3(0.0f + dx, 0.0f + dy, 0.0f)), view, projection, GL_LINES, glm::vec4(0.5f, 0.5f, 0.5f, 0.7f));
            OGL::Draw(*vao_p[u], glm::translate(model, glm::vec3(0.0f + dx, 0.0f + dy, 0.0f)), view, projection, GL_POINTS, glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
            OGL::Draw(*vao_V[u], glm::translate(model, glm::vec3(0.0f + dx, 0.0f + dy, 0.0f)), view, projection, GL_LINE_STRIP, glm::vec4(1.0f, 0.3f, 0.7f, 1.0f));
            OGL::Draw(*vao_S[u], glm::translate(model, glm::vec3(0.0f + dx, 0.0f + dy, 0.0f)), view, projection, GL_LINE_STRIP, glm::vec4(0.1f, 0.7f, 0.1f, 1.0f));
        }


        // Swap the buffers
        glfwSwapBuffers(window);
        Sleep(10);
        dt++;
    }


    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if ((keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) && keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(FORWARD, deltaTime * 4);
    if ((keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) && keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(BACKWARD, deltaTime * 4);
    if ((keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) && keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(LEFT, deltaTime * 4);
    if ((keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) && keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(RIGHT, deltaTime * 4);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}





void Radius(vector< shared_ptr<crvs::Shape>>& vShape, int begin, int end, double* sum) // Задача выяисления суммы радиусов
{
    double tmp = 0;
    for_each(vShape.cbegin() + begin, vShape.cbegin() + end,
        [&tmp](const auto& c) {tmp += c->usCircle().get_radius(); });

    m.lock();
    *sum += tmp;
    m.unlock();
}


vector<uint32_t> Span(uint32_t size, uint32_t div) // Разбиваем диапазон на примерно одинаковые поддиапазоны для равномерной нагрузки на потоки вычислений 
{
    int step = size / div;
    vector<uint32_t> tmp;

    for (uint32_t i = 0; i < div; ++i)
    {
        tmp.push_back(i * step);
        tmp.push_back(i * step + step - 1);
    }
    tmp.back() = size - 1;
    return tmp;
}


int Task_execute(vector< shared_ptr<crvs::Shape>>& vShape, uint32_t div, double* sum) // запуск задачи в отдельном потоке
{
    vector< thread> vtr;
    vector<uint32_t> sp = Span(vShape.size(), div);

    for (uint32_t i = 0; i < sp.size(); i += 2)
        //vtr.push_back(thread(Radius, vector< shared_ptr<Circle>>(vShape.begin() + sp[i], vShape.begin() + sp[i + 1] + 1), sum));
        vtr.push_back(thread(Radius, ref(vShape), sp[i], sp[(i + 1)] + 1, sum));

    for (auto& x : vtr)
        x.join();

    return 0;
}