#version 450
#define BIND_ID_PROJECTION_MATRIX   0
#define BIND_ID_UBO2                1

#define ATTR_POSITION       0
#define ATTR_COLOR          1
#define ATTR_TEXTURE_COORD  2


layout(location = ATTR_POSITION) in vec2 vaPosition;
layout(location = ATTR_COLOR) in vec3 vaColor;
layout(location = ATTR_TEXTURE_COORD) in vec2 vaTextureCoord;

layout(location=0) out vec3 fragColor;
layout(location=1) out vec2 fragTextureCoord;

layout(binding = BIND_ID_PROJECTION_MATRIX) uniform UB {
    mat4 projection;
};

layout(binding = BIND_ID_UBO2) uniform UB2 {
    mat4 view;
    mat4 world;
};

void main()
{
    gl_Position = projection * view * world * vec4(vaPosition, 0.0f, 1.0f);
    
    fragColor = vaColor;
    fragTextureCoord = vaTextureCoord;
}
