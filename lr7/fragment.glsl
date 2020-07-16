#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform bool inv_draw;
uniform vec3 viewPos;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct DirectedLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectedLight directed_light;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light[3];

struct ProjectorLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform ProjectorLight projector_light;

vec3 calculateDirectedLight(DirectedLight light, vec3 viewDir){
    // ambient
    vec3 ambient = light.ambient * material.diffuse;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, norm), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    return ambient + diffuse + specular;
}

vec3 calculatePointLight(Light light, vec3 viewDir){
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                    light.quadratic * (distance * distance));
    // ambient
    vec3 ambient = light.ambient * material.diffuse;
    ambient *= attenuation;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    diffuse *= attenuation;

    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, norm), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    specular *= attenuation;
    return ambient + diffuse + specular;
}

vec3 calculateProjectorLight(ProjectorLight light, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 

    // ambient
    vec3 ambient = light.ambient * material.diffuse;
    if(theta < light.outerCutOff){
        return ambient;
    }

    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    diffuse *= intensity;

    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, norm), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    specular *= intensity;

    return ambient + diffuse + specular;
}


void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    if(!inv_draw && dot(Normal, viewDir) < 0)
        discard;

    vec3 result = calculateDirectedLight(directed_light, viewDir);
    result += calculateProjectorLight(projector_light, viewDir);
    for(int i=0; i<3; ++i)
        result += calculatePointLight(light[i], viewDir);
    color = vec4(result, 1.0f);
}
