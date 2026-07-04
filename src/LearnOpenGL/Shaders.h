#pragma once

namespace Durik {
	class Shaders
	{
		static constexpr int infoLogSize = 512;
		inline static const char* vertexShaderSourceCode[] = {
			// Source Code #1
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0"
		};
		inline static const char* fragShaderSourceCode[] = {
			// Source Code #1
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // Orange
			"}\0",
			// Source Code #2
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n" // Yellow
			"}\0"
		};
	public:
		static unsigned int CompileVertexShader(int source_code_num);
		static unsigned int CompileFragShader(int source_code_num);
		static unsigned int CreateShaderProgram(unsigned int& vertexShader, unsigned int& fragShader);
	};
}