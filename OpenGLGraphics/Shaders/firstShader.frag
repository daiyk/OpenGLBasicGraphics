#version 330
#define NUM_POINT_LIGHTS 5
#define NUM_SPOT_LIGHTS 5
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
uniform sampler2D tex;

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
vec4 CalPointLight(PointLight pointLight)
{
    vec3 direction = fragPos - pointLight.position; //from light source to the fragment
    float distance = length(direction);
    direction = normalize(direction);
    float attenuation = pointLight.constant + pointLight.linear * distance + pointLight.exponent * distance * distance;
    //reverse the light direction, now from the fragment to the light source
    return vec4(CalLightByDirection(pointLight.base,-direction).xyz/attenuation, 1.0f);
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
    colour = texture(tex, vTex) * (spotLight);
}