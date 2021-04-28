#version 450
#define BIND_ID_PROJECTION_MATRIX   0
#define BIND_ID_UBO2                1

const vec2 A = vec2(-0.5f, -0.5f);
const vec2 B = vec2( 0.5f, -0.5f);
const vec2 C = vec2( 0.5f,  0.5f);
const vec2 D = vec2(-0.5f, 0.5f);

// Hình vuông ABCD, tạo bởi 2 tam giác ABC và ACD
const vec2 positions[6] = vec2[](
    A,B,C,
    A,C,D
);

const vec3 mau_do               = vec3(1.0f, 0.0f, 0.0f);
const vec3 mau_tim              = vec3(1.0f, 0.0f, 1.0f);
const vec3 mau_xanh_duong       = vec3(0.0f, 0.0f, 1.0f);
const vec3 mau_xanh_la          = vec3(0.0f, 1.0f, 0.0f);
const vec3 mau_vang             = vec3(1.0f, 1.0f, 0.0f);

// màu sắc 6 đỉnh tương ứng với 2 tam giác
const vec3 colorsRGB[6] = vec3[](
    mau_do, mau_xanh_duong, mau_xanh_la,
    mau_do, mau_xanh_la, mau_vang
);

layout(location=0) out vec3 fragColor;

layout(binding = BIND_ID_PROJECTION_MATRIX) uniform UB {
    mat4 projection;
};

layout(binding = BIND_ID_UBO2) uniform UB2 {
    mat4 view;
    mat4 world;
};

void main()
{
    gl_Position = projection * view * world * vec4(positions[gl_VertexIndex], 0.0f, 1.0f);
    fragColor = colorsRGB[gl_VertexIndex];
}
