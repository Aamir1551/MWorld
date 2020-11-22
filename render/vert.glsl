#version 330 core
layout(location=0)in float xPos;
layout(location=1)in float yPos;
layout(location=2)in vec3 color;
out vec3 cc;
void main()
{
    gl_Position=vec4(xPos,yPos,0,1);
    cc=color;
};