#include "Shader.h"
#include <cassert>

Shader::Shader()
{
	shaderProgram = 0;
	uniformModelLoc = 0;
	uniformProjectionLoc = 0;
	uniformViewLoc = 0;
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
	assert(("Light count cannot exceeded the MACRO NUM_POINT_LIGHT!", _lightCount <= NUM_POINT_LIGHTS));
	glUniform1i(pointLightsCountLoc, _lightCount);
	for (int i = 0; i < _lightCount; i++)
	{
		_pointLights[i].UseLight(
			this->pointLights[i].baseLight.uniformAmbientIntensity, 
			this->pointLights[i].baseLight.uniformColor, 
			this->pointLights[i].baseLight.uniformDiffuseIntensity, 
			this->pointLights[i].uniformPosition, 
			this->pointLights[i].uniformConstant,
			this->pointLights[i].uniformLinear, 
			this->pointLights[i].uniformExponent
		);
	}
}
void Shader::SetSpotLights(SpotLight* _spotLights, unsigned int _lightCount)
{
	assert(("Light count cannot exceeded the MACRO NUM_SPOT_LIGHT!",_lightCount <= NUM_SPOT_LIGHTS));
	
	glUniform1i(spotLightsCountLoc, _lightCount);
	for (int i = 0; i < _lightCount; i++)
	{
		_spotLights[i].UseLight(
			this->spotLights[i].basePointLight.baseLight.uniformAmbientIntensity, 
			this->spotLights[i].basePointLight.baseLight.uniformColor, 
			this->spotLights[i].basePointLight.baseLight.uniformDiffuseIntensity, 
			this->spotLights[i].basePointLight.uniformPosition, 
			this->spotLights[i].basePointLight.uniformConstant, 
			this->spotLights[i].basePointLight.uniformLinear, 
			this->spotLights[i].basePointLight.uniformExponent,  
			this->spotLights[i].uniformSpotDirection,
			this->spotLights[i].uniformEdge,
			this->spotLights[i].uniformOuterEdge);
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

	///-----Set up Lights components' uniform locations ----///
	//pointlights
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{

		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		std::string locStr = locBuff;
		pointLights[i].baseLight.uniformColor = glGetUniformLocation(shaderProgram, locStr.c_str());
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		pointLights[i].baseLight.uniformAmbientIntensity = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		pointLights[i].baseLight.uniformDiffuseIntensity = glGetUniformLocation(shaderProgram, locBuff);
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
	pointLightsCountLoc = glGetUniformLocation(shaderProgram, "pointLightCount");

	//spotlight
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{

		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.base.color", i);
		std::string locStr = locBuff;
		spotLights[i].basePointLight.baseLight.uniformColor = glGetUniformLocation(shaderProgram, locStr.c_str());
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.base.ambientIntensity", i);
		spotLights[i].basePointLight.baseLight.uniformAmbientIntensity = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.base.diffuseIntensity", i);
		spotLights[i].basePointLight.baseLight.uniformDiffuseIntensity = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.position", i);
		spotLights[i].basePointLight.uniformPosition = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.constant", i);
		locStr = locBuff;
		spotLights[i].basePointLight.uniformConstant = glGetUniformLocation(shaderProgram, locStr.c_str());
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.linear", i);
		locStr = locBuff;
		spotLights[i].basePointLight.uniformLinear = glGetUniformLocation(shaderProgram, locStr.c_str());
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.exponent", i);
		locStr = locBuff;
		spotLights[i].basePointLight.uniformExponent = glGetUniformLocation(shaderProgram, locStr.c_str());
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		spotLights[i].uniformSpotDirection = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		spotLights[i].uniformEdge = glGetUniformLocation(shaderProgram, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].outerEdge", i);
		spotLights[i].uniformOuterEdge = glGetUniformLocation(shaderProgram, locBuff);
	}
	spotLightsCountLoc = glGetUniformLocation(shaderProgram, "spotLightCount");
	///---- end of setting up lights components' uniform locations ----///
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

GLuint* Shader::GetSpotLightCountLocation() {
	return &spotLightsCountLoc;
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