#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Shader.h"

#include<iostream>
using namespace std;
void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void preocessInput(GLFWwindow *window);

int main() {


	glfwInit();//初始化GLFW

	//配置GLFW。参数1：想要配置的选项，参数2：选项值
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心模式

	//创建窗口
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();//完成，销毁所有的窗口、cursor

		return -1;
	}

	//创建上下文
	glfwMakeContextCurrent(window);


	//GLAD加载
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//监听窗口调整的回调函数
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	float vertices[] = {
		//position         //color 
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2//第一个三角形
	};

	Shader myShader("shader2_exercise3.vs", "shader2_exercise3.fs");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//使得窗口不会马上关闭
	while (!glfwWindowShouldClose(window))//检测此窗口是否被要求关闭
	{
		preocessInput(window);//处理输入


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//放置渲染的指令代码
		//...
		glBindVertexArray(VAO);
		float horizontalOffset = sin(glfwGetTime());
		cout << horizontalOffset << endl;
		myShader.setFloat("horizontalOffset", horizontalOffset);
		myShader.use();




		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);//交换color buffer
		glfwPollEvents();//监听是否有任何事件被触发，比如键盘输入，鼠标移动，更新窗口状态，并调用对应的函数
	}


	glfwTerminate();//销毁分配的内存空间



	return 0;
}
//回调函数
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}


void preocessInput(GLFWwindow *window) {
	/*
	 * 如果输入的是Esc键，则退出循环，关闭窗口;
	 * 如果不是的话，glfwGetKey会返回GLFW_RELEASE
	 */
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}