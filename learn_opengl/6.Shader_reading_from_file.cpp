#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include<iostream>
#include "Shader.h"

using namespace std;
void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void preocessInput(GLFWwindow *window);

int main() {
	/*
	 * 1. GLFW��ʼ��
	 */

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



	/*
	 * 2. GLAD����OpenGL����ָ��ĵ�ַ
	 */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	/*
	 * 3. Viewport
	 */
	glViewport(0, 0, 800, 600);

	//�������ڵ����Ļص�����
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);



	/*
	 * һ����������
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
		0, 1, 2,//��һ��������
	};

	Shader* ourShader = new Shader("shader1.vs", "shader1.fs");

	//Shader ourShader("shader1.vs", "shader1.fs");

	/*
	 * �ߡ�����VAO
	 */
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	/*
	 * ����VBO����
	 */
	unsigned int VBO;
	glGenBuffers(1, &VBO);//����buffer������VBO��buffer id
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//����buffer���ͣ�֮���GL_ARRAY_BUFFER�ĵ��ú�����Ҳ��ָ��VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//�����û��Զ����data����ǰ��buffer


	/*
	 * �š�Element Buffer Objects
	 * Ҫ��VBO�����ã�����
	 */

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);





	/*
	 * �������Ӷ�������
	 */

	 //����OpenGL��ΰѶ������ݺ�vertex shader��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	//���߶������Զ�Ӧvertex shader�е�location
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	//���VAO
	glBindVertexArray(0);





	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//ȫģʽ


	/*
	 * 4. render loop
	 */

	 //ʹ�ô��ڲ������Ϲر�
	while (!glfwWindowShouldClose(window))//���˴����Ƿ�Ҫ��ر�
	{
		preocessInput(window);//��������

		/*
		 * 7. ����clear color��clear
		 */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//������Ⱦ��ָ�����
		//...
		/*
		 * �ˡ�������Ⱦ
		 */
		//glUseProgram(shaderProgram);
		ourShader->use();
		glBindVertexArray(VAO);//�󶨶�Ӧ��VAO������������Ϊֻ��һ��VAO��������ʵ�ɰ�ɲ���
		//glDrawArrays(GL_TRIANGLES, 0, 3);


		/*
		 * ʮ��ʹ��EBO��Ⱦ
		 */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);//Ҳ���Բ����



		glfwSwapBuffers(window);//����color buffer
		glfwPollEvents();//�����Ƿ����κ��¼�������������������룬����ƶ������´���״̬�������ö�Ӧ�ĺ���
	}

	/*
	 * 5. �˳���render loop��������Դ
	 */
	glfwTerminate();//���ٷ�����ڴ�ռ�



	return 0;
}
//�ص�����
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

/*
 * 6. ��������
 */
void preocessInput(GLFWwindow *window) {
	/*
	 * ����������Esc�������˳�ѭ�����رմ���;
	 * ������ǵĻ���glfwGetKey�᷵��GLFW_RELEASE
	 */
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}