#version 330

#define MAX_LIGHTS 64

// set DIRECTIONAL to 0
#define DIRECTIONAL 0

// set POINT to 1
#define POINT 1

// set SPOT to 2
#define SPOT 2

struct Light {
    //type of light spot , directional ,point
    int type;
    // the position of the light
    vec3 position;
    //the direction of the light
    vec3 direction;
    // These defines the colors and intensities of the light.
    vec3 diffuse;
    vec3 specular; 
    vec3 attenuation; // x*d^2 + y*d + z
    vec2 coneAngles; // x: inner angle, y: outer angle for spot light
};

uniform Light lights[MAX_LIGHTS];
uniform int light_count;
//struct for sky light
struct Sky {
    vec3 top, middle, bottom;
};
//sky light from forward render
uniform Sky sky;

// struct for material 
// albedo: which is used to represent the diffuse of the material.
// specular: which is used to represent the specular of the material.
// ambient_occlusion: which is used to represent how much ambient each part should get, not all locations get the same ambient.
// roughness: which is used to represent the shininess of the material.
// emissive: which is used to make the object emit its own light
struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D ambient_occlusion;
    sampler2D roughness;
    sampler2D emissive;
};
// Receive the material as uniform.
uniform Material material;

in Varyings {
    vec4 color;
    vec2 tex_coord;
     // We will need the surface normal in the world space.
    vec3 normal;
    // We will need the view vector (vertex to eye vector in the world space),
    vec3 view;
     // We will need the vertex position in the world space,
    vec3 world;
} fs_in;

out vec4 frag_color;

void main(){
    // First we normalize the normal and the view.
    vec3 view = normalize(fs_in.view);
    vec3 normal = normalize(fs_in.normal);
   // get the material components
    vec3 material_diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 material_specular = texture(material.specular, fs_in.tex_coord).rgb;
    vec3 material_ambient = material_diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    
    float material_roughness = texture(material.roughness, fs_in.tex_coord).r;
    float material_shininess = 2.0 / pow(clamp(material_roughness, 0.001, 0.999), 4.0) - 2.0;

    vec3 material_emissive = texture(material.emissive, fs_in.tex_coord).rgb;
    //sky light 
    vec3 sky_light = (normal.y > 0) ?
        mix(sky.middle, sky.top, normal.y * normal.y) :
        mix(sky.middle, sky.bottom, normal.y * normal.y);

    frag_color = vec4(material_emissive + material_ambient * sky_light , 1.0);
     //get light counts
    int clamped_light_count = min(MAX_LIGHTS, light_count);
    for(int i = 0; i < clamped_light_count; i++){
        Light light = lights[i];
           // Then we get the light direction 
        vec3 direction_to_light = -light.direction;
        if(light.type != DIRECTIONAL){
            direction_to_light = normalize(light.position - fs_in.world);
        }

          // Now we compute the  components of the light separately.
        
        vec3 diffuse = light.diffuse * material_diffuse * max(0, dot(normal, direction_to_light));
        
        vec3 reflected = reflect(-direction_to_light, normal); // this is used for specular
        
        vec3 specular = light.specular * material_specular * pow(max(0, dot(view, reflected)), material_shininess);

        float attenuation = 1;
        if(light.type != DIRECTIONAL){
            //distance relative to the pixel location in the world space.
            float d = distance(light.position, fs_in.world);
            attenuation /= dot(light.attenuation, vec3(d*d, d, 1));
            if(light.type == SPOT){
                // Then we calculate the angle between the pixel and the cone axis.
                float angle = acos(dot(-direction_to_light, light.direction));
                 // And we calculate the attenuation based on the angle.
                attenuation *= smoothstep(light.coneAngles.y, light.coneAngles.x, angle);
            }
        }
         // Then we combine the light component .
        frag_color.rgb += (diffuse + specular) * attenuation;
    }
}