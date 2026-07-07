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
	Durik::Shader myShader("shaders/shader.vs","shaders/shader.fs");
	myShader.useProgram();

	// Creating a Shape.
	// ----------------------
	// Create some vertex data
	float vertices[]{
		// positions		 // colors			// texture coor
		-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,	0.0f, 0.0f
	};
	// Indicate indexes to use (only 3 vertices in this case)
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	// Create the buffer objects. Since we have indices, we have an element buffer object (EBO)
	unsigned int VBO, EBO, VAO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	// Bind the vertex buffer object, then copy in the vertices data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Bind the vertex array object
	glBindVertexArray(VAO);
	// Bind the element buffer object (note: since VAO is binded it will store the EBO data)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Set the vertex attribute pointers
	glEnableVertexAttribArray(0); // Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1); // Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2); // Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Generating textures.
	// ---------------------
	unsigned int texture0, texture1;
	glGenTextures(1, &texture0);
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("textures/container_wood_01.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		DV_ERROR("Failed to load texture \"container_wood_01.jpg\".");
	}
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		DV_ERROR("Failed to load texture.\"awesomeface.png\"");
	}
	stbi_image_free(data);

	glUniform1i(glGetUniformLocation(myShader.ID, "texture0"), 0);
	myShader.setInt("texture1", 1);

	// Render loop.
	// ------------
	float Offset{};
	while (!glfwWindowShouldClose(window)) {
		// Process user input.
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle.
		myShader.useProgram();
		Offset += 0.001f;
		if (Offset >= 1.0f) Offset = 0.0f;
		myShader.setFloat("posOff", Offset);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// GLFW: Swap buffers and poll events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (glfwWindowShouldClose) DV_TRACE("Window closed.");

	// De-allocate unneeded resources.
	// -------------------------------
	DV_TRACE("Deleting buffers.");
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	DV_TRACE("Deleting vertex arrays.");
	glDeleteVertexArrays(1, &VAO);

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