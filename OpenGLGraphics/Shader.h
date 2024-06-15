#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <GL/glew.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "PointLight.h"
#include "SpotLight.h"
#include "PCHTypes.h"

class Shader
{

public:
	Shader();
	~Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	//three shaders pipeline version
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	GLuint GetCameraPosition();
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetSpecularShininessLocation();
	GLuint* GetPointLightCountLocation();
	GLuint* GetSpotLightCountLocation();
	GLuint GetShaderLocation();

	/// <summary>
	///  partial assignmentr of the uniform variables location are through the AssignUniform... functions, and the rest are through the GetUniform... functions which are inside CompileVertFrag function
	/// </summary>
	void AssignUniformCameraPosition(const char* uniformName);
	void AssignUniformDiffuseIntensityLoc(const char* uniformName);
	void AssignUniformDirectionLoc(const char* uniformName);
	void AssignUniformProjectLoc(const char* uniformName);
	void AssignUniformModelLoc(const char* uniformName);
	void AssignUniformViewLoc(const char* uniformName);
	void AssignUniformAmbientIntensityLoc(const char* uniformName);
	void AssignUniformAmbientColorLoc(const char* uniformName);
	void AssignUniformMatSpecularIntLoc(const char* uniformName);
	void AssignUniformMatSpecularShinLoc(const char* uniformName);
	GLuint GetUniformLocation(const char* uniformName);
	std::string ReadFile(const char* fileLocation);


	int SetPointLights(PointLightVector& _pointLights, unsigned int _lightCount, unsigned int shadowMapStartUnit);
	void SetSpotLights(SpotLightVector& _spotLights, unsigned int _lightCount);
	/// ----- shadow map components ----- ///
	void SetUniformDirectionalShadowMap(std::string directionalShadowMapName, GLuint textureUnit);
	void SetDirectionalLightTransform(const glm::mat4* _lightTransform);
	void SetUniformOmniLightPos(const glm::vec3 _lightPos);
	void SetOmniLightMatrices(const glm::mat4* _lightMatrices, GLuint _lightMatricesSize);
	void SetFarPlane(float _farPlane);

	void UseShader();
	void ClearShader();
	void Validate();
	/// <summary>
	/// basic light components
	/// </summary>
	struct BaseLight
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
	} baseLight;

	struct
	{
		BaseLight base;
		GLuint uniformDirection;
	} directionalLight;

	/// <summary>
	/// maximum point lights number is defined in light.h
	/// </summary>
	struct pointLight
	{
		BaseLight baseLight;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
		GLuint uniformFarPlane;
	} pointLights[NUM_POINT_LIGHTS];
	/// <summary>
	/// maximum spot lights number is defined in light.h
	/// </summary>
	struct {
		pointLight basePointLight;
		GLuint uniformSpotDirection;
		GLuint uniformEdge;
		GLuint uniformOuterEdge;
	} spotLights[NUM_SPOT_LIGHTS];

	/// <summary>
	/// for the omi-directional shadow map for each point light and spot light
	/// </summary>
	struct
	{
		GLuint uniformShadowMap;
		GLuint uniformFarPlane;
	} omniShadowMap[NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS];;

private:
	GLuint cameraPosition, shaderProgram = 0, uniformProjectionLoc, uniformModelLoc, uniformViewLoc, uniformMatSpecularIntLoc,
		uniformMatSpecularShinLoc;
	GLuint pointLightsCountLoc, spotLightsCountLoc; //light components
	/// ----- shadow map components during rendering ----- ///
	GLuint uniformDirectionalLightTransform;
	GLuint uniformShaderMap;
	/// ----- omini-shadow map components during building ----- ///
	GLuint uniformOmniLightPosBuilding, uniformFarPlaneBuilding;
	GLuint uniformOmnilightTransformsBuilding[6];  
	void CompileVertFrag(const char* vertexCode, const char* fragmentCode);
	void CompileVertFragGeom(const char* vertexCode, const char* geometryCode, const char* fragmentCode);

	void CompileShader();
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void InitShaderUniformVariables();
};

