#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{

public:
	Shader();
	~Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
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
	void AssignUniformLocWithName(const char* uniformName, GLuint* uniformLocation);
	std::string ReadFile(const char* fileLocation);
	GLuint GetShaderLocation();

	void SetPointLights(PointLight * _pointLights, unsigned int _lightCount);
	void SetSpotLights(SpotLight* _spotLights, unsigned int _lightCount);
	void UseShader();
	void ClearShader();
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

private:
	GLuint cameraPosition, shaderProgram=0, uniformProjectionLoc, uniformModelLoc, uniformViewLoc, uniformMatSpecularIntLoc,uniformMatSpecularShinLoc;
	GLuint pointLightsCountLoc, spotLightsCountLoc; //light components
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

};

