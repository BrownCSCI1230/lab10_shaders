#version 330 core

in vec4 world_pos;
in vec4 world_normal;
in vec4 camera_pos;
out vec4 fragColor;

struct Light {
    vec4 position;
    vec3 color;
};

uniform Light light;

void main() {

   float diffuse  = clamp(dot(normalize(vec3(world_normal)),
                          normalize(vec3(light.position) - vec3(world_pos))),
                          0,
                          1);

   vec3 toCamera = normalize(vec3(camera_pos) - vec3(world_pos));
   vec3 reflectedLight = normalize(reflect(vec3(light.position - world_pos),
                                           vec3(world_normal)));

   float ambient  = 0.1;
   float specular = pow(clamp(dot(-toCamera, reflectedLight),0,1),15);

   fragColor = vec4(vec3(0.8 * diffuse + ambient + specular),1);
}
