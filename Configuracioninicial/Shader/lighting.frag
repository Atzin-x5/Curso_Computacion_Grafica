#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// === NUEVA ESTRUCTURA: ahora soporta arreglo de luces ===
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 color;

uniform vec3 viewPos;
uniform Material material;

// === NUEVA DECLARACIÓN: arreglo de 2 luces en lugar de una sola ===
uniform Light lights[2];

uniform sampler2D texture_diffuse;

// === NUEVA FUNCIÓN: calcula la contribución de una luz individual ===
vec3 CalcLight(Light light, vec3 norm, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * material.diffuse;

    // Diffuse
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // === NUEVO: suma la contribución de ambas luces ===
    vec3 result = CalcLight(lights[0], norm, viewDir)
                + CalcLight(lights[1], norm, viewDir);

    color = vec4(result, 1.0f) * texture(texture_diffuse, TexCoords);
}