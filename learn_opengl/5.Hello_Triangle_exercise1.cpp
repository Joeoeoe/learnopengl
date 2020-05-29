#include <glad\glad.h>
#include <GLFW\glfw3.h>

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


	/*
	 *主要内容
	 */
	float vertices[] = {
		0.0f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,

		0.0f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	const char *vertexShaderSource =
		"# version 330 core \n"
		"layout (location = 0) in vec3 aPos; \n"
		"void main() \n"
		"{\n"
		"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	const char *fragmentShaderSource =
		"#version 330 core \n"
		"out vec4 FragColor; \n"
		"void main() \n"
		"{\n"
		"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
		"}\0";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);





	//使得窗口不会马上关闭
	while (!glfwWindowShouldClose(window))//检测此窗口是否被要求关闭
	{
		preocessInput(window);//处理输入


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//放置渲染的指令代码
		//...
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);




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