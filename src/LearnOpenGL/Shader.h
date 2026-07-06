#pragma once
#include <glad/glad.h>
#include "Log.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Durik {
	class Shader
	{
	private:
		static constexpr int infoLogSize = 4096;
		std::string vShaderType{ "VERTEX" }, fShaderType{ "FRAGMENT" }, programType{ "PROGRAM"};
		std::string loadFile(const std::string& path);
		void compileShaders(const char* vShader, const char* fShader);
		void checkCompileErrors(unsigned int ref, const std::string& type, bool& CompSuccess);
	public:
		// Program ID
		unsigned int ID{};
		// Constructor
		Shader(const char* vertexPath, const char* fragmentPath);
		// use/activate the shader
		void useProgram();
		// utility uniform functions
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
	};
}

