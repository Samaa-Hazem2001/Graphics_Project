#version 330

// The texture holding the rendered scene
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// Duotone effect parameters
const vec3 tone1 = vec3(0.6627, 0.8078, 0.8824);   // First tone color
const vec3 tone2 = vec3(0.8, 0.2, 0.6);   // Second tone color

void main() {
    // Sample the color from the texture
    vec4 color = texture(tex, tex_coord);

    // Map the input colors to the two-tone color palette
    // The mix() function is used to interpolate between tone1 and tone2 based on the multiply value.
    vec3 duotoneColor = mix(tone1, tone2, color.rgb*vec3(0.3333));

    frag_color = vec4(duotoneColor, color.a);
}