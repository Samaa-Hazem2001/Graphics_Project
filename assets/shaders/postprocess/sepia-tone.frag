#version 330

// The texture holding the rendered scene
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

void main() {
    // Sample the color from the texture
    vec4 color = texture(tex, tex_coord);

    // vec3 sepiaColor = vec3(0.393, 0.769, 0.189) * color.rgb;
    // sepiaColor += vec3(0.349, 0.686, 0.168) * color.rgb;
    // sepiaColor += vec3(0.272, 0.534, 0.131) * color.rgb;

    // Apply the sepia tone effect
    const mat3 sepiaMatrix = mat3(
        0.393, 0.769, 0.189,
        0.349, 0.686, 0.168,
        0.272, 0.534, 0.131
    );
    vec3 sepiaColor = sepiaMatrix * color.rgb;

    frag_color = vec4(sepiaColor, color.a);
}