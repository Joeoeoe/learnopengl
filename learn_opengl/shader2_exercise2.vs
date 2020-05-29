#version 330 core
layout (location = 0 ) in vec3 aPos; //设置vec3输入变量，名命为aPos
layout (location = 1 ) in vec3 aColor; 
out vec3 ourColor;

uniform float horizontalOffset;

void main()
{
	gl_Position = vec4(aPos.x + horizontalOffset * 0.5, aPos.y, aPos.z, 1.0);//设置vertex shader的ouput。gl_Position变量是一个vec4
    ourColor = aColor;
};