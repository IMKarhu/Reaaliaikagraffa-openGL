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
    //ambient
    vec3 lightColor = vec3(1.0);\n
    vec4 objectColor = texture2D(texture0, texCoord);\n
    float ambientStrength = 0.1;\n
    vec3 ambient = ambientStrength * lightColor;\n
    //diffuse
    vec3 lightDir = normalize(lightPos - fragPos);\n
    vec3 norm = normalize(normal);\n
    float diff = max(dot(norm, lightDir), 0.0);\n
    vec3 diffuse = diff * lightColor;\n
    //specular
    float specularStrength = 0.8;\n
    vec3 viewDir = normalize(viewPos - fragPos);\n
    vec3 reflectDir = reflect(-lightDir, norm);\n
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);\n
    vec3 specular = specularStrength * spec * lightColor;\n

    vec3 result = (ambient + diffuse) * objectColor.xyz + specular;\n
   FragColor = vec4(result, objectColor.w);\n
};