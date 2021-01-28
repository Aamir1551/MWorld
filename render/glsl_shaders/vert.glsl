#version 330 core
layout(location=0)in vec3 pos;
//layout(location=1)in vec3 color;
out vec3 cc;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position=proj*view*model*vec4(pos,1);
    //cc=pos; -- uncomment this line, to get the multi colour again
    cc=vec3(1, 1, 1);
};