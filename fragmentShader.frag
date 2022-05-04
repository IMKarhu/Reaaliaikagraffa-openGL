#version 330 core
uniform sampler2D texture0;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;
out vec4 FragColor;

void main()
    {
    vec3 lightColor = vec3(1.0);
    vec4 objectColor = texture2D(texture0, texCoord);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;
    vec3 result = (ambient + diffuse) * objectColor.xyz + specular;
    FragColor = vec4(result, objectColor.w);
};