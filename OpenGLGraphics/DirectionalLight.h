#pragma once
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
class DirectionalLight :
    public Light
{
    public:
		DirectionalLight() : Light() { direction = glm::vec3(0.0f, 0.0f, 0.0f); };
		DirectionalLight(Light light, glm::vec3 dir) : Light(light) { direction = dir; };
		DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat x, GLfloat y, GLfloat z, GLfloat diffuseIntensity, glm::vec3 dir) : Light(red, green, blue, aIntensity, diffuseIntensity)
		{
			direction = dir;
		};
		void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrengthLocation, GLuint directionLocation) {
			__super::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseStrengthLocation);
			glUniform3f(directionLocation, direction.x, direction.y, direction.z);
		};
		ShadowMap* GetShadowMap() { return shadowMap.get(); }
		bool CreateShadowMap(GLuint width, GLuint height, GLfloat nearPlane, GLfloat farPlane) override;
		void WriteShadowMap() { shadowMap->WriteShadowMap(); }
		void FinishShadowMap(){ shadowMap->FinishWriteShadowMap(); }
		const glm::mat4 *GetLightTransform() { return &lightTransform; }
	private:
		glm::vec3 direction;
		glm::mat4 lightTransform;
};

