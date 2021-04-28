#version 450

layout(location=0) in vec3 fragColor;

layout(location=0) out vec4 color;

layout(push_constant) uniform PUSH {
    vec3 lightColor;
};

void main()
{
    float mixFac = 0.8f;
    vec3 mixedColor = mix(lightColor, fragColor, mixFac);
    color = vec4(mixedColor, 1.0f);
}
