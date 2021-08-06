#include "Window.h"
#include <vector>


namespace Engine{
	glm::mat4 view;
	float isSphere = true;
	bool firstMouse = true;
	double lastX = 400;
	double lastY = 300;
	 float YAW = -90.0f;
	 float PITCH = 0.0f;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 6.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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

		style->FramePadding = ImVec2(4, 2);
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

		style->DisplaySafeAreaPadding = ImVec2(4, 4);
	
		
	}
	void FrameBufferCallback(GLFWwindow* window, int width, int height)
	{
		//glViewport(0, 0, width, height);
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
		props.Name = name;
		props.Width = Width;
		props.Height = Height;

	}

	void Window::Init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		props.window = glfwCreateWindow(props.Width, props.Height, props.Name.c_str(), NULL, NULL);
		
		
		glfwMakeContextCurrent(props.window);
	
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize OpenGL context" << std::endl;

		}
		glViewport(0, 0, 800., 600.);
		glEnable(GL_MULTISAMPLE);
		

		//glfwSetCursorPosCallback(props.window, cursorPosCallback);
		
		glfwSetFramebufferSizeCallback(props.window,FrameBufferCallback);
	
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
		Shader defaultShader = ResourceManager::GetShader("default");

		ResourceManager::LoadTexture("albedo", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\albedo.jpg");
		ResourceManager::LoadTexture("metallic", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\metallic.jpg");
		ResourceManager::LoadTexture("roughness", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\roughness.jpg");
		ResourceManager::LoadTexture("ao", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\ao.jpg");
		ResourceManager::LoadTexture("normal", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\normal.jpg");
	
	
	}

	void Window::GameLoop()
	{
		
		createSphereVertices(1., 100, 100);

		unsigned int VBOS, VAOS,VEOS;
		glGenVertexArrays(1, &VAOS);
		glBindVertexArray(VAOS);

		glGenBuffers(1, &VBOS);
		glBindBuffer(GL_ARRAY_BUFFER, VBOS);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
			/*
		glBufferSubData(GL_ARRAY_BUFFER,0,vertices.size()*sizeof(float),&vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),normals.size()*sizeof(float),&normals[0]);
		glBufferSubData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float)+normals.size()*sizeof(float),normals.size()*sizeof(float),&texCoords[0]);
	*/	
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*8,0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) *6));

		glGenBuffers(1, &VEOS);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEOS);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(float),&indices[0],GL_STATIC_DRAW );
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
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)* 6));
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
		unsigned int framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture
		unsigned int textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800,600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		
	

		Timer timer;
		


		view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 100.0f);
		

		 
		//Setting Up ImGUI
		setUpImGui(props.window);
		//Add a Custom Font To ImGui
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font1 = io.Fonts->AddFontFromFileTTF("C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\OpenGL\\font.ttf", 16);

		
		//Variables that will be modified with ImGUI
		bool open = true;
		float colors1[4] = {229./255.,57./255.,219./255.,1.};
		float rotate = 0;
		float specPow = 64.0f;
		float specStrength = 0.6f;
		float ambientStrength = 0.8f;
		float ambient[3] = { 170.f/255.,170./255.0f,170./255.0f };
		float diffuse[3] = { 115./255.0f,115/255.0f,115/255.0f };
		float specular[3] = { 150./255.,150./255.f,150./255.f };
		float rotationx = 20.f;
		float rotationy = 45.f;
		float rotationz = 0.f;
		// GET SHADERS
		Shader defaultShader = ResourceManager::GetShader("default");
		Shader screenShader = ResourceManager::GetShader("frame");
		Shader pbrShader = ResourceManager::GetShader("pbr");
		pbrShader.Use();
		pbrShader.SetFloat1("albedoMap", 0);
		pbrShader.SetFloat1("normalMap", 1.);
		pbrShader.SetFloat1("metallicMap", 2.);
		pbrShader.SetFloat1("roughnessMap", 3.);
		pbrShader.SetFloat1("aoMap", 4.);

		
		while (!glfwWindowShouldClose(props.window))
		{
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			//Fps counter
			timer.currentTime = glfwGetTime();
			timer.deltaTime = timer.currentTime - timer.lastTime;
			timer.lastTime = timer.currentTime;
			//If you want to display fps -> std::cout << 1. / timer.deltaTime;
			
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
			glClearColor(1.0f,1.0, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glm::mat4 lightModel = glm::mat4(1.0f);
			lightModel = glm::rotate(lightModel, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
			lightModel = glm::translate(lightModel, glm::vec3(0.0f, 2.0f, -5.0f));
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(rotationx), glm::vec3(1., 0., 0.));
			model = glm::rotate(model, glm::radians(rotationy), glm::vec3(0., 1., 0.));
			model = glm::rotate(model, glm::radians(rotationz), glm::vec3(0., 0., 1.));
			defaultShader.Use();
			defaultShader.SetMat4("projection", projection);
			defaultShader.SetMat4("view", view);
			defaultShader.SetMat4("model", model);
			defaultShader.SetFloat1("specPow", specPow);
			defaultShader.SetFloat1("specularStrength",specStrength);
			defaultShader.SetFloat1("ambientStrength", ambientStrength);
			
			defaultShader.SetMat4("lightModel", lightModel);
			defaultShader.SetFloat4("color", glm::vec4(colors1[0],colors1[1],colors1[2],colors1[3]));
			defaultShader.SetFloat3("viewPos", glm::vec3(0.0f, 2.0f, 5.0f));
			
			defaultShader.SetFloat3("ambient_color", glm::vec3(ambient[0], ambient[1], ambient[2]));
			defaultShader.SetFloat3("diffuse_color", glm::vec3(diffuse[0],diffuse[1], diffuse[2]));
			//glBindVertexArray(VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		
			//std::cout << glGetError();

						
			/*
			pbrShader.Use();
			pbrShader.SetMat4("projection", projection);
			pbrShader.SetMat4("view", view);
			pbrShader.SetMat4("model", model);
			pbrShader.SetMat4("lightModel", lightModel);
			pbrShader.SetFloat3("albedo", glm::vec3(0.8, 0., 0.));
			pbrShader.SetFloat1("ao", 1.0f);
			pbrShader.SetFloat1("metallic", 0.3);
			pbrShader.SetFloat1("roughness", 0.3);
			pbrShader.SetFloat3("lightColor", glm::vec3(0.8f, 0.8f, 0.8f));
			pbrShader.SetFloat3("camPos", cameraPos);
		
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,ResourceManager::GetTexture("albedo"));
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("normal"));
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("metallic"));
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("roughness"));
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("ao"));
			*/
			if (isSphere) {
				glBindVertexArray(VAOS);
				glDrawElements(GL_TRIANGLES, 100 * 100 * 10, GL_UNSIGNED_INT, 0);
			}
			else
			{
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			std::cout << glGetError();
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
			// clear all relevant buffers
			glClearColor(1.0f, 1.f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT);

			screenShader.Use();
			glBindVertexArray(quadVAO);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
			glDrawArrays(GL_TRIANGLES, 0, 6);


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::SetNextWindowPos({ 0.0,0.0 });
			ImGui::SetNextWindowSize({ 250.,600. });
			

			ImGui::Begin("Demo  window", &open,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			ImGui::PushFont(font1);
		
				ImGui::SetWindowFontScale(1.5);
				ImGui::Text("Shapes");
				ImGui::SetWindowFontScale(1.0);
			if(ImGui::Button("Sphere"))
			{
				isSphere = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cube"))
			{
				isSphere = false;
			}
		
			ImGui::SetWindowFontScale(1.5);
			ImGui::Text("Object Properties");
			ImGui::SetWindowFontScale(1.0);
		
			ImGui::Text("Object Color");
			ImGui::ColorPicker4("Object Color", colors1);
			
			ImGui::Text("Object Rotation");
			ImGui::SliderFloat("x rotation", &rotationx,0.0f,360.0f ,"%f degrees" ,1.0f);
			ImGui::SliderFloat("y rotation", &rotationy,0.0f,360.0f ,"%f degrees" ,1.0f);
			ImGui::SliderFloat("z rotation", &rotationz,0.0f,360.0f ,"%f degrees" ,1.0f);
	
		
			
			
			
			
			
			
		
			
			
			ImGui::Spacing();
			ImGui::SetWindowFontScale(1.5);
			ImGui::Text("Light Properties");
			ImGui::SetWindowFontScale(1.0);
			

			ImGui::SliderFloat("Light Rotation", &rotate, 0.0f, 360.0f, "%f degrees", 1.0f);
			ImGui::Text("Specular Power");
			ImGui::InputFloat("", &specPow, 0.0f, 0.0f, "%f", 0);

			
			ImGui::InputFloat("Ambient Power", &ambientStrength);
			ImGui::Spacing();
			
			ImGui::InputFloat("Specular Strength", &specStrength);
			

		
			ImGui::Spacing();
			ImGui::SetWindowFontScale(1.5);
			ImGui::Text("Light Colors");
			ImGui::SetWindowFontScale(1.0);
			
			ImGui::ColorEdit3("Ambient", ambient,0 );
		
			ImGui::ColorEdit3("Diffuse", diffuse, 0);
		
			ImGui::ColorEdit3("Specular", specular, 0);
		
			ImGui::PopFont();
			ImGui::End();
			ImGui::SetNextWindowPos({ 250.0,0.0 });
			ImGui::SetNextWindowSize({ 800.,600. });
			ImGui::Begin("GameWindow", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			ImGui::PushFont(font1);
				// Using a Child allow to fill all the space of the window.
				// It also alows customization
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


			glfwSwapBuffers(props.window);
			glfwPollEvents();
		}
		
	}


	
};
