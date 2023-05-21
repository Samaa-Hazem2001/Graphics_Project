#version 330

uniform vec3 eye;//eye
uniform mat4 VP;//view * position matrix
uniform mat4 M; // model matrix
uniform mat4 M_IT;//model matrix  inverse transpose

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
layout(location=2) in vec2 tex_coord;
// Now we need to the surface normal to compute the light so we will send it as an attribute.
layout(location=3) in vec3 normal;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vs_out;

void main(){
    // First we compute the world position.
    vec3 world = (M * vec4(position, 1.0)).xyz;
    gl_Position = VP * vec4(world, 1.0);
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    // Then we compute normal in the world space (Note that w=0 since this is a vector).
    vs_out.normal = normalize((M_IT * vec4(normal, 0.0)).xyz);
    // Then we compute the view vector (vertex to eye vector in the world space).
    vs_out.view = eye - world;
     // Finally, we compute the position in the homogenous clip space and send the rest of the data.
    vs_out.world = world;
}