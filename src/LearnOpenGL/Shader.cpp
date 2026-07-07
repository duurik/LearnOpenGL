#include "Shader.h"

Durik::Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 1. Retrieve the vertex/fragment source code from the given file path and load
	std::string vertexCode = loadFile(vertexPath);
	std::string fragmentCode = loadFile(fragmentPath);

	// 2. Compile the shaders and the shader program
	compileShaders(vertexCode.c_str(), fragmentCode.c_str());
}

// Private methods.
// ----------------
std::string Durik::Shader::loadFile(const std::string& path) {
	DV_TRACE("Attemping to open file [{}]", path);
	std::ifstream file(path);
	std::stringstream stream;
	// Check if file is open
	if (!file.is_open()) {
		DV_CRIT("Failed to load file. Abort.");
		return "";
	}
	else DV_TRACE("File loaded.");
	stream << file.rdbuf();
	// Check if badbit and failbit
	if (file.bad()) {
		DV_CRIT("I/O error while reading shader file. Abort.");
		return "";
	}
	if (file.fail() && !file.eof()) {
		DV_CRIT("Format error while reading shader file. Abort.");
		return "";
	}
	return stream.str();
}
void Durik::Shader::compileShaders(const char* vShaderCode, const char* fShaderCode){
	unsigned int vShader, fShader;
	bool vCompSuccess{ false },
		fCompSuccess{ false },
		linkSuccess{ false };

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vShaderCode, NULL);
	glCompileShader(vShader);
	checkCompileErrors(vShader, vShaderType, vCompSuccess);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderCode, NULL);
	glCompileShader(fShader);
	checkCompileErrors(fShader, fShaderType, fCompSuccess);

	ID = glCreateProgram();
	if(vCompSuccess && fCompSuccess) {
		glAttachShader(ID, vShader);
		glAttachShader(ID, fShader);
		glLinkProgram(ID);
		checkCompileErrors(ID, programType, linkSuccess);
	}
	else {
		DV_ERROR("Shader program linking skipped due to compilation errors.");
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}
void Durik::Shader::checkCompileErrors(unsigned int ref, const std::string& type, bool& CompSuccess) {
	int success;
	char infoLog[infoLogSize];
	if (type == vShaderType || type == fShaderType) {
		glGetShaderiv(ref, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(ref, infoLogSize, NULL, infoLog);
			DV_ERROR("ERROR::SHADER::{}::COMPILATION_FAILED\n{}", type, infoLog);
		}
		else {
			DV_INFO("Shader {} compiled successfully.", type);
			CompSuccess = true;
		}
	}
	else if (type == programType) {
		glGetProgramiv(ref, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ref, infoLogSize, NULL, infoLog);
			DV_ERROR("ERROR::SHADER::{}::LINKING_FAILED\n{}", type, infoLog);
		}
		else {
			DV_INFO("Shader {} compiled successfully.", type);
			CompSuccess = true;
		}
	}
	else
		DV_ERROR("ERROR::INVALID_PARAMETER::checkCompileErrors()");
}

// Public methods.
// ---------------
void Durik::Shader::useProgram() {
	glUseProgram(ID);
}
void Durik::Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Durik::Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Durik::Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
