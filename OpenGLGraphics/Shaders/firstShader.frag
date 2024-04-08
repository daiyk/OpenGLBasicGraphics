#version 330
#define NUM_POINT_LIGHTS 5
in vec4 vCol;
in vec2 vTex;
in vec3 Normal;
in vec3 fragPos;
out vec4 colour;


struct BaseLight
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct PointLight
{
    BaseLight base;
	vec3 position;
	float constant, linear, exponent;
};

struct Material
{
	float shininess;
    float specularIntensity;
};

//assume the directionaLight is incident direction: from the light source to the fragments
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform Material material;
uniform sampler2D tex;
uniform int pointLightCount;
uniform vec3 cameraPosition;

/// <summary>
/// Calculate the light by direction, direction is the reverse direction, from the fragment to the light source
/// </summary>
vec4 CalLightByDirection(BaseLight light, vec3 direction)
{
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)),0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    vec3 viewDirection = normalize(cameraPosition - fragPos);

    //reflect use the incident vector
    float specularFac = max(dot(reflect(-direction, normalize(Normal)), viewDirection), 0.0f);
    specularFac = pow(specularFac, material.shininess);
    vec4 specularColor  = vec4(light.color, 1.0f) * specularFac * material.specularIntensity;
    return texture(tex, vTex) * (ambientColor + diffuseColor + specularColor);
}

vec4 CalDirectionalLight()
{
    //assume the directionLight is incident direction, thus need reverse the direction
	return CalLightByDirection(directionalLight.base, -directionalLight.direction);
}

vec4 CalPointLight()
{
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    for(int i=0; i < pointLightCount; i++)
    {
        
        vec3 direction = fragPos - pointLights[i].position;
        float distance = length(direction);
        direction = normalize(direction);
        float attenuation = pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].exponent * distance * distance;
        result += vec4(CalLightByDirection(pointLights[i].base,-direction).xyz/attenuation, 1.0f); //CalLightByDirection(pointLights[i].base, direction).xyz/attenuation;
        
    }
	return result;
}

void main()                  
{   
    vec4 directionColor = CalDirectionalLight();
    vec4 pointColor = CalPointLight();
    //if(pointLightCount > 0)
		//pointColor = vec4(1.0f,1.0f,1.0f, 1.0f);
    colour = texture(tex, vTex) * (pointColor+directionColor);
}