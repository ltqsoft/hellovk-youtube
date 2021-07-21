#version 450
#define BIND_ID_TEXTURE 2
#define SO_LUONG_INSTANCE   4


const float mixFacs[SO_LUONG_INSTANCE] = float[](
    0.2f, 0.4f, 0.6f, 0.8f
);

layout(location=0) in vec3 fragColor;
layout(location=1) in vec2 fragTextureCoord;
layout(location=2) in vec3 fragNormal;
layout(location=3) in flat uint fragInstanceIndex;

layout(location=0) out vec4 color;

layout(binding = BIND_ID_TEXTURE) uniform sampler2D textureSampler;

layout(push_constant) uniform PUSH {
    vec3 lightColor;
};


void basic()
{
    vec3 sampledColor = texture(textureSampler, fragTextureCoord).rgb;
    float mixFac = mixFacs[fragInstanceIndex];
    vec3 mixedColor = mix(lightColor, fragColor, mixFac);
    mixedColor = mix(mixedColor, sampledColor, 1.0f - mixFac);
    color = vec4(mixedColor, 1.0f);
}


vec4 visualize_normal()
{
    vec3 norm = fragNormal;
    if( norm.x < 0 && norm.y < 0 ) norm = vec3(1.0f);
    return vec4(norm, 1.0f);
}


void main()
{
    color = visualize_normal();
}
