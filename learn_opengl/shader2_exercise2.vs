#version 330 core
layout (location = 0 ) in vec3 aPos; //����vec3�������������ΪaPos
layout (location = 1 ) in vec3 aColor; 
out vec3 ourColor;

uniform float horizontalOffset;

void main()
{
	gl_Position = vec4(aPos.x + horizontalOffset * 0.5, aPos.y, aPos.z, 1.0);//����vertex shader��ouput��gl_Position������һ��vec4
    ourColor = aColor;
};