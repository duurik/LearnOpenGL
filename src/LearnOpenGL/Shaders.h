#pragma once

namespace Durik {
	class Shaders
	{
		static constexpr int infoLogSize = 512;
		inline static const char* vertexShaderSourceCode[] = {
			// Source Code #1
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"out vec4 vertexColor;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"	vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0);\n"
			"}\0"
		};
		inline static const char* fragShaderSourceCode[] = {
			// Source Code #1
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec4 vertexColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vertexColor;\n"
			"}\0",
			// Source Code #2
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec4 ourColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = ourColor;\n"
			"}\0"
		};
	public:
		static unsigned int CompileVertexShader(int source_code_num);
		static unsigned int CompileFragShader(int source_code_num);
		static unsigned int CreateShaderProgram(unsigned int& vertexShader, unsigned int& fragShader);
	};
}