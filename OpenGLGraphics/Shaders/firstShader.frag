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
	float farPlane;
};

struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float edge;
    float outerEdge;
};

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material
{
	float shininess;
    float specularIntensity;
};

//sample offset directions
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

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

//includes shadow map for point light and spot light, notice that spotlights has a offset of NUM_POINT_LIGHTS
uniform OmniShadowMap omniShadowMap[NUM_POINT_LIGHTS+NUM_SPOT_LIGHTS];
uniform vec3 cameraPosition;

//forward declaration
float CalculateOmniShadowFactor(PointLight light, int index);

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
vec4 CalPointLight(PointLight pointLight, int shadowIndex)
{
    vec3 direction = fragPos - pointLight.position; //from light source to the fragment
    float distance = length(direction);
    direction = normalize(direction);
    float attenuation = pointLight.constant + pointLight.linear * distance + pointLight.exponent * distance * distance;
    //calculate the shadow factor
	float shadowFactor = 0.0f;
	if(shadowIndex < NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS)
	{
		shadowFactor = CalculateOmniShadowFactor(pointLight, shadowIndex);
	}
	//reverse the light direction, now from the fragment to the light source
    return vec4(CalLightByDirection(pointLight.base,-direction, shadowFactor).xyz/attenuation, 1.0f);
}

//compute single point light color
vec4 CalTotalPointLight()
{
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    for(int i=0; i < pointLightCount; i++)
    {
        result.xyz += CalPointLight(pointLights[i], i).xyz;
    }
	return result;
}

//write a spot light function
vec4 CalTotalSpotLight()
{
//TODO: add shadow map for spot lights
	vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i=0; i < spotLightCount; i++)
	{
		vec3 direction = fragPos - spotLights[i].pointLight.position;
		direction = normalize(direction); // from light source to the fragment
		float spotFactor = dot(direction, normalize(spotLights[i].direction));
		if(spotFactor > spotLights[i].outerEdge) //cosine value: bigger value means smaller than the edge angle, spotlight direction is factor 1.0f
		{
			vec4 lightColor = CalPointLight(spotLights[i].pointLight, i + NUM_POINT_LIGHTS);
            //softedge
            float softEdgeFactor = clamp((spotFactor - spotLights[i].outerEdge)/(spotLights[i].edge - spotLights[i].outerEdge), 0.0f, 1.0f);
			result+= lightColor*softEdgeFactor;
		}
	}
	return result;
}

float CalculateOmniShadowFactor(PointLight light, int index)
{
    //loop the offset directions and calculate the shadow factor
	int samples = 20;
	vec3 fragToLight = fragPos - light.position;
	float diskRadius = (1.0 + (length(fragToLight) / omniShadowMap[index].farPlane)) / 25.0;
	float currentDistance = length(fragToLight);
	float bias = max(0.01 * (1.0 - dot(normalize(Normal), normalize(-fragToLight))), 0.005);
	float shadowFactor = 0.0f;
	for(int i = 0; i < samples; i++)
	{
		float closest = texture(omniShadowMap[index].shadowMap, fragToLight + sampleOffsetDirections[i]*diskRadius).r;
		closest = closest * omniShadowMap[index].farPlane;
		
		//compare the distance between the fragment and the light source
		shadowFactor += currentDistance - bias > closest ? 0.0f : 1.0f;
	}
	return shadowFactor/float(samples);
}

void main()                  
{   
    vec4 directionColor = CalDirectionalLight();
    vec4 pointColor = CalTotalPointLight();
    vec4 spotLight = CalTotalSpotLight();
    colour = texture(texture_diffuse0, vTex) * (pointColor+spotLight);
}