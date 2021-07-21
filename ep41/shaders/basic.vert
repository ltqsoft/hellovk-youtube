#version 450
#define BIND_ID_PROJECTION_MATRIX   0
#define BIND_ID_UBO2                1
#define ATTR_POSITION       0
#define ATTR_COLOR          1
#define ATTR_TEXTURE_COORD  2
#define ATTR_NORMAL         3
#define SO_LUONG_INSTANCE 4


layout(location = ATTR_POSITION) in vec3 vaPosition;
layout(location = ATTR_COLOR) in vec3 vaColor;
layout(location = ATTR_NORMAL) in vec3 vaNormal;
layout(location = ATTR_TEXTURE_COORD) in vec2 vaTextureCoord;

layout(location=0) out vec3 fragColor;
layout(location=1) out vec2 fragTextureCoord;
layout(location=2) out vec3 fragNormal;
layout(location=3) out flat uint fragInstanceIndex;

layout(binding = BIND_ID_PROJECTION_MATRIX) uniform UB {
    mat4 projection;
};

layout(binding = BIND_ID_UBO2) uniform UB2 {
    mat4 view;
    mat4 worlds[SO_LUONG_INSTANCE];
};


void main()
{
    mat4 world = worlds[gl_InstanceIndex];
    gl_Position = projection * view * world * vec4(vaPosition, 1.0f);
    
    fragColor           = vaColor;
    fragTextureCoord    = vaTextureCoord;
    fragNormal          = normalize( mat3(world) * vaNormal );
    fragInstanceIndex   = gl_InstanceIndex;
}
