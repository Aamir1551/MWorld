#version 330 core
out vec4 FragColors; //fragment shader requires a vec4 output\n"
in vec3 cc;
uniform vec3 ourColor;
void main()
{
    FragColors = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColors = vec4(ourColor*cc, 1);
    //FragColors = vec4(ourColor, 1);
    //FragColors = vec4(cc, 1);
};