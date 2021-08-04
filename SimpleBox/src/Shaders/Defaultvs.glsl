#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightModel;

vec4 lightPos = vec4(0.0,0.0f,0.,1.);
out vec4 FragPos;
out vec3 Normal;
out vec3 light_pos;
void main()
{
	light_pos = vec3(lightModel * lightPos);
    Normal = aNormal;
	FragPos = model * vec4(aPos,1.0f);
	gl_Position = projection * view * model * vec4(aPos,1.0f);
}