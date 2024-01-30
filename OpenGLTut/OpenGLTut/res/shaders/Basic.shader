#shader vertex
#version 330 core 

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTextureCord;

out vec2 textureCord; 

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
   gl_Position = u_projection * u_view * u_model * aPosition; 
   textureCord = aTextureCord;
};

#shader fragment
#version 330 core 

layout(location = 0) out vec4 color; 

in vec2 textureCord;

uniform vec4 u_Color;
uniform sampler2D customTexture;

void main(){
   color = texture(customTexture, textureCord);
};