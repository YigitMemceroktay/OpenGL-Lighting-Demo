#version 330

in vec3 Normal;
in vec4 FragPos;
in vec3 light_pos;

out vec4 Color;

uniform vec4 color;
uniform vec3 viewPos;
uniform float specPow;

uniform float specularStrength = 0.5;
uniform float ambientStrength = 0.5f;

vec3 object_color = vec3(color);
uniform vec3 ambient_color = vec3(0.5,0.5,0.5);
uniform vec3 diffuse_color = vec3(0.5f,0.5f,0.5f);
uniform vec3 spec_color = vec3(0.7f,0.7,0.7);

vec3 fragpos = vec3(FragPos);
void main()
{
	
	vec3 ambient = ambientStrength*ambient_color;
	
	vec3 lightdir = normalize(light_pos - fragpos);
	float diff = max(dot(normalize(Normal),lightdir),0.0);
	vec3 diffuse = diff * diffuse_color;
	
	vec3 viewdir = normalize(viewPos - fragpos);
	vec3 reflectdir = reflect(-lightdir,Normal);
	
	float spec = pow(max(dot(reflectdir,viewdir),0.0),specPow);
	vec3 specular = specularStrength*spec*spec_color;

	vec3 result = object_color*(ambient+ diffuse+specular);
	
	
	Color = vec4(result,1.0f);
}