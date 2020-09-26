#version 450
#extension GL_ARB_separate_shader_objects : enable

// Have to specify our own output variable for each framebuffer,
// where the layout(location = 0) modifier specifies the index of the framebuffer.
layout(location = 0) out vec4 outColor;

// Input variable does not necessarily have to use the same name,
// they will be linked together using the indexes specified by the location directives.
layout(location = 0) in vec3 fragColor;

// Called for every fragment.
void main() {
    // The values for fragColor will be automatically interpolated
    // for the fragments between the three vertices, resulting in a smooth gradient.
    outColor = vec4(fragColor, 1.0);
}