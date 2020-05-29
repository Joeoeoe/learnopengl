#version 330 core
out vec4 FragColor;//定义变量名为FragColor
in vec3 ourPos;
void main()
{
    FragColor = vec4(ourPos, 1.0f);
};