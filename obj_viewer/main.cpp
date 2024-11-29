#include <iostream>
#include <vector> 
#include <atomic>

#include "shaderMaker.h"
#include "DefaultModel.h"
#include "KeyBoard.h"
#include "Camera.h"
#include "Light.h"
#include "Debug.h"

using namespace std;

void InitBuffer();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

//모델
vector<Model*> models;

void printProgressBar(int progress, int total) {
    const int barWidth = 50; // 로딩바 너비
    float percentage = static_cast<float>(progress) / total;

    std::cout << "\r[";
    int pos = static_cast<int>(barWidth * percentage);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(percentage * 100.0) << "%";
    std::cout.flush();
}

void loadModelWithProgress(const std::string& modelPath, const std::string& modelName, const std::string& modelType, glm::mat4 scale, std::vector<Model*>& models) {
    std::cout << "Loading model: " << modelName << std::endl;

    const int totalSteps = 4; // 총 작업 단계 수
    int currentStep = 0;

    // Step 1: OBJ 파일 로드
    std::cout << "Step 1/4: Reading OBJ file..." << std::endl;
    DefaultModel* default_model = new DefaultModel(modelPath, modelName, modelType, scale);
    currentStep++;
    printProgressBar(currentStep, totalSteps);

    // Step 2: MTL 파일 처리
    std::cout << "\nStep 2/4: Processing Materials (MTL file)..." << std::endl;
    for (const auto& [materialName, material] : default_model->materials) {
        std::cout << "Processing material: " << materialName << std::endl;
        if (material.hasTexture()) {
            std::cout << "  Material has textures: " << material.textures.size() << std::endl;
        }
        else {
            std::cout << "  No textures found for material: " << materialName << std::endl;
        }
    }
    currentStep++;
    printProgressBar(currentStep, totalSteps);

    // Step 3: 텍스처 로드
    std::cout << "\nStep 3/4: Loading textures..." << std::endl;
    for (const auto& [materialName, material] : default_model->materials) {
        for (const auto& texture : material.textures) {
            std::cout << "Loading texture: " << texture.path << std::endl;
            if (texture.id != 0) {
                std::cout << "  Texture loaded successfully (ID: " << texture.id << ")" << std::endl;
            }
            else {
                std::cerr << "  Failed to load texture: " << texture.path << std::endl;
            }
        }
    }
    currentStep++;
    printProgressBar(currentStep, totalSteps);

    // Step 4: 버퍼 초기화
    std::cout << "\nStep 4/4: Initializing buffers..." << std::endl;
    default_model->initBuffer();
    currentStep++;
    printProgressBar(currentStep, totalSteps);

    // 모델 로드 완료
    std::cout << "\nModel loaded successfully!" << std::endl;
    models.push_back(default_model);
}

int main(int argc, char** argv) {
    // 모델 정보를 사용자로부터 입력받기
    string modelPath;
    string modelName;
    string modelType;
    glm::vec3 modelScale;
    string debugStatus1;
    string debugStatus2;

    cout << "Enter model file path (ex> obj/Skull.obj): ";
    cin >> modelPath;
    cout << "Enter model name (ex> Skull): ";
    cin >> modelName;
    cout << "Enter model type (ex> bone): ";
    cin >> modelType;
    cout << "Enter model scale (ex>, 1.0 1.0 1.0): ";
    cin >> modelScale.x >> modelScale.y >> modelScale.z;
    cout << "Enter Debug model mode (ex> y or n): ";
    cin >> debugStatus1;
    cout << "Enter Debug texture mode (ex> y or n): ";
    cin >> debugStatus2;

    width = 800;
    height = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 200);
    glutInitWindowSize(width, height);
    glutCreateWindow("OBJ Viewer");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        cout << "GLEW Initialized\n";
    }

    make_shaderProgram();

    // 로딩바와 함께 모델 로드
    loadModelWithProgress(
        modelPath,
        modelName,
        modelType,
        glm::scale(glm::mat4(1.0f), modelScale),
        models
    );

    // 디버깅 출력
    if (debugStatus1 == "y")
        debug_model(models.back());
    if (debugStatus2 == "y")
        debug_materials(models.back()->materials);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecial);

    InitBuffer();
    
    drawScene();

    glutMainLoop();

    return 0;
}

GLvoid drawScene() {

    glClearColor(1.0, 1.0, 1.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(shaderProgramID);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Error in glUseProgram: " << error << std::endl;
    }

    view = glm::mat4(1.0f);
    view = glm::lookAt(
        cameraPos,
        cameraDirection,
        cameraUp
    );
    unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 300.0f);
    unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

    GLint lightPosLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

    glEnable(GL_DEPTH_TEST);
    for (const auto& model : models) { // 실제 모델 draw
        model->draw(shaderProgramID, isKeyPressed_s);
    }
    glDisable(GL_DEPTH_TEST);

    glutSwapBuffers();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cout << "OpenGL error: " << err << endl;
    }
}

// 버퍼 초기화 함수
void InitBuffer() {
    //-----------------------------------------------------------------------------------------------------------
    for (const auto& model : models) { // 모든 모델 initBuffer
        model->initBuffer();
    }
}