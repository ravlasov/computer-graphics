#version 330 core
out vec4 color;

struct Material{
    vec3 diffuse;
};

uniform Material material;

void main()
{
    color = vec4(material.diffuse, 1.0f);
}
