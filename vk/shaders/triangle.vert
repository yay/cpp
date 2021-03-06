#version 450

// Hard-coded array of vertex data.
vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

// Hard-coded array of color data.
vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout(location = 0) out vec3 fragColor;

// Invoked for every vertex.
void main() {
    // gl_Position is the built-in output variable.
    // gl_VertexIndex is usually an index into the vertex buffer,
    // but in our case it is an index into a hard-coded array of vertex data.
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}