#version 430 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;
layout (location = 10) in uint draw_id;

out vec4 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(std430, binding = 0) buffer ssboData
{
    mat4 instance_data[1000000];
};

void main()
{
    //gl_Position = projection * view * instance_data[draw_id] * vec4(position+gl_InstanceID, 1.0f);
	gl_Position = projection * view * model * vec4(position, 1.0f);
}

