#version 330 core

in vec3 normalVector;
in vec3 fragmentPos;
in vec2 textureCoords;

out vec4 fragmentColour;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 u_viewPos;

void main() {
    vec3 lightDirection = normalize(light.position - fragmentPos);

    // Spotlight
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Ambient light
    vec3 ambient = light.ambient * texture(material.diffuse, textureCoords).rgb;

    // Diffuse light
        vec3 normal = normalize(normalVector);
        float diff = max(dot(normal, lightDirection), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureCoords).rgb;

    // Specular light
    vec3 viewDir = normalize(u_viewPos - fragmentPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, textureCoords).rgb;

    // Attenuation
    float distance = length(light.position - fragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    fragmentColour = vec4(ambient + (diffuse + specular) * attenuation * intensity, 1.0);
}