#version 330 core

out vec4 frag_color;


// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    /*
    
    
    */

    vec2 Pos = floor(vec2(gl_FragCoord) / size);//get the position 
    float PatternMask = mod(Pos.x + mod(Pos.y, 2.0), 2.0);// get the pattern mask from the position 
    vec4 col1 = vec4(colors[1], 1.0);//first color
    vec4 col2 = vec4(colors[0], 1.0);//second color
    frag_color = (bool(PatternMask)) ? col1 : col2;//choose the current color depending on the pattern mask
}