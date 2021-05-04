#version 450
#define BIND_ID_TEXTURE 2

layout(location=0) in vec3 fragColor;
layout(location=1) in vec2 fragTextureCoord;

layout(location=0) out vec4 color;

layout(binding = BIND_ID_TEXTURE) uniform sampler2D textureSampler;

layout(push_constant) uniform PUSH {
    vec3 lightColor;
};

void main()
{
    vec3 sampledColor = texture(textureSampler, fragTextureCoord).rgb;
    float mixFac = 0.8f;
    vec3 mixedColor = mix(lightColor, fragColor, mixFac);
    mixedColor = mix(mixedColor, sampledColor, 1.0f - mixFac);
    color = vec4(mixedColor, 1.0f);
}
