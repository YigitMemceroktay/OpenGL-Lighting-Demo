#include "Window.h"
#include <vector>

#include "stb_image.h"
namespace Engine {
	float WindowProperties::Width;
	float WindowProperties::Height;
	std::string WindowProperties::Name;
	GLFWwindow* WindowProperties::window;
	glm::mat4 projection;
	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;
	void renderQuad();
	void processInput(GLFWwindow* window);
	glm::mat4 view;
	unsigned int hdrTexture;
	float isSphere = true;
	bool firstMouse = true;
	double lastX = 400;
	double lastY = 300;
	float YAW = -90.0f;
	float PITCH = 0.0f;	
	Camera camera;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 6.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 lightPositions[] = {
	glm::vec3(0.f,  0.,10.0f),
	
	};
	glm::vec3 lightColors[] = {
		glm::vec3(150.0f, 150.0f,150.0f),
		
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	std::vector<float> vertices;

	std::vector<unsigned int> indices;
	std::vector<int> lineIndices;
	void createSphereVertices(float radius, int sectorCount, int stackCount)
	{
		float PI = 3.1415;




		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				vertices.push_back(nx);
				vertices.push_back(ny);
				vertices.push_back(nz);

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				vertices.push_back(s);
				vertices.push_back(t);
			}
		}
		// generate CCW index list of sphere triangles
		// k1--k1+1
		// |  / |
		// | /  |
		// k2--k2+1

		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}

				// store indices for lines
				// vertical lines for all stacks, k1 => k2
				lineIndices.push_back(k1);
				lineIndices.push_back(k2);
				if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
				{
					lineIndices.push_back(k1);
					lineIndices.push_back(k1 + 1);
				}
			}
		}
	}

	void setUpImGui(GLFWwindow* window)
	{
		ImGui::CreateContext();
		//ImGui::SetFl
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;
		//style->WindowRounding = 10;
		colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		style->FramePadding = ImVec2(20, 2);
		style->ItemSpacing = ImVec2(10, 10);
		style->IndentSpacing = 12;
		style->ScrollbarSize = 10;

		style->WindowRounding = 15;
		style->FrameRounding = 4;
		style->PopupRounding = 4;
		style->ScrollbarRounding = 6;
		style->GrabRounding = 4;
		style->TabRounding = 4;

		style->WindowTitleAlign = ImVec2(1.0f, 0.5f);
		style->WindowMenuButtonPosition = ImGuiDir_Right;

		style->DisplaySafeAreaPadding = ImVec2(1, 4);


	}
	void FrameBufferCallback(GLFWwindow* window, int width, int height)
	{
		WindowProperties::Height = (float)height;
		WindowProperties::Width = (float)width;
		std::cout << WindowProperties::Height << std::endl;
		std::cout << WindowProperties::Width << std::endl;
		glViewport(0., 0., width-250, height);
		
		glDeleteFramebuffers(1, &framebuffer);
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture

		//glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width- 250, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

		//glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width-250, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//	cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		

	}

	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		YAW += xoffset;
		PITCH += yoffset;

		if (PITCH > 89.0f)
			PITCH = 89.0f;
		if (PITCH < -89.0f)
			PITCH = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
		direction.y = sin(glm::radians(PITCH));
		direction.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
		cameraFront = glm::normalize(direction);
		
	}
	Window::Window(std::string name, int Width, int Height)
	{
		WindowProperties::Name = name;
		WindowProperties::Width = Width;
		WindowProperties::Height = Height;

	}

	void Window::Init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		glfwWindowHint(GLFW_SAMPLES, 4);
		WindowProperties::window = glfwCreateWindow(WindowProperties::Width, WindowProperties::Height, WindowProperties::Name.c_str(), NULL, NULL);


		glfwMakeContextCurrent(WindowProperties::window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize OpenGL context" << std::endl;

		}
		glViewport(0, 0, 1670., 1080.);
		glEnable(GL_MULTISAMPLE);


		//glfwSetCursorPosCallback(WindowProperties::window, cursorPosCallback);

		glfwSetFramebufferSizeCallback(WindowProperties::window, FrameBufferCallback);

		//glEnable(GL_BLEND);

		//glfwSetInputMode(props.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		ResourceManager::LoadShader("default",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Defaultvs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Defaultfs.glsl"
		);
		ResourceManager::LoadShader("frame",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Framevs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Framefs.glsl"
		);
		ResourceManager::LoadShader("pbr"
			, "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Pbrvs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Pbrfs.glsl");

		ResourceManager::LoadShader("cubemap"
			, "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Cubemapvs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Cubemapfs.glsl");
		Shader defaultShader = ResourceManager::GetShader("default");

		ResourceManager::LoadTexture("albedo", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\albedo.png");
		ResourceManager::LoadTexture("metallic", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\metallic.png");
		ResourceManager::LoadTexture("roughness", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\roughness.png");
		ResourceManager::LoadTexture("ao", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\ao.png");
		ResourceManager::LoadTexture("normal", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\normal.png");
		
		ResourceManager::LoadTexture("albedof", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\albedo.png");
		ResourceManager::LoadTexture("metallicf", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\metallic.png");
		ResourceManager::LoadTexture("roughnessf", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\roughness.png");
		ResourceManager::LoadTexture("aof", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\ao.png");

		ResourceManager::LoadTexture("normalf", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\normal.png");

		Shader pbrShader = ResourceManager::GetShader("pbr");
		pbrShader.Use();
	
		
		pbrShader.SetInt1("albedoMap", 0);
		pbrShader.SetInt1("normalMap", 1);
		pbrShader.SetInt1("metallicMap", 2);
		pbrShader.SetInt1("roughnessMap", 3);
		pbrShader.SetInt1("aoMap", 4);
		stbi_set_flip_vertically_on_load(true);
		int width, height, nrComponents;
		float* data = stbi_loadf("./src/Textures/cubemap/Mt-Washington-Gold-Room_Ref.hdr", &width, &height, &nrComponents, 0);
		
		if (data)
		{
			glGenTextures(1, &hdrTexture);
			glBindTexture(GL_TEXTURE_2D, hdrTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load HDR image." << std::endl;
		}

	}

	void Window::GameLoop()
	{

		createSphereVertices(1., 100, 100);

		unsigned int VBOS, VAOS, VEOS;
		glGenVertexArrays(1, &VAOS);
		glBindVertexArray(VAOS);

		glGenBuffers(1, &VBOS);
		glBindBuffer(GL_ARRAY_BUFFER, VBOS);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

		glGenBuffers(1, &VEOS);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEOS);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);
		// ------------------------------------------------------------------
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			 // positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
			// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);

		// screen quad VAO
		unsigned int quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


		//SETUP FRAME BUFFER
	
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture
		
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1670, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1670, 1080); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//	cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Timer timer;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		
		projection = glm::perspective(glm::radians(45.f), (WindowProperties::Width-250.f) / WindowProperties::Height, 0.1f, 100.0f);


		//Setting Up ImGUI
		setUpImGui(WindowProperties::window);
		//Add a Custom Font To ImGui
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font1 = io.Fonts->AddFontFromFileTTF("C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\OpenGL\\font.ttf", 16);


		//Variables that will be modified with ImGUI
		bool open = true;
		float colors1[4] = { 229. / 255.,57. / 255.,219. / 255.,1. };
		float rotate1 = 0;
		float rotate2 = 0;
		float rotate3 = 0;
		float rotate4 = 0;
		float roughness = 0.05;
		float mettalic = 0.0f;

		// GET SHADERS
		Shader defaultShader = ResourceManager::GetShader("default");
		Shader screenShader = ResourceManager::GetShader("frame");
		Shader pbrShader = ResourceManager::GetShader("pbr");
		Shader cubemapShader = ResourceManager::GetShader("cubemap");


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glm::mat4 lightModel  = glm::mat4(1.0f);
		while (!glfwWindowShouldClose(WindowProperties::window))
		{
			

		
			processInput(WindowProperties::window);

			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			projection = glm::perspective(glm::radians(45.f), (WindowProperties::Width - 250.f) / WindowProperties::Height, 0.1f, 100.0f);

			//Fps counter
			timer.currentTime = glfwGetTime();
			timer.deltaTime = timer.currentTime - timer.lastTime;
			timer.lastTime = timer.currentTime;
			//If you want to display fps -> std::cout << 1. / timer.deltaTime;

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
			glClearColor(0.0f, 0.0, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::scale(model, glm::vec3(10.0f, 0.2f, 10.0f));
			pbrShader.Use();
			pbrShader.SetMat4("projection", projection);
			pbrShader.SetMat4("view", view);
			pbrShader.SetMat4("model", model);
			pbrShader.SetFloat3("camPos", cameraPos);		
			pbrShader.SetFloat3("albedo", glm::vec3(colors1[0],colors1[1],colors1[2]));
		
			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
				lightModel = glm::mat4(1.0f);
				if(i == 0)
					lightModel = glm::rotate(lightModel, glm::radians(rotate1), glm::vec3(0., 1., 0.));
				else if(i == 1)
					lightModel = glm::rotate(lightModel, glm::radians(rotate2), glm::vec3(0., 1., 0.));
				else if(i == 2)
					lightModel = glm::rotate(lightModel, glm::radians(rotate3), glm::vec3(0., 1., 0.));
				else if(i == 3)
					lightModel = glm::rotate(lightModel, glm::radians(rotate4), glm::vec3(0., 1., 0.));
				lightModel = glm::translate(lightModel,lightPositions[i]);

				glm::vec3 newPos;
				newPos = lightPositions[i];
				pbrShader.SetFloat3(("lightPosition[" + std::to_string(i) + "]").c_str(),glm::mat3(lightModel)*newPos);
				pbrShader.SetFloat3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
				
			}
		
			
			
			glBindVertexArray(VAOS);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("albedo"));
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("normal"));
			

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("metallic"));
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("roughness"));
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("ao"));

			//glDrawElements(GL_TRIANGLE_STRIP, 100 * 100 * 10, GL_UNSIGNED_INT, 0);
		
			
			glDrawElements(GL_TRIANGLE_STRIP, 100 * 100 * 10, GL_UNSIGNED_INT, 0);
			/*
			cubemapShader.Use();
			cubemapShader.SetMat4("projection", projection);
			cubemapShader.SetMat4("view", view);
			cubemapShader.SetInt1("equirectangularMap", 0.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, hdrTexture);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			*/

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glBlitFramebuffer(0, 0, WindowProperties::Width-250, WindowProperties::Height, 0, 0,WindowProperties::Width-250., WindowProperties::Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
			// clear all relevant buffers
			glClearColor(1.0f, 1.f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT);

			screenShader.Use();
			glBindVertexArray(quadVAO);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
			glDrawArrays(GL_TRIANGLES, 0, 6);

			//std::cout << glGetError();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::SetNextWindowPos({ 0.0,0.0 });
			ImGui::SetNextWindowSize({ 250.,(float)WindowProperties::Height });


			ImGui::Begin("Demo  window", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			ImGui::PushFont(font1);

		
			ImGui::Text("Material Properties");
			ImGui::SetWindowFontScale(1.0);

			ImGui::Text("Object Color");
			ImGui::ColorPicker4("Object Color", colors1);

		
			ImGui::Spacing();
			ImGui::SetWindowFontScale(1.5);
		
			ImGui::SetWindowFontScale(1.0);

			ImGui::Separator();

			ImGui::Spacing();
			ImGui::SetWindowFontScale(1.5);
			ImGui::Text("Light Properties");

			ImGui::SetWindowFontScale(1.0);
			ImGui::Spacing();
		
			ImGui::SliderFloat("Light Rotation ", &rotate1, 0.0f, 360.0f, "%f degrees", 1.0f);
		

			ImGui::SliderFloat("Roughness",&roughness , 0.05, 1.0f, "", 0);
			ImGui::SliderFloat("Mettalic",&mettalic , 0.0, 1.0f, "", 0);

			ImGui::PopFont();
			ImGui::End();
			ImGui::SetNextWindowPos({ 250.0,0.0 });
			ImGui::SetNextWindowSize({ (float)WindowProperties::Width-250.0f,(float)WindowProperties::Height });
			ImGui::Begin("GameWindow", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			ImGui::PushFont(font1);
			
			ImGui::BeginChild("GameRender");
			// Get the size of the child (i.e. the whole draw size of the windows).
			ImVec2 wsize = ImGui::GetWindowSize();
			// Because I use the texture from OpenGL, I need to invert the V from the UV.
			ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		
			ImGui::PopFont();
			ImGui::End();
			// Render dear imgui into screen
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(WindowProperties::window);
			glfwPollEvents();

		}



	};
	void processInput(GLFWwindow* window)
	{
		
			const float cameraSpeed = 0.05f; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
};