#version 330
#define NUM_POINT_LIGHTS 5
#define NUM_SPOT_LIGHTS 5
in vec4 vCol;
in vec2 vTex;
in vec3 Normal;
in vec3 fragPos;
in vec4 lightSpaceFragPos;
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

struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float edge;
    float outerEdge;
};

struct Material
{
	float shininess;
    float specularIntensity;
};

//light conponents
//assume the directionaLight is incident direction: from the light source to the fragments
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];
uniform int pointLightCount;
uniform int spotLightCount;

//material and texture components
uniform Material material;

//texture samplers
uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular0;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

//shadow map sampler
uniform sampler2D shadowMap;

uniform vec3 cameraPosition;

float CalculateShadowFactor()
{
    vec3 projCoords = lightSpaceFragPos.xyz/lightSpaceFragPos.w;
    //if the sqrtlength of projCoords is close to 0, it is outside the light frustum
	projCoords = projCoords * 0.5 + 0.5;
    //get texture size
    vec2 texelSize = 1.0f/textureSize(shadowMap, 0);
	float currentDepth = projCoords.z;
	float shadow = 0.0;
	//float bias = 0.005;
	float bias = max(0.01 * (1.0 - dot(normalize(Normal), normalize(-directionalLight.direction))), 0.005);
    for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			vec2 offset = vec2(x, y) * texelSize;
			float depth = texture(shadowMap, projCoords.xy + offset).r;
			shadow += currentDepth - bias > depth ? 0.0 : 1.0;
		}
	}
    //shadow factor that represents the how much light is blocked by the shadow
	shadow = shadow/9.0f;
	return shadow;
}

/// <summary>
/// Calculate the light by direction, direction is the reverse direction, from the fragment to the light source
/// </summary>
vec4 CalLightByDirection(BaseLight light, vec3 direction, float shadowFactor)
{
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)),0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    vec3 viewDirection = normalize(cameraPosition - fragPos);

    //reflect use the incident vector
    float specularFac = max(dot(reflect(-direction, normalize(Normal)), viewDirection), 0.0f);
    specularFac = pow(specularFac, material.shininess);
    vec4 specularColor  = vec4(light.color, 1.0f) * specularFac * material.specularIntensity;
    return texture(texture_diffuse0, vTex) * (ambientColor + shadowFactor*(diffuseColor + specularColor));
}

vec4 CalDirectionalLight()
{
    float directionalShadow = CalculateShadowFactor();
    //assume the directionLight is incident direction, thus need reverse the direction
	return CalLightByDirection(directionalLight.base, -directionalLight.direction, directionalShadow);
}
vec4 CalPointLight(PointLight pointLight)
{
    vec3 direction = fragPos - pointLight.position; //from light source to the fragment
    float distance = length(direction);
    direction = normalize(direction);
    float attenuation = pointLight.constant + pointLight.linear * distance + pointLight.exponent * distance * distance;
    //reverse the light direction, now from the fragment to the light source
    return vec4(CalLightByDirection(pointLight.base,-direction, 1.0f).xyz/attenuation, 1.0f); //TODO: remove it
}

//compute single point light color
vec4 CalTotalPointLight()
{
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    for(int i=0; i < pointLightCount; i++)
    {
        result.xyz += CalPointLight(pointLights[i]).xyz;
    }
	return result;
}

//write a spot light function
vec4 CalTotalSpotLight()
{
	vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i=0; i < spotLightCount; i++)
	{
		vec3 direction = fragPos - spotLights[i].pointLight.position;
		direction = normalize(direction); // from light source to the fragment
		float spotFactor = dot(direction, normalize(spotLights[i].direction));
		if(spotFactor > spotLights[i].outerEdge) //cosine value: bigger value means smaller than the edge angle, spotlight direction is factor 1.0f
		{
			vec4 lightColor = CalPointLight(spotLights[i].pointLight);
            //softedge
            float softEdgeFactor = clamp((spotFactor - spotLights[i].outerEdge)/(spotLights[i].edge - spotLights[i].outerEdge), 0.0f, 1.0f);
			result+= lightColor*softEdgeFactor;
		}
	}
	return result;
}

void main()                  
{   
    vec4 directionColor = CalDirectionalLight();
    vec4 pointColor = CalTotalPointLight();
    vec4 spotLight = CalTotalSpotLight();
    colour = texture(texture_diffuse0, vTex) * (directionColor);
}