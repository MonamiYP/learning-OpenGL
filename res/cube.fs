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

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform vec3 u_viewPos;

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NO_POINT_LIGHTS 4
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NO_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 GetDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDirection = normalize(-light.direction);
    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    // Specular
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture(material.diffuse, textureCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, textureCoords).rgb;

    return (ambient + diffuse + specular);
}

vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir) {
    vec3 lightDirection = normalize(-light.position);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDir = reflect(-lightDirection, normal);;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * texture(material.diffuse, textureCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, textureCoords).rgb;

    return (ambient + diffuse + specular) * attenuation;
}

vec3 GetSpotLight(SpotLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir) {
    vec3 lightDirection = normalize(light.position - fragmentPos);

    // Spotlight intensity
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Diffuse, specular
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 reflectDir = reflect(-lightDirection, normal);;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenutation
    float distance = length(light.position - fragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * texture(material.diffuse, textureCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, textureCoords).rgb;

    return (ambient + diffuse + specular) * intensity * attenuation;
}

void main() {
    vec3 normal = normalize(normalVector);
    vec3 viewDir = normalize(u_viewPos - fragmentPos);

    vec3 result = GetDirLight(dirLight, normal, viewDir);

    for (int i = 0; i < NO_POINT_LIGHTS; i++) {
        result += GetPointLight(pointLights[i], normal, fragmentPos, viewDir);
    }

    result += GetSpotLight(spotLight, normal, fragmentPos, viewDir);

    fragmentColour = vec4(result, 1.0);
}