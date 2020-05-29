#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include<iostream>
#include "Shader.h"

using namespace std;
void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void preocessInput(GLFWwindow *window);

int main() {
	/*
	 * 1. GLFW初始化
	 */

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



	/*
	 * 2. GLAD加载OpenGL函数指针的地址
	 */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	/*
	 * 3. Viewport
	 */
	glViewport(0, 0, 800, 600);

	//监听窗口调整的回调函数
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);



	/*
	 * 一、顶点数据
	 */
	 //float vertices[] = {
	 //	-0.5f, -0.5f, 0.0f,
	 //	0.5f, -0.5f, 0.0f,
	 //	0.0f, 0.5f, 0.0f
	 //};


	float vertices[] = {
		//position         //color 
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2,//第一个三角形
	};

	Shader* ourShader = new Shader("shader1.vs", "shader1.fs");

	//Shader ourShader("shader1.vs", "shader1.fs");

	/*
	 * 七、设置VAO
	 */
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	/*
	 * 二、VBO设置
	 */
	unsigned int VBO;
	glGenBuffers(1, &VBO);//生成buffer，设置VBO的buffer id
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//设置buffer类型，之后对GL_ARRAY_BUFFER的调用和配置也是指向VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//复制用户自定义的data到当前的buffer


	/*
	 * 九、Element Buffer Objects
	 * 要在VBO后设置！！！
	 */

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);





	/*
	 * 六、连接顶点属性
	 */

	 //告诉OpenGL如何把顶点数据和vertex shader连接起来
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	//告诉顶点属性对应vertex shader中的location
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	//解绑VAO
	glBindVertexArray(0);





	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//全模式


	/*
	 * 4. render loop
	 */

	 //使得窗口不会马上关闭
	while (!glfwWindowShouldClose(window))//检测此窗口是否被要求关闭
	{
		preocessInput(window);//处理输入

		/*
		 * 7. 设置clear color及clear
		 */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//放置渲染的指令代码
		//...
		/*
		 * 八、进行渲染
		 */
		//glUseProgram(shaderProgram);
		ourShader->use();
		glBindVertexArray(VAO);//绑定对应的VAO。此例子中因为只有一个VAO，所以其实可绑可不绑
		//glDrawArrays(GL_TRIANGLES, 0, 3);


		/*
		 * 十、使用EBO渲染
		 */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);//也可以不解绑



		glfwSwapBuffers(window);//交换color buffer
		glfwPollEvents();//监听是否有任何事件被触发，比如键盘输入，鼠标移动，更新窗口状态，并调用对应的函数
	}

	/*
	 * 5. 退出了render loop，销毁资源
	 */
	glfwTerminate();//销毁分配的内存空间



	return 0;
}
//回调函数
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

/*
 * 6. 处理输入
 */
void preocessInput(GLFWwindow *window) {
	/*
	 * 如果输入的是Esc键，则退出循环，关闭窗口;
	 * 如果不是的话，glfwGetKey会返回GLFW_RELEASE
	 */
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}