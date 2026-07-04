#define ACTIVE
#ifdef ACTIVE
#include "Core.h"

// Prototypes.
// -----------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	// Constants.
	// ----------
	constexpr int WINDOW_WIDTH{ 800 }, VIEWPORT_WIDTH{ 800 } ,
		WINDOW_HEIGHT{ 600 }, VIEWPORT_HEIGHT{ 600 };

	// Initialize logger.
	// ------------------
	Durik::Log::Init();
	DV_TRACE("Logger initialized.");

	// GLFW: Initialize and configure.
	// -------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	DV_TRACE("GLFW initialized.");
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
#endif

	// GLFW: Window creation.
	// ----------------------
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		DV_ERROR("Failed to create GLFW window. Exiting.");
		glfwTerminate();
		return -1;
	}
	DV_TRACE("Window created.");
	glfwMakeContextCurrent(window);

	// Registering the window resize callback function.
	// ------------------------------------------------
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD: Load all GLFW function pointers.
	// --------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		DV_ERROR("GLAD failed to initialize. Exiting.");
		return -1;
	}

	// Tell OpenGL the size of the RENDERING window.
	// ---------------------------------------------
	glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

	// Create a shader program for the renderer to use.
	// ------------------------------------------------
	unsigned int vertexShader[2];
	vertexShader[0] = Durik::Shaders::CompileVertexShader(0);
	vertexShader[1] = Durik::Shaders::CompileVertexShader(0);
	unsigned int fragShader[2]; 
	fragShader[0] = Durik::Shaders::CompileFragShader(0);
	fragShader[1] = Durik::Shaders::CompileFragShader(1);
	unsigned int shaderProgram[2];
	shaderProgram[0] = Durik::Shaders::CreateShaderProgram(vertexShader[0], fragShader[0]);
	shaderProgram[1] = Durik::Shaders::CreateShaderProgram(vertexShader[1], fragShader[1]);
	DV_TRACE("Shader program created, ID: {}", shaderProgram[0]);
	DV_TRACE("Shader program created, ID: {}", shaderProgram[1]);

	// Creating the triangle.
	// ------------------------
	// Create some vertex data
	float vertices1[]{	// right triangle
		 0.0f,  0.0f, 0.0f, // vertex #1 (x, y, z)  
		 0.2f,  0.5f, 0.0f, // #2  
		 0.2f,  0.0f, 0.0f  // #3 
	};
	float vertices2[]{	// left triangle
		 0.0f, 0.0f, 0.0f,
		-0.2f, 0.5f, 0.0f,
		-0.2f, 0.0f, 0.0f
	};
	// Indicate indexes to use (only 3 vertices in this case)
	unsigned int indices[] = {
		0, 1, 2,   // triangle
	};
	// Create the buffer objects. Since we have indices, we have an element buffer object (EBO)
	unsigned int VBO[2], EBO[2], VAO[2];
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);
	glGenVertexArrays(2, VAO);
	// Bind the vertex buffer object, then copy in the vertices data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	// Bind the vertex array object
	glBindVertexArray(VAO[0]);
	// Bind the element buffer object (note: since VAO is binded it will store the EBO data)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Set the vertex attribute pointers
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	// Repeat?
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	// Render loop.
	// ------------
	while (!glfwWindowShouldClose(window)) {
		// Process user input.
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw stuff.
		glUseProgram(shaderProgram[0]);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		// GLFW: Swap buffers and poll events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (glfwWindowShouldClose) DV_TRACE("Window closed.");

	// De-allocate unneeded resources.
	// -------------------------------
	DV_TRACE("Deleting buffers.");
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);
	DV_TRACE("Deleting vertex arrays.");
	glDeleteVertexArrays(2, VAO);
	DV_TRACE("Deleting programs.");
	glDeleteProgram(shaderProgram[0]);
	glDeleteProgram(shaderProgram[1]);

	// GLFW: Delete all allocated GLFW resources before exit.
	// ------------------------------------------------------
	DV_TRACE("Deleting GLFW resources.");
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
#endif