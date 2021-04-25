#version 450
#define BIND_ID_PROJECTION_MATRIX  0

// tọa độ 3 đỉnh tam giác ABC
const vec2 positions[3] = vec2[](
    vec2(-0.5f, -0.5f),
    vec2( 0.5f, -0.5f),
    vec2( 0.0f,  0.5f)
);

// màu sắc 3 đỉnh tam giác ABC
const vec3 colorsRGB[3] = vec3[](
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f)
);

layout(location=0) out vec3 fragColor;

layout(binding = BIND_ID_PROJECTION_MATRIX) uniform UB {
    mat4 projection;
};

void main()
{
    gl_Position = projection * vec4(positions[gl_VertexIndex], 0.0f, 1.0f);
    fragColor = colorsRGB[gl_VertexIndex];
}
