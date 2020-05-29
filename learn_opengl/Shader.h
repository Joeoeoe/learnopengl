#ifndef SHADER_H
#define SHADER_H

#include<glad\glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader {
    public:
    	unsigned int ID;
		//构造函数
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
			//1.从着色器路径拿到代码
			std::string vertexCode;
			std::string fragmentCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;

			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try {
				//打开文件
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				std::stringstream vShaderStream, fShaderStream;

				//读取文件buffer内容至stream中
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();

				//关闭文件
				vShaderFile.close();
				fShaderFile.close();

				//将stream转换至字符串
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
			}
			catch (std::ifstream::failure e) {
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULY_READ" << std::endl;
			}

			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();

			//std::cout << vShaderCode;

			//2.编译shader
			unsigned int vertex, fragment;
			int success;
			char infoLog[512];

			//vertex shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex,1, &vShaderCode, NULL);
			glCompileShader(vertex);

			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertex, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPLILATION_FAILED\n" << infoLog << std::endl;
			}

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment,1, &fShaderCode, NULL);
			glCompileShader(fragment);

			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragment, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::CIMPLILATION_FAILED\n" << infoLog << std::endl;
			}

			//创建shader program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);

			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		void use() {
			glUseProgram(ID);
		}

		void setBool(const std::string &name, bool value) const {
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}
		void setInt(const std::string &name, int value) const {
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		void setFloat(const std::string &name, float value) const {
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}
};

#endif
