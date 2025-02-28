#include "lab_m1/tema3/tema3.h"
#include "lab_m1/lab4/transform3D.h"
#include "lab_m1/lab9/lab9.h"
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/glm_utils.h"

#include "core/gpu/mesh.h"
#include <iostream>
#include <vector>
using namespace std;
using namespace m1;

struct BalloonConfig {
    float radius;      
    float height;      
    float speed = 2.0f;       
    float angle;       
    string texture;

    glm::vec3 position;      
    glm::vec3 velocity;       
    glm::vec3 acceleration;   
    float mass = 1.0f;        
    bool usePhysics = false;  
    float windStrength = 0.0f;
    float windTimer = 0.0f;   

};

vector<BalloonConfig> balloons;


Tema3::Tema3()
{
    animationTime = 0.0f;
}

Tema3::~Tema3()
{

}

void Tema3::Init() {

    /*Lab9 lab9Instance;*/
    //lab9 instance


    const string textureLoc = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "crate.jpg");
    const string textureballoon = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "bluepink.jpg");
    const string textureballoon1 = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "paint.jpg");
    const string textureballoon2 = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "stray.jpg");
    const string testureballoon3 = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "bluemarble.jpg");
    const string textureballoon4 = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "yellowmarble.jpg");
    const string textureballoon5 = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "colorfulmarble.jpg");

    const string textureSoil = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "soil.jpeg");
    const string textureGrass = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "grass.jpg");

    const string heightmapText = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "heightmap256.png");

    cout << textureLoc << endl;

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;

    }

    //load heightmap texture and the grass and soil textures
    {
       
        Texture2D* heightmap = new Texture2D();
        heightmap->Load2D(heightmapText.c_str(), GL_REPEAT);
        mapTextures["heightmap"] = heightmap;

        Texture2D* soilTexture = new Texture2D();
        soilTexture->Load2D(textureSoil.c_str(), GL_REPEAT);
        mapTextures["soilTexture"] = soilTexture;

        Texture2D* grassTexture = new Texture2D();
        grassTexture->Load2D(textureGrass.c_str(), GL_REPEAT);
        mapTextures["grassTexture"] = grassTexture;

        if (!mapTextures["soilTexture"] || mapTextures["soilTexture"]->GetTextureID() == 0) {
            std::cout << "Error loading soilTexture!" << std::endl;
        }
        if (!mapTextures["grassTexture"] || mapTextures["grassTexture"]->GetTextureID() == 0) {
            std::cout << "Error loading grassTexture!" << std::endl;
        }
        if (!mapTextures["heightmap"] || mapTextures["heightmap"]->GetTextureID() == 0) {
            std::cout << "Error loading heightmap!" << std::endl;
        }
    }


	//load the box texture and the 5 balloon textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(textureLoc.c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;

        if (texture->GetTextureID() == 0) {
            std::cout << "Failed to load texture!" << std::endl;
        }
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(textureballoon.c_str(), GL_REPEAT);
        mapTextures["bluepink"] = texture;

        if (texture->GetTextureID() == 0) {
            std::cout << "Failed to load texture!" << std::endl;
        }
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(textureballoon1.c_str(), GL_TEXTURE_WRAP_T);
        mapTextures["paint"] = texture;

        if (texture->GetTextureID() == 0) {
            std::cout << "Failed to load texture!" << std::endl;
        }
    }


    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(testureballoon3.c_str(), GL_REPEAT);
        mapTextures["bluemarble"] = texture;

        if (texture->GetTextureID() == 0) {
            std::cout << "Failed to load texture!" << std::endl;
        }
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(textureballoon4.c_str(), GL_REPEAT);
        mapTextures["yellowmarble"] = texture;

        if (texture->GetTextureID() == 0) {
            std::cout << "Failed to load texture!" << std::endl;
        }
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(textureballoon5.c_str(), GL_REPEAT);
        mapTextures["colorfulmarble"] = texture;

        if (texture->GetTextureID() == 0) {
            std::cout << "Failed to load texture!" << std::endl;
        }
    }

    //shader for box meaning base of balloon
    {
        Shader* shader = new Shader("ShaderBase");
        shader->AddShader("src/lab_m1/tema3/VertexShader.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema3/FragmentShader.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //shader for balloon upper part
    {
        Shader* shader = new Shader("ShaderBalloon");
        shader->AddShader("src/lab_m1/tema3/BalloonVertexShader.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema3/BalloonFragmentShader.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

	//shader for ground
    {
        Shader* shader = new Shader("TerrainShader");
        shader->AddShader("src/lab_m1/tema3/TerrainVertexShader.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema3/TerrainFragmentShader.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    //DrawBase("cube", glm::vec3(1, 0, 0));

    //

    balloons = {
            {5.0f, 0.0f, 0.5f, 0.0f, "bluepink"},
            {10.0f, 2.0f, 0.3f, 72.0f, "paint"},
            {25.0f, 9.5f, 0.4f, 144.0f, "yellowmarble"},
            {17.0f, 12.0f, 0.6f, 216.0f, "bluemarble"},
            {29.5f, 7.0f, 0.2f, 288.0f, "colorfulmarble"}
    };

    Mesh* terrainMesh = GenerateTerrainMesh("terrain", 200, 200);
    meshes["terrain"] = terrainMesh;

}

void Tema3::FrameStart() {

    // clears the color buffer and depth buffer
    glClearColor(0.933f, 0.686f, 0.380f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // get window resolution and set viewport
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

}

void Tema3::RenderTerrain() {

    //move the terrain so the balloons are floating above it
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-100.0f, -40.0f, -100.0f));


    Shader* shader = shaders["TerrainShader"];
    if (!shader || !shader->program)
        return;

    glUseProgram(shader->program);
    glUniform1i(glGetUniformLocation(shader->program, "isSun"), 0); //is not sun flag


    if (!mapTextures["soilTexture"] || !mapTextures["grassTexture"] || !mapTextures["heightmap"]) {
        std::cout << "Error: One or more textures are missing!" << std::endl;
        return;
    }

	//soil texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mapTextures["soilTexture"]->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "soilTexture"), 0);

    //grass texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mapTextures["grassTexture"]->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "grassTexture"), 1);

    //hieghtmap texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mapTextures["heightmap"]->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "heightmap"), 2);

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE,
        glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE,
        glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    //render mesh for the whole ground
    RenderSimpleMeshText(meshes["terrain"], shader, modelMatrix, nullptr, nullptr);
}




void Tema3::Update(float deltaTimeSeconds) {
    animationTime += deltaTimeSeconds;
    RenderTerrain();

    glm::vec3 sunPosition = glm::vec3(0.0f, 100.0f, -50.0f);
    RenderSun(sunPosition);

    const float GRAVITY = -9.81f;
    const float BALLOON_LIFT = 15.0f;
    const float MAX_WIND_STRENGTH = 5.0f;
    const float WIND_CHANGE_INTERVAL = 2.0f;

    for (auto& balloon : balloons) {
        if (balloon.texture == "bluepink") {
            if (!balloon.usePhysics) {
                balloon.usePhysics = true;
                balloon.position = glm::vec3(
                    30.0f,
                    balloon.height + 20.0f,
                    0.0f
                );
                balloon.velocity = glm::vec3(0.0f); //start from resting state
                balloon.acceleration = glm::vec3(0.0f);
            }

			//change wind direction after a certain interval
            balloon.windTimer += deltaTimeSeconds;
            if (balloon.windTimer >= WIND_CHANGE_INTERVAL) {
                balloon.windTimer = 0.0f;
				//random wind intensity between -1 and 1 multiplied by MAX_WIND_STRENGTH
                balloon.windStrength = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * MAX_WIND_STRENGTH;
            }

            //F = m * g
            glm::vec3 gravityForce = glm::vec3(0.0f, GRAVITY * balloon.mass, 0.0f);
            //wind force activated on OX
            glm::vec3 windForce = glm::vec3(balloon.windStrength, 0.0f, 0.0f);

			//lift only when pressing space
            glm::vec3 liftForce = glm::vec3(0.0f, isLiftActive ? BALLOON_LIFT : 0.0f, 0.0f);

			//balance 80% of gravity making the balloon float
            glm::vec3 buoyancyForce = glm::vec3(0.0f, -GRAVITY * balloon.mass * 0.8f, 0.0f);

            //a = F/m 
            balloon.acceleration = (gravityForce + windForce + liftForce + buoyancyForce) / balloon.mass;

			//v = v0+a*t
            balloon.velocity += balloon.acceleration * deltaTimeSeconds;

			//reduce velocity of balloon by simulating air friction
            balloon.velocity *= 0.99f;

			//x = x0+v*t
            balloon.position += balloon.velocity * deltaTimeSeconds;

			//make balloon stay within certain bounds
            balloon.position.x = glm::clamp(balloon.position.x, -50.0f, 50.0f);
            balloon.position.y = glm::clamp(balloon.position.y, 5.0f, 50.0f);
            balloon.position.z = glm::clamp(balloon.position.z, -50.0f, 50.0f);
            RenderBalloon(balloon.position.x, balloon.position.z, balloon.position.y, balloon.texture);
        }
        else {
			//for the other ballons make them rotate around the center
            float currentAngle = animationTime * balloon.speed;
            float x = centerX + balloon.radius * cos(currentAngle);
            float z = centerZ + balloon.radius * sin(currentAngle);
            RenderBalloon(x, z, balloon.height, balloon.texture);
        }
    }
}


void Tema3::RenderBalloon(float xOffset, float zOffset, float heightOffset, const std::string& textureName) {
    //box/crate
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(xOffset, heightOffset, zOffset);
        modelMatrix *= transform3D::Scale(1.0f, 0.5f, 1.0f);
        RenderSimpleMeshText(meshes["cube"], shaders["ShaderBase"], modelMatrix, mapTextures["crate"], nullptr);
    }

    //render bars
    float columnHeight = 1.4f;
    float columnWidth = 0.1f;
    float columnOffset = 0.4f;

    {
        float xCol = xOffset + columnOffset;
        float zCol = zOffset - columnOffset;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(xCol, heightOffset + 0.5f, zCol);
        modelMatrix *= transform3D::Scale(columnWidth, columnHeight, columnWidth);
        RenderSimpleMeshText(meshes["cube"], shaders["ShaderBase"], modelMatrix, mapTextures["crate"], nullptr);
    }

    {
        float xCol = xOffset + columnOffset;
        float zCol = zOffset + columnOffset;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(xCol, heightOffset + 0.5f, zCol);
        modelMatrix *= transform3D::Scale(columnWidth, columnHeight, columnWidth);
        RenderSimpleMeshText(meshes["cube"], shaders["ShaderBase"], modelMatrix, mapTextures["crate"], nullptr);
    }

    {
        float xCol = xOffset - columnOffset;
        float zCol = zOffset + columnOffset;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(xCol, heightOffset + 0.5f, zCol);
        modelMatrix *= transform3D::Scale(columnWidth, columnHeight, columnWidth);
        RenderSimpleMeshText(meshes["cube"], shaders["ShaderBase"], modelMatrix, mapTextures["crate"], nullptr);
    }

    {
        float xCol = xOffset - columnOffset;
        float zCol = zOffset - columnOffset;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(xCol, heightOffset + 0.5f, zCol);
        modelMatrix *= transform3D::Scale(columnWidth, columnHeight, columnWidth);
        RenderSimpleMeshText(meshes["cube"], shaders["ShaderBase"], modelMatrix, mapTextures["crate"], nullptr);
    }

    //upper part of balloon
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(xOffset, heightOffset + 2.5f, zOffset);
        modelMatrix *= transform3D::Scale(2.6f, 1.95f, 2.6f);
        RenderSimpleMeshText(meshes["sphere"], shaders["ShaderBalloon"], modelMatrix, mapTextures[textureName], nullptr);
    }
}


void Tema3::RenderSun(glm::vec3 sunPosition) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, sunPosition);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f)); //sun size

    Shader* shader = shaders["TerrainShader"];

	glUniform1i(glGetUniformLocation(shader->program, "isSun"), 1); //is sun flag
    RenderSimpleMeshText(meshes["sphere"], shader, modelMatrix, nullptr, nullptr);

}


void Tema3::FrameEnd() {

    //DrawCoordinateSystem();

}

void Tema3::OnInputUpdate(float deltaTime, int mods) {

}

void Tema3::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_SPACE) {
        isLiftActive = true;
    }
}

void Tema3::OnKeyRelease(int key, int mods) {
    if (key == GLFW_KEY_SPACE) {
        isLiftActive = false;
    }
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {

}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {

}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

}

void Tema3::OnWindowResize(int width, int height) {

}

Mesh* Tema3::GenerateTerrainMesh(const std::string& name, int width, int height) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            glm::vec3 position = glm::vec3(x, 0, z);
            glm::vec3 normal = glm::vec3(0, 1, 0);
            glm::vec2 texCoord = glm::vec2(
                static_cast<float>(x) / (width - 1), //coord U
                static_cast<float>(z) / (height - 1) //coord V
            );
            glm::vec3 color = glm::vec3(1, 1, 1);

            vertices.push_back(VertexFormat(position, color, normal, texCoord));
        }
    }

    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int i0 = z * width + x;
            int i1 = i0 + 1;
            int i2 = i0 + width;
            int i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);
        }
    }

    Mesh* terrain = new Mesh(name);
    terrain->InitFromData(vertices, indices);
    return terrain;
}




void Tema3::RenderSimpleMeshText(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need
    //glUniform1i(glGetUniformLocation(shader->program, "mix_grassandsoil"), mix_grassandsoil);


    if (texture1)
    {
        // Activate texture location 0
        glActiveTexture(GL_TEXTURE0);

        // Bind the texture1 ID
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());

        // Send texture uniform value
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    }

    if (texture2)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);

    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}