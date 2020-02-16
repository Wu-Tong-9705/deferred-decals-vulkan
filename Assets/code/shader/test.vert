#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform MVP 
{
    mat4 model;
    mat4 view;
    mat4 proj;
} mvp;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

void main() 
{
    gl_Position = mvp.proj * mvp.view * mvp.model * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}