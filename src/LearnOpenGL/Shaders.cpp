#include "Shaders.h"
#include "Core.h"
#include "Log.h"

unsigned int Durik::Shaders::CompileVertexShader(int source_code_num) {
	// Create the shader source code
	const char* vertexShaderSource = vertexShaderSourceCode[source_code_num];
	DV_TRACE("Vertex source code #{} loaded.", source_code_num + 1);

	//Compile the shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for compile-time errors
	int success{};
	char infoLog[infoLogSize];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, infoLogSize, NULL, infoLog);
		DV_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
		DV_ERROR(infoLog);
	}
	return vertexShader;
}
unsigned int Durik::Shaders::CompileFragShader(int source_code_num) {
	// Create the shader source code
	const char* fragShaderSource = fragShaderSourceCode[source_code_num];
	DV_TRACE("Fragment source code #{} loaded.", source_code_num + 1);

	//Compile the shader
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragShader);

	// Check for compile-time errors
	int success{};
	char infoLog[infoLogSize];
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, infoLogSize, NULL, infoLog);
		DV_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
		DV_ERROR(infoLog);
	}
	return fragShader;
}
unsigned int Durik::Shaders::CreateShaderProgram(unsigned int &vertexShader, unsigned int &fragShader) {
	unsigned int shaderProgram{};

	// Create a Shader Program Object.
	shaderProgram = glCreateProgram();

	// Attach the compiled shaders to the Shader Program Object.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);

	// Link the shaders to the Shader Program Object.
	glLinkProgram(shaderProgram);

	// Check for compile-time linking errors.
	int success{};
	char infoLog[infoLogSize];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, infoLogSize, NULL, infoLog);
		DV_ERROR("ERROR::SHADER::PROGRAM::LINK_FAILED\n");
		DV_ERROR(infoLog);
	}

	// After linking, delete the shader objects.
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	return shaderProgram;
}
