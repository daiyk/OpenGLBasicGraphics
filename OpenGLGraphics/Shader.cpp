#include "Shader.h"
#include <cassert>

Shader::Shader()
{
	shaderProgram = 0;
	uniformModelLoc = 0;
	uniformProjectionLoc = 0;
	uniformViewLoc = 0;
	pointLightCount = 0;
}
void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}
GLuint Shader::GetShaderLocation()
{
	return shaderProgram;
}

void Shader::SetPointLights(PointLight* _pointLights, unsigned int _lightCount)
{
	glUniform1i(pointLightsCountLoc, _lightCount);
	for (int i = 0; i < _lightCount; i++)
	{
		_pointLights[i].UseLight(this->pointLights[i].base.uniformAmbientIntensity, this->pointLights[i].base.uniformColor, this->pointLights[i].base.uniformDiffuseIntensity, pointLights[i].uniformPosition, pointLights[i].uniformConstant, pointLights[i].uniformLinear, pointLights[i].uniformExponent);
	}
}
void Shader::AssignUniformLocWithName(const char* uniformName, GLuint* uniformLocation) {
	*uniformLocation = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::AssignUniformDiffuseIntensityLoc(const char* uniformName) {
	baseLight.uniformDiffuseIntensity = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::AssignUniformDirectionLoc(const char* uniformName) {
	directionalLight.uniformDirection = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::AssignUniformModelLoc(const char* uniformName) {
	uniformModelLoc = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::AssignUniformCameraPosition(const char* uniformName) {
	cameraPosition = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::AssignUniformViewLoc(const char* uniformName) {
	uniformViewLoc = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::AssignUniformAmbientIntensityLoc(const char* uniformName) {
	baseLight.uniformAmbientIntensity = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::AssignUniformAmbientColorLoc(const char* uniformName) {
	baseLight.uniformColor = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::AssignUniformProjectLoc(const char* uniformName) {
	uniformProjectionLoc = glGetUniformLocation(shaderProgram, uniformName);
}


void Shader::AssignUniformMatSpecularIntLoc(const char* uniformName) {
	uniformMatSpecularIntLoc = glGetUniformLocation(shaderProgram, uniformName);
}
void Shader::AssignUniformMatSpecularShinLoc(const char* uniformName) {
	uniformMatSpecularShinLoc = glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
	shaderProgram = glCreateProgram(); //create the shader program
	if (!shaderProgram)
	{
		printf("Error creating shader program!");
		return;
	}
	
	AddShader(shaderProgram, vertexCode, GL_VERTEX_SHADER); //add the vertex shader to the shader program
	AddShader(shaderProgram, fragmentCode, GL_FRAGMENT_SHADER); //add the fragment shader to the shader program

	glLinkProgram(shaderProgram); //link the shader program
	GLint success = 0;
	//get the link status after linking the shader program
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		printf("Error linking shader program: '%s'\n", infoLog);
		return;
	}
	glValidateProgram(shaderProgram); //validate the shader program
	//get the validation status after validating the shader program
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		printf("Error validating shader program: '%s'\n", infoLog);
		return;
	}

	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{

		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		std::string locStr = locBuff;
		pointLights[i].base.uniformColor = glGetUniformLocation(shaderProgram, locStr.c_str());
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		pointLights[i].base.uniformAmbientIntensity = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		pointLights[i].base.uniformDiffuseIntensity = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		pointLights[i].uniformPosition = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		locStr = locBuff;
		pointLights[i].uniformConstant = glGetUniformLocation(shaderProgram, locStr.c_str());
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		locStr = locBuff;
		pointLights[i].uniformLinear = glGetUniformLocation(shaderProgram, locStr.c_str());
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		locStr = locBuff;
		pointLights[i].uniformExponent = glGetUniformLocation(shaderProgram, locStr.c_str());

	}
	pointLights[0].uniformConstant = glGetUniformLocation(shaderProgram, "pointLights[0].constant");
	pointLights[1].uniformConstant = glGetUniformLocation(shaderProgram, "pointLights[1].constant");
	pointLights[0].uniformLinear = glGetUniformLocation(shaderProgram, "pointLights[0].linear");

	pointLights[0].uniformExponent = glGetUniformLocation(shaderProgram, "pointLights[0].exponent");
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation) {
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	CompileShader(vertexCode, fragmentCode);
}
std::string Shader::ReadFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);
	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType); //create the shader
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength); //attach the shader source code to the shader
	glCompileShader(theShader); //compile the shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); //check for errors
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader); //attach the shader to the program
}
GLuint Shader::GetCameraPosition() {
	return cameraPosition;
}
GLuint Shader::GetAmbientColorLocation() {
	return baseLight.uniformColor;
}
GLuint Shader::GetAmbientIntensityLocation() {
	return baseLight.uniformAmbientIntensity;
}

void Shader::UseShader()
{
	glUseProgram(0); //unbind the shader program
	glUseProgram(shaderProgram); //bind the current shader program
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjectionLoc;
}

GLuint Shader::GetModelLocation()
{
	return uniformModelLoc;
}

GLuint Shader::GetViewLocation() {
	return uniformViewLoc;
}

GLuint Shader::GetDiffuseIntensityLocation() {
	return baseLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation() {
	return directionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation() {
	return uniformMatSpecularIntLoc;
}
GLuint Shader::GetSpecularShininessLocation() {
	return uniformMatSpecularShinLoc;
}
GLuint* Shader::GetPointLightCountLocation() {
	return &pointLightsCountLoc;
}
Shader::~Shader()
{
	ClearShader();
}

void Shader::ClearShader()
{
	if (shaderProgram != 0)
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}
	uniformModelLoc = 0;
	uniformProjectionLoc = 0;
}