#version 330 core
out vec4 FragColors;// fragment shader requires a vec4 output\n"
in vec3 cc;
void main()
{
    //FragColors=vec4(1.f,.5f,.2f,1.f);
    FragColors=vec4(cc,1);
};