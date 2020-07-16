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

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light[3];

vec3 calculateLight(Light light, vec3 viewDir){
    // ambient
    vec3 ambient = light.ambient * material.diffuse;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, norm), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    return ambient + diffuse + specular;
}

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    if(!inv_draw && dot(Normal, viewDir) < 0)
        discard;

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i=0; i<3; ++i)
        result += calculateLight(light[i], viewDir);
    color = vec4(result, 1.0f);
}
