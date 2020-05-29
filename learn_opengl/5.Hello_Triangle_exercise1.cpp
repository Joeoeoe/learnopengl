#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include<iostream>
using namespace std;
void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void preocessInput(GLFWwindow *window);

int main() {
	glfwInit();//��ʼ��GLFW

	//����GLFW������1����Ҫ���õ�ѡ�����2��ѡ��ֵ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ʹ�ú���ģʽ

	//��������
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();//��ɣ��������еĴ��ڡ�cursor

		return -1;
	}
	//����������
	glfwMakeContextCurrent(window);

	//GLAD����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//�������ڵ����Ļص�����
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);


	/*
	 *��Ҫ����
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





	//ʹ�ô��ڲ������Ϲر�
	while (!glfwWindowShouldClose(window))//���˴����Ƿ�Ҫ��ر�
	{
		preocessInput(window);//��������


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//������Ⱦ��ָ�����
		//...
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);




		glfwSwapBuffers(window);//����color buffer
		glfwPollEvents();//�����Ƿ����κ��¼�������������������룬����ƶ������´���״̬�������ö�Ӧ�ĺ���
	}

	glfwTerminate();//���ٷ�����ڴ�ռ�

	return 0;
}
//�ص�����
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}


void preocessInput(GLFWwindow *window) {
	/*
	 * ����������Esc�������˳�ѭ�����رմ���;
	 * ������ǵĻ���glfwGetKey�᷵��GLFW_RELEASE
	 */
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}