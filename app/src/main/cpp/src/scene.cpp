#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));

    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

bool mouseDown = false;
void Scene::update(float deltaTime) {

    static float time = 0.0f;
    if(mouseDown) {
        time += deltaTime;
    }

    glm::mat4 m_p[28], m_d_i[28], m_a[28], m_i[28], m_l[28], Rx, Ry, Rz;
    quat q[4][28];
    // calculate m_d;
    for(int i = 0; i < 28; i++) {
        m_p[i] = translate(jOffsets[i]);
        m_d_i[i] = (i == 0 ) ? mat4() : inverse(m_p[i]) * m_d_i[jParents[i]];
    }

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 28; j++) {
            Rx = rotate(radians(motions[i][3 * (j + 1)]), vec3(1.0f, 0.0f, 0.0f));
            Ry = rotate(radians(motions[i][3 * (j + 1) + 1]), vec3(0.0f, 1.0f, 0.0f));
            Rz = rotate(radians(motions[i][3 * (j + 1) + 2]), vec3(0.0f, 0.0f, 1.0f));

            q[i][j] = quat_cast(Rz * Rx * Ry);
        }
    }

    float angle = time - int(time);

    quat qr[28];

    for(int j = 0; j < 28; j++) {
        qr[j] = glm::lerp(q[int(time) % 4][j], q[(int(time) + 1) % 4][j], angle);
        m_l[j] = mat4_cast(qr[j]);
        m_a[j] = j == 0 ? mat4() : m_a[jParents[j]] * m_p[j] *m_l[j];
        m_i[j] = m_a[j] * m_d_i[j];
    }


    vector<Vertex> vertices = playerVertices;

    for(Vertex & v: vertices) {
        vec3 pos = vec3(0);
        for(int i = 0 ; i < 4; i++) {
            if(v.bone[i] != -1) {
                pos += vec3(m_i[v.bone[i]] * vec4(v.pos, 1.0f)) * v.weight[i];
            }
            else break;
        }
        v.pos = pos;
    }


    Scene::program->use();

    Scene::camera->update();

    Scene::player->load(vertices, playerIndices);
    Scene::player->draw();

    // Line Drawer & Debugger
//    glLineWidth(20);
//    Scene::lineDraw->load({{vec3(-20.0f, 0.0f, 0.0f)}, {vec3(20.0f, 0.0f, 0.0f)}}, {0, 1});
//    Scene::lineDraw->draw();

    LOG_PRINT_DEBUG("You can also debug variables with this function: %f", M_PI);
}

void Scene::mouseUpEvents(float x, float y, bool doubleTouch) {
    mouseDown = false;
}

void Scene::mouseDownEvents(float x, float y, bool doubleTouch) {
    mouseDown = true;
}

void Scene::mouseMoveEvents(float x, float y, bool doubleTouch) {

}