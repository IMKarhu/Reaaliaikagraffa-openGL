#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;
layout (location = 2) in vec3 in_normal;
uniform mat4 MVP;
uniform mat4 M;
out vec3 normal;
out vec2 texCoord;
out vec3 fragPos;
out vec3 viewPos;
void main()
{
	texCoord = in_texCoord;
	gl_Position = MVP * vec4(in_position.x, in_position.y, in_position.z, 1.0);
	fragPos = vec3(M * vec4(in_position, 1.0));
	normal = mat3(transpose(inverse(M))) * in_normal;
};