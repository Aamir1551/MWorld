#version 330 core
layout(location=0)in vec3 pos;
//layout(location=1)in float yPos;
//layout(location=2)in vec3 color;
out vec3 cc;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    //gl_Position=vec4(pos,1);
    gl_Position=proj*view*model*vec4(pos,1);
    cc=pos;
    //gl_Position=proj*view*model*vec4(xPos,yPos,0,1);
    //gl_Position=transform*vec4(xPos,yPos,0,1);
    //cc=color;
};