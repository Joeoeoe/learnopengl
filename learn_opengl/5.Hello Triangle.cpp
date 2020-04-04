#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include<iostream>
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
		0.5f, 0.5f, 0.0f,//����
		0.5f, -0.5f, 0.0f,//����
		-0.5f, -0.5f, 0.0f,//����
		-0.5f, 0.5f, 0.0f//����

	};
	unsigned int indices[] = {
		0, 1, 3,//��һ��������
		1, 2, 3//�ڶ���������
	};





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
	 * ����vertex shader
	 */
	 //Ŀǰ����򵥰汾��shader������������ʲô��û����ֱ������ˡ�
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0 ) in vec3 aPos;\n" //����vec3�������������ΪaPos
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"//����vertex shader��ouput��gl_Position������һ��vec4
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//���ñ���õ�shader���ڶ�����������˼�Ǵ�����ٸ�shader�ַ�����
	glCompileShader(vertexShader);//��̬����shader

	//���Ա����Ƿ�ɹ�
	int sucess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPLIATION_FAILED\n" << infoLog << endl;
	}

	/*
	 * �ġ�fragment shader
	 */
	 //ƬԪ��ɫ��������Ҫһ�����
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"//���������ΪFragColor
		"void main()\n"
		"{\n"
		"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";



	//����ƬԪ��ɫ���Ĺ����붥����ɫ������
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//�����Ƿ����ɹ�
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED\n" << infoLog << endl;
	}
	//����shader���ѱ�����ϣ�������Ҫ����������shader����shader program��������Ⱦ

	/*
	 * �塢shader program
	 * �ѱ���õĶ��shader����������Ϊshader program������Ⱦ����ʱ���������������shader����Ⱦ�����оͻᱻʹ��
	 */
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//attach shader������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//�����Ƿ����ӳɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
	if (!sucess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::PROGRAM::LINK::COMPLIATION_FAILED\n" << infoLog << endl;
	}

	//����program�������render����ʹ�õ���program
	glUseProgram(shaderProgram);

	//���ڿ���ɾ��֮ǰ�����shader��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//��ĿǰΪֹ�Ѿ��Ѷ��������ͽ�GPU�ڴ桢����GPUӦʹ��shader����data
	//����������OpenGL��ν���GPU�ڴ��е����ݣ���ΰѶ����������ӵ�shader


	/*
	 * �������Ӷ�������
	 */

	 //����OpenGL��ΰѶ������ݺ�vertex shader��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	//���߶�������λ��
	glad_glEnableVertexAttribArray(0);






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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);


		/*
		 * ʮ��ʹ��EBO��Ⱦ
		 */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);



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

