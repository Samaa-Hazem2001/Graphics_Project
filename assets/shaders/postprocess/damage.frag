#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// How far (in the texture space) is the distance (on the x-axis) between
// the pixels from which the red/green (or green/blue) channels are sampled
#define STRENGTH .005

void main(){
  // samples the red channel (r) from a pixel located to the left of the current tex_coord by an amount defined by STRENGTH.
  float r=texture(tex,tex_coord+vec2(-STRENGTH,0)).r;
  // samples the green channel (g) from the current tex_coord.
  float g=texture(tex,tex_coord).g;
  // samples the blue channel (b) from a pixel located to the right of the current tex_coord by an amount defined by STRENGTH.
  float b=texture(tex,tex_coord+vec2(STRENGTH,0)).b;
  // sets the final color of the fragment. It adds 0.2 to the red channel, subtracts 0.1 from the green and blue channels, and sets the alpha channel to 1.0.
  frag_color=vec4(r+.2,g-.1,b-.1,1.);
  
}