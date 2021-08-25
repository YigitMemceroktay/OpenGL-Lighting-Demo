#include "Window.h"
#include <vector>

#include "stb_image.h"
namespace Engine {
	float WindowProperties::Width;
	float WindowProperties::Height;
	std::string WindowProperties::Name;
	GLFWwindow* WindowProperties::window;

	glm::mat4 view;
	glm::mat4 projection;
	
	bool input = true;
	bool firstMouse = true;
	MousePicker mousePicker(projection, view);

	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;
	unsigned int quadVAO1 = 0;
	unsigned int quadVBO1;
	void renderQuad()
	{
		if (quadVAO1 == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO1);
			glGenBuffers(1, &quadVBO1);
			glBindVertexArray(quadVAO1);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO1);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	bool TestRayOBBIntersection(
		glm::vec3 ray_origin,        // Ray origin, in world space
		glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
		glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
		glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
		glm::mat4 ModelMatrix,  
		glm::vec3 ModelMatrix1,    // Transformation applied to the mesh (which will thus be also applied to its bounding box)
		float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
	);
	void processInput(GLFWwindow* window);
	void createSphereVertices(float radius, int sectorCount, int stackCount);
	void setUpImGui(GLFWwindow* window);
	void FrameBufferCallback(GLFWwindow* window, int width, int height);
	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	double lastX = 1670;
	double lastY = 900;
	float YAW =  -90.0f;
	float PITCH = 0.0f;	
	
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 lightPositions[] = {
	glm::vec3(-10.0f,  10.0f, 10.0f),
	glm::vec3(10.0f,  10.0f, 10.0f),
	glm::vec3(-10.0f, -10.0f, 10.0f),
	glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;	


	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<int> lineIndices;
	
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
		glViewport(0, 0, 1670., 900.);
		glEnable(GL_MULTISAMPLE);


	//	glfwSetCursorPosCallback(WindowProperties::window, cursorPosCallback);

		glfwSetFramebufferSizeCallback(WindowProperties::window, FrameBufferCallback);
		glfwSetKeyCallback(WindowProperties::window, key_callback);
		//glEnable(GL_BLEND);
		    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

		glfwSetInputMode(props.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		ResourceManager::LoadShader("default",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Defaultvs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Defaultfs.glsl"
		);
		ResourceManager::LoadShader("frame",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Framevs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Framefs.glsl"
		);
		ResourceManager::LoadShader("pbr"
			, "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Pbrntvs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Pbrntfs.glsl");

		ResourceManager::LoadShader("cubemap"
			, "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Cubemapvs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Cubemapfs.glsl");
		ResourceManager::LoadShader("eq"
			, "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Eqvs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Eqfs.glsl");
		ResourceManager::LoadShader("irradiance"
			, "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Irradiancevs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Irradiancefs.glsl");
		ResourceManager::LoadShader("brdf"
			, "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Brdfvs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Brdffs.glsl");
		ResourceManager::LoadShader("prefilter"
			, "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Prefiltervs.glsl",
			"C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Shaders\\Prefilterfs.glsl");
		Shader defaultShader = ResourceManager::GetShader("default");

		ResourceManager::LoadTexture("albedo", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\albedo.png");
		ResourceManager::LoadTexture("metallic", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\metallic.png");
		ResourceManager::LoadTexture("roughness", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\roughness.png");
		ResourceManager::LoadTexture("ao", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\ao.png");
		ResourceManager::LoadTexture("normal", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\metal\\normal.png");
		
		ResourceManager::LoadTexture("albedof", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\albedo2.png");
		ResourceManager::LoadTexture("metallicf", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\metallic2.png");
		ResourceManager::LoadTexture("roughnessf", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\roughness2.png");
		ResourceManager::LoadTexture("aof", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\ao.png");

		ResourceManager::LoadTexture("normalf", "C:\\Users\\yigit\\source\\repos\\SimpleBox\\SimpleBox\\src\\Textures\\floor\\normal2.png");

		Shader pbrShader = ResourceManager::GetShader("pbr");
		pbrShader.Use();
	
		pbrShader.SetInt1("albedoMap1", 0);
		pbrShader.SetInt1("normalMap1", 1);
		pbrShader.SetInt1("metallicMap1", 2);
		pbrShader.SetInt1("roughnessMap1", 3);
		pbrShader.SetInt1("aoMap1", 4);
		

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
			-1.0f	, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1670, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1670, 900); // use a single renderbuffer object for both a depth AND stencil buffer.
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
		float mettalic = 0.8f;

		// GET SHADERS
		Shader defaultShader = ResourceManager::GetShader("default");
		Shader screenShader = ResourceManager::GetShader("frame");
		Shader pbrShader = ResourceManager::GetShader("pbr");
		Shader cubemapShader = ResourceManager::GetShader("cubemap");
		Shader irradianceShader = ResourceManager::GetShader("irradiance");
		Shader prefilterShader = ResourceManager::GetShader("prefilter");
		Shader brdfShader = ResourceManager::GetShader("brdf");
		Shader eq = ResourceManager::GetShader("eq");

		float total = 0;
		bool  setInput = true;
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glm::mat4 lightModel  = glm::mat4(1.0f);


		unsigned int captureFBO;
		unsigned int captureRBO;
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


		stbi_set_flip_vertically_on_load(true);
		int width, height, nrComponents;
		float* data = stbi_loadf("./src/Textures/cubemap/Mt-Washington-Gold-Room_Ref.hdr", &width, &height, &nrComponents, 0);
		unsigned int hdrTexture;
		if (data)
		{
			glGenTextures(1, &hdrTexture);
			glBindTexture(GL_TEXTURE_2D, hdrTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

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

		// pbr: setup cubemap to render to and attach to framebuffer
		// ---------------------------------------------------------
		unsigned int envCubemap;
		glGenTextures(1, &envCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
		// ----------------------------------------------------------------------------------------------
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		// pbr: convert HDR equirectangular environment map to cubemap equivalent
		// ----------------------------------------------------------------------
		eq.Use();
		eq.SetInt1("equirectangularMap", 0);
		eq.SetMat4("projection", captureProjection);
		//equirectangularToCubemapShader.setInt("equirectangularMap", 0);
		//equirectangularToCubemapShader.setMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);

		glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			eq.SetMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		unsigned int irradianceMap;
		glGenTextures(1, &irradianceMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
		// -----------------------------------------------------------------------------
		irradianceShader.Use();
		irradianceShader.SetInt1("environmentMap", 0);
		irradianceShader.SetMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

		glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			irradianceShader.SetMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//renderCube();
			
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
		// --------------------------------------------------------------------------------
		unsigned int prefilterMap;
		glGenTextures(1, &prefilterMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
		// ----------------------------------------------------------------------------------------------------
		prefilterShader.Use();
		prefilterShader.SetInt1("environmentMap", 0);
		prefilterShader.SetMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader.SetFloat1("roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				prefilterShader.SetMat4("view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// pbr: generate a 2D LUT from the BRDF equations used.
		// ----------------------------------------------------
		unsigned int brdfLUTTexture;
		glGenTextures(1, &brdfLUTTexture);

		// pre-allocate enough memory for the LUT texture.
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
		// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

		glViewport(0, 0, 512, 512);
		brdfShader.Use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0,1670, 900); // don't forget to configure the viewport to the capture dimensions.
		float distance = 3.0f;
		while (!glfwWindowShouldClose(WindowProperties::window))
		{
			

		
			processInput(WindowProperties::window);
			if (glfwGetKey(WindowProperties::window, GLFW_KEY_P))
			{
				
	}
			
			YAW = -90.0f;
			YAW -= rotate1;
			


			glm::vec3 direction;
			direction.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
			direction.y = sin(glm::radians(PITCH));
			direction.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
						cameraFront = glm::normalize(direction);
			cameraPos.z = distance*glm::cos(glm::radians(rotate1));
		cameraPos.x = distance* glm::sin(glm::radians(rotate1));
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			projection = glm::perspective(glm::radians(45.f), (WindowProperties::Width - 250.f) / WindowProperties::Height, 0.1f, 100.0f);
			double y = 0;
			double x = 0;	
			glfwGetCursorPos(WindowProperties::window, &x, &y);
			
			// std::cout << "X :"<<x << " Y: " <<y << ", ";
			//mousePicker.Update(view, projection,x, y);
		
			//Fps counter
			timer.currentTime = glfwGetTime();
			timer.deltaTime = timer.currentTime - timer.lastTime;
			timer.lastTime = timer.currentTime;
			total += timer.deltaTime;
			if (total > 1.0f) {
				total = 0.0f;
				//std::cout << mousePicker.GetCurrentRay().x << " " << mousePicker.GetCurrentRay().y << " " << mousePicker.GetCurrentRay().z << "," << std::endl;	// std::cout << 1. / timer.deltaTime << '\n';
			}
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
			glClearColor(0.0f, 0.0, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			
			glm::mat4 model = glm::mat4(1.0f);
			//odel = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
			//model = glm::translate(model, glm::vec3(0.0f, 0.f,-distance));
			pbrShader.Use();
	
			pbrShader.SetMat4("projection", projection);
			pbrShader.SetMat4("view", view);
			pbrShader.SetMat4("model", model);
			pbrShader.SetFloat3("camPos", cameraPos);		
			pbrShader.SetFloat3("albedo", glm::vec3(colors1[0],colors1[1],colors1[2]));
			pbrShader.SetFloat1("ao", 1.0f);
		
		
		
			pbrShader.SetInt1("irradianceMap", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
			pbrShader.SetInt1("prefilterMap", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
			pbrShader.SetInt1("brdfLUT", 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
			glBindVertexArray(VAOS);


			pbrShader.SetFloat1("metallic", mettalic);
			pbrShader.SetFloat1("roughness", roughness);
			pbrShader.SetMat4("model", model);

			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
				glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
				newPos = lightPositions[i];
				pbrShader.SetFloat3(("lightPosition[" + std::to_string(i) + "]").c_str(), newPos);
				pbrShader.SetFloat3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);

			
			}


			glBindVertexArray(VAOS);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			glDrawElements(GL_TRIANGLE_STRIP, 100 * 100 * 10, GL_UNSIGNED_INT, 0);
		/*
			for (int row = 0; row < nrRows; ++row)
			{
				pbrShader.SetFloat1("metallic", (float)row / (float)nrRows);
				for (int col = 0; col < nrColumns; ++col)
				{
					// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
					// on direct lighting.
				pbrShader.SetFloat1("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(
						(float)(col - (nrColumns / 2)) * spacing,
						(float)(row - (nrRows / 2)) * spacing,
						-2.0f
					));
					pbrShader.SetMat4("model", model);
					glBindVertexArray(VAOS);
					//glDrawArrays(GL_TRIANGLES, 0, 36);
					glDrawElements(GL_TRIANGLE_STRIP, 100 * 100 * 10, GL_UNSIGNED_INT, 0);
				}
			}


			// render light source (simply re-render sphere at light positions)
			// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
			// keeps the codeprint small.
			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
				glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
				newPos = lightPositions[i];
				pbrShader.SetFloat3(("lightPosition[" + std::to_string(i) + "]").c_str(), newPos);
				pbrShader.SetFloat3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);

				model = glm::mat4(1.0f);
				model = glm::translate(model, newPos);
				model = glm::scale(model, glm::vec3(0.5f));
				pbrShader.SetMat4("model", model);
				glBindVertexArray(VAOS);
				//glDrawArrays(GL_TRIANGLES, 0, 36);
				glDrawElements(GL_TRIANGLE_STRIP, 100 * 100 * 10, GL_UNSIGNED_INT, 0);
			}


			*/



			cubemapShader.Use();
			cubemapShader.SetMat4("view", view);
			cubemapShader.SetMat4("projection", projection);
			cubemapShader.SetInt1("environmentMap", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//std::cout << glGetError() << '\n';
		
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		
			
			//glDrawElements(GL_TRIANGLE_STRIP, 100 * 100 * 10, GL_UNSIGNED_INT, 0);
			
		

			
			float inter;

			float intersection_distance; // Output of TestRayOBBIntersection()
			glm::vec3 aabb_min(-1.f, -1.f, -1.);
			glm::vec3 aabb_max(1.f, 1., 1.f);

			// The ModelMatrix transforms :
			// - the mesh to its desired position and orientation
			// - but also the AABB (defined with aabb_min and aabb_max) into an OBB
		
		

			if (TestRayOBBIntersection(
				cameraPos,
				mousePicker.GetCurrentRay(),
				
				aabb_min,
				aabb_max,
				model,
				glm::vec3(10.0f, 0.2f, 10.0f),
				intersection_distance)
				) {
				//std::cout << "1";
			}

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glBlitFramebuffer(0, 0, WindowProperties::Width-250, WindowProperties::Height, 0, 0,WindowProperties::Width-250., WindowProperties::Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
			// clear all relevant buffers
			glClearColor(1.0f, 1.f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			screenShader.Use();
			screenShader.SetFloat1("offset", glfwGetTime()*10);
			glBindVertexArray(quadVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
			glDrawArrays(GL_TRIANGLES, 0, 6);

			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
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
		
			ImGui::SliderFloat("Distance", &distance, 3.0f, 10.0f, "%f meters", 1.0f);
			ImGui::SliderFloat("Camera Rot", &rotate1, 0.0f, 360.0f, "%f degrees", 1.0f);
		

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
	bool TestRayOBBIntersection(
		glm::vec3 ray_origin,        // Ray origin, in world space
		glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
		glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
		glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
		glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
		glm::vec3 ModelMatrix1,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
		float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
	) {

		
		// Intersection method from Real-Time Rendering and Essential Mathematics for Games

		float tMin = 0.0f;
		float tMax = 100000.0f;

		glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

		glm::vec3 delta = OBBposition_worldspace - ray_origin;

		// Test intersection with the 2 planes perpendicular to the OBB's X axis
		{
			glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
			float e = glm::dot(xaxis, delta);
			float f = glm::dot(ray_direction, xaxis);

			if (fabs(f) > 0.001f) { // Standard case

				float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
				float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
				// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

				// We want t1 to represent the nearest intersection, 
				// so if it's not the case, invert t1 and t2
				if (t1 > t2) {
					float w = t1; t1 = t2; t2 = w; // swap t1 and t2
				}

				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
				if (t2 < tMax)
					tMax = t2;
				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
				if (t1 > tMin)
					tMin = t1;

				// And here's the trick :
				// If "far" is closer than "near", then there is NO intersection.
				// See the images in the tutorials for the visual explanation.
				if (tMax < tMin)
					return false;

			}
			else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
				if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
					return false;
			}
		}


		// Test intersection with the 2 planes perpendicular to the OBB's Y axis
		// Exactly the same thing than above.
		{
			glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
			float e = glm::dot(yaxis, delta);
			float f = glm::dot(ray_direction, yaxis);

			if (fabs(f) > 0.001f) {

				float t1 = (e + aabb_min.y) / f;
				float t2 = (e + aabb_max.y) / f;

				if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

				if (t2 < tMax)
					tMax = t2;
				if (t1 > tMin)
					tMin = t1;
				if (tMin > tMax)
					return false;

			}
			else {
				if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
					return false;
			}
		}


		// Test intersection with the 2 planes perpendicular to the OBB's Z axis
		// Exactly the same thing than above.
		{
			glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
			float e = glm::dot(zaxis, delta);
			float f = glm::dot(ray_direction, zaxis);

			if (fabs(f) > 0.001f) {

				float t1 = (e + aabb_min.z) / f;
				float t2 = (e + aabb_max.z) / f;

				if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

				if (t2 < tMax)
					tMax = t2;
				if (t1 > tMin)
					tMin = t1;
				if (tMin > tMax)
					return false;

			}
			else {
				if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
					return false;
			}
		}

		intersection_distance = tMin;
		return true;

	}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			if (!input)
			{
				glfwSetInputMode(WindowProperties::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				firstMouse = true;
				input = !input;
			}
			else
			{

				glfwSetInputMode(WindowProperties::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				input = !input;
			}

		}
	}
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
	};
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


	};
	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (input) {
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
	};
	void FrameBufferCallback(GLFWwindow* window, int width, int height)
	{
		mousePicker.SetDims(width, height);
		WindowProperties::Height = (float)height;
		WindowProperties::Width = (float)width;
		std::cout << WindowProperties::Height << std::endl;
		std::cout << WindowProperties::Width << std::endl;
		glViewport(0., 0., width - 250, height);

		glDeleteFramebuffers(1, &framebuffer);
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture

		//glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width - 250, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

		//glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width - 250, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//	cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);



	}
		
};