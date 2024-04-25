// OpenGLGraphics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//define stb_image_implementation before including stb_image.h
#define STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Materials.h"
#include "MeshData.h"
#include "ModelData.h"


const GLint WIDTH = 1600, HEIGHT = 900; //set the window size
GLuint VAO, VBO, EBO, shader, MoveLocation, ProjectionLocation, ViewLocation, ambientColorLoc, ambientIntensityLoc,matSpeculInt, matSpeculShin, diffuseIntensityLoc, LightDirectionLoc;
bool direction = true;
float raidusTransform = 3.1415926f / 180.0f;
float triOffset = 0.0f;
float triMaxOffset = 90.0f * (3.1415926f / 180.0f);
float triIncrement = 0.01f;

//vertex data, vertex specification
std::vector<MeshData> meshList;
//vertex shader/fragment shader, render pipeline
std::vector<Shader> shaderList;

PointLight pointLight[NUM_POINT_LIGHTS];
SpotLight spotLight[NUM_SPOT_LIGHTS];

//variables holding textures
Texture brickTexture;
Texture dirtTexture;
void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticesCount, int vLength, int normalOffset)
{
    //for each three vertices indices in the element indices array
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
        normal = glm::normalize(normal);
        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;
        //compute the average normal of the three faces
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }
    for (size_t i = 0; i < verticesCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void CreateTriangle()
{
    GLfloat vertices1[] = {
        //x,y,z,u,v,normal.x,normal.y,normal.z
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f, //bottom left corner
        1.0f, -1.0f, 0.0f, 1.0f,0.0f, 0.0f,0.0f,0.0f,//bottom right corner
        0.0f, 1.0f, 0.0f, 0.5f,1.0f, 0.0f,0.0f,0.0f,//top middle corner
        0.0f,0.0f,-1.0f, 0.5f,0.5f, 0.0f,0.0f,0.0f,//middle point
    };

    GLfloat vertices2[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,//bottom left corner
        1.0f, 1.0f, 0.0f, 1.0f,0.0f, 0.0f,0.0f,0.0f,//bottom right corner
        0.0f, 3.0f, 0.0f, 0.5f,1.0f, 0.0f,0.0f,0.0f,//top middle corner
        0.0f,2.0f,-1.0f, 0.5f,0.5f, 0.0f,0.0f,0.0f //middle point
    };

    GLfloat floorVertices[] = {
		-10.0f,0.0f,-10.0f,0.0f,0.0f,0.0f,1.0f,0.0f,
		10.0f,0.0f,-10.0f,10.0f,0.0f,0.0f,1.0f,0.0f,
		10.0f,0.0f,10.0f,10.0f,10.0f,0.0f,1.0f,0.0f,
		-10.0f,0.0f,10.0f,0.0f,10.0f,0.0f,1.0f,0.0f
	};

    //build indices for the floor, two triangles meshes
    GLuint floorIndices[] = {
        0,3,1, //first triangle
        1,3,2 //second triangle
        };

    GLuint indices1[] = {
		0,3,1, //first triangle
        1,3,2, //second triangle
        2,3,0, //third triangle
        0,1,2 //fourth triangle
	};
    GLuint indices2[] = {
        0,3,1, //first triangle
        1,3,2, //second triangle
        2,3,0, //third triangle
        0,1,2 //fourth triangle
    };
    MeshData *obj1 = new MeshData();
    MeshData *obj2 = new MeshData();
    MeshData *floor = new MeshData();
    CalcAverageNormals(indices1, 12, vertices1, 32, 8, 5);
    CalcAverageNormals(indices2, 12, vertices2, 32, 8, 5);
    obj1->CreateMesh(vertices1, 32, indices1, 12);
    obj2->CreateMesh(vertices2, 32, indices2, 12);
    floor->CreateMesh(floorVertices, 32, floorIndices, 6);
    
    meshList.push_back(*obj1);
    meshList.push_back(*obj2);
    meshList.push_back(*floor);
}



int main()
{
    ///-----Main UI Windows Init-----///
    meshList.reserve(10);
    const char* vShader = "Shaders/firstShader.vert";
    const char* fShader = "Shaders/firstShader.frag";
    Window mainWindow = Window(WIDTH, HEIGHT);
    mainWindow.Initialise();

    GLuint bufferWidth = mainWindow.getBufferWidth();
    GLuint bufferHeight = mainWindow.getBufferHeight();

    ///-----Load Texture-----///
    Texture brickTexture = Texture("Texture/pexels.jpg");
    brickTexture.LoadTexture();
    Texture dirtTexture = Texture("Texture/wall.jpg");
    dirtTexture.LoadTexture();

    ///-----Create Triangle(Mesh)-----///
    CreateTriangle();

    ///-----build shader-----///
    Shader shader = Shader();
    //shader.CreateFromString(vShader, fShader);
    shader.CreateFromFiles(vShader, fShader);
    shader.AssignUniformCameraPosition("cameraPosition");
    shader.AssignUniformModelLoc("model");
    shader.AssignUniformProjectLoc("projectMat");
    shader.AssignUniformViewLoc("viewMat");
    shader.AssignUniformAmbientColorLoc("directionalLight.base.color");
    shader.AssignUniformAmbientIntensityLoc("directionalLight.base.ambientIntensity");
    shader.AssignUniformDiffuseIntensityLoc("directionalLight.base.diffuseIntensity");
    shader.AssignUniformDirectionLoc("directionalLight.direction");
    shader.AssignUniformMatSpecularIntLoc("material.specularIntensity");
    shader.AssignUniformMatSpecularShinLoc("material.shininess");


    //NOTICE---------FinalStep other shader settings should before this line---------NOTICE//

    shaderList.push_back(shader);

    
    //-------Create Light components--------//
    //light direction vec, 45 degree angle downwards, the direction is from light source to the object, need to be reversed in the shader
    glm::vec3 lightDirection = glm::vec3(0.0f, -glm::sqrt(2.0f) / 2.0f, -glm::sqrt(2.0f) / 2.0f); //-glm::sqrt(2.0f) / 2.0f
    Light lightsource = Light(1.0f, 1.0f, 1.0f, 0.2f, 0.6f);
    //create the light object
    DirectionalLight mainLight(lightsource, lightDirection);

    //create point light
    pointLight[0] = PointLight(0.0f, 0.0f, 1.0f, 
                               0.1f, 1.0f, 
                               2.0f, 2.0f, -2.0f, 
                               0.3f, 0.2f, 0.1f);
    pointLight[1] = PointLight(1.0f, 0.0f, 0.0f,
                               0.6f, 1.0f,
							   0.0f, 0.0f, -2.0f,
							   0.3f, 0.2f, 0.1f);
    unsigned int pointLightCount = 2;

    //create spot light
    spotLight[0] = SpotLight(1.0f, 1.0f, 1.0f,
        					0.1f, 4.0f,
        					0.0f, 0.0f, -1.5f,
        					glm::vec3(0.0f,0.0f,-1.0f),
        					0.4f, 0.3f, 0.2f,
        					5.0f, 30.0f);
    spotLight[1] = SpotLight(1.0f, 1.0f, 0.0f,
        					0.1f, 2.0f,
        					0.0f, 3.0f, -2.5f,
        					glm::vec3(0.0f,-1.0f,0.0f),
        					1.0f, 0.0f, 0.0f,
        					20.0f, 30.f);
    unsigned int spotLightCount = 2;
    //-------End Light creation --------//

    glEnable(GL_DEPTH_TEST); //enable depth testing

    /// --- import model --- ///
    stbi_set_flip_vertically_on_load(true);
    ModelData modelData = ModelData();
    modelData.LoadModel("Models/backpack.obj");

    /// --- End import model --- ///

    //create Material
    Materials dullMaterial = Materials(0.8f, 32.0f);
    Materials shinyMaterial = Materials(4.0f, 256.0f);

    //initialize the camera object
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1.0f, 0.0f), 90.0f, 0.0f, 0.5f, 0.01f);
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f); //create the projection matrix
    ProjectionLocation = shaderList[0].GetProjectionLocation();
    MoveLocation = shaderList[0].GetModelLocation();
    ViewLocation = shaderList[0].GetViewLocation();
    ambientColorLoc = shaderList[0].GetAmbientColorLocation();
    ambientIntensityLoc = shaderList[0].GetAmbientIntensityLocation();
    diffuseIntensityLoc = shaderList[0].GetDiffuseIntensityLocation();
    LightDirectionLoc = shaderList[0].GetDirectionLocation();
    matSpeculInt = shaderList[0].GetSpecularIntensityLocation();
    matSpeculShin = shaderList[0].GetSpecularShininessLocation();

    double lastTime = glfwGetTime();
    double accuTime = glfwGetTime();
    //loop until window closed
    while (!mainWindow.getShouldClose())
    {
        double currentTime = glfwGetTime();
        camera.KeyControl(mainWindow.getKeys(), currentTime - lastTime);
        camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        lastTime = currentTime;
        //get + handle user input events(keyboard, mouse, etc.)
        glfwPollEvents();
        //clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //red
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer only
        /*if (direction) {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if (triOffset >= triMaxOffset || triOffset <= -triMaxOffset)
        {
            direction = !direction;
        }*/
        
        shaderList[0].UseShader(); //use the shader program
        
        glm::mat4 model(1.0f); //create the model matrix with identity matrix
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); //move the model front around 2.5meters
        model = glm::rotate(model, triOffset, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        
        glm::mat4 viewMat = camera.GetViewMatrix();
        
        //model = glm::translate(model,glm::vec3(triOffset, triOffset, 0.0f)); //translate the model matrix   

        //set the uniform geometric transformation variable value
        glUniformMatrix4fv(MoveLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(viewMat));
        //assign camera position to the shader
        glUniform3f(shaderList[0].GetCameraPosition(), camera.GetCameraPosition().x, camera.GetCameraPosition().y,camera.GetCameraPosition().z);
        
        //Set up TExture
        brickTexture.UseTexture();

        //Set up the light conditions
        mainLight.UseLight(ambientIntensityLoc, ambientColorLoc,diffuseIntensityLoc, LightDirectionLoc);
        shaderList[0].SetPointLights(pointLight,pointLightCount);
        //update the spot light pose to align with the camera before set the spotlights
        spotLight[0].SetPose(camera.GetCameraPosition()+glm::vec3(0.0f,-0.5f,0.0f), camera.GetFrontDirection());
        shaderList[0].SetSpotLights(spotLight, spotLightCount);

        //set up material and render the two pyramids
        shinyMaterial.UseMaterial(matSpeculInt, matSpeculShin);
        meshList[0].RenderMesh(); //render the mesh

        dirtTexture.UseTexture();
        meshList[1].RenderMesh(); //render the mesh

        //set up the ground object
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(MoveLocation, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        shinyMaterial.UseMaterial(matSpeculInt, matSpeculShin);
        meshList[2].RenderMesh(); //render the mesh


        //set up the ground object
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(MoveLocation, 1, GL_FALSE, glm::value_ptr(model));
        dullMaterial.UseMaterial(matSpeculInt, matSpeculShin);
        modelData.RenderModel(shaderList[0]);

        glUseProgram(0); //unbind the shader program

        mainWindow.swapBuffers();//swap the buffers, double buffering
        glfwPollEvents();

    }
    glfwTerminate(); //terminate GLFW
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
