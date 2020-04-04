#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include<iostream>
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
		0.5f, 0.5f, 0.0f,//右上
		0.5f, -0.5f, 0.0f,//右下
		-0.5f, -0.5f, 0.0f,//左下
		-0.5f, 0.5f, 0.0f//左上

	};
	unsigned int indices[] = {
		0, 1, 3,//第一个三角形
		1, 2, 3//第二个三角新
	};





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
	 * 三、vertex shader
	 */
	 //目前是最简单版本的shader，对输入数据什么都没做就直接输出了。
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0 ) in vec3 aPos;\n" //设置vec3输入变量，名命为aPos
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"//设置vertex shader的ouput。gl_Position变量是一个vec4
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//设置编码好的shader，第二个参数的意思是传入多少个shader字符串码
	glCompileShader(vertexShader);//动态编译shader

	//测试编译是否成功
	int sucess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPLIATION_FAILED\n" << infoLog << endl;
	}

	/*
	 * 四、fragment shader
	 */
	 //片元着色器仅仅需要一个输出
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"//定义变量名为FragColor
		"void main()\n"
		"{\n"
		"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";



	//创建片元着色器的过程与顶点着色器类似
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//测试是否编译成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED\n" << infoLog << endl;
	}
	//现在shader均已编译完毕，接下来要做的是连接shader对象到shader program，用于渲染

	/*
	 * 五、shader program
	 * 把编译好的多个shader连接起来作为shader program，当渲染对象时激活他，被激活的shader在渲染过程中就会被使用
	 */
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//attach shader并连接
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//测试是否连接成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
	if (!sucess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::PROGRAM::LINK::COMPLIATION_FAILED\n" << infoLog << endl;
	}

	//激活program，后面的render都会使用到此program
	glUseProgram(shaderProgram);

	//现在可以删除之前编译的shader了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//到目前为止已经把顶点数据送进GPU内存、告诉GPU应使用shader处理data
	//接下来告诉OpenGL如何解释GPU内存中的数据，如何把顶点数据连接到shader


	/*
	 * 六、连接顶点属性
	 */

	 //告诉OpenGL如何把顶点数据和vertex shader连接起来
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	//告诉顶点属性位置
	glad_glEnableVertexAttribArray(0);






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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);


		/*
		 * 十、使用EBO渲染
		 */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);



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

