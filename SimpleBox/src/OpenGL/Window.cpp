#include "Window.h"
#include <vector>


namespace Engine{


	bool firstMouse = true;
	double lastX;
	double lastY;

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
		Shader defaultShader = ResourceManager::GetShader("default");

	
	}

	void Window::GameLoop()
	{
		
		// ------------------------------------------------------------------
		
		float vertices[] = {
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(1);
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
		


		glm::mat4 view = glm::lookAt(glm::vec3(0.0f,2.0f,5.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0., 1., 0.));
		glm::mat4 projection;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0., 1., 0.));
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
		
		// GET SHADERS
		Shader defaultShader = ResourceManager::GetShader("default");
		Shader screenShader = ResourceManager::GetShader("frame");
		
		
		
		while (!glfwWindowShouldClose(props.window))
		{
			//Fps counter
			timer.currentTime = glfwGetTime();
			timer.deltaTime = timer.currentTime - timer.lastTime;
			timer.lastTime = timer.currentTime;
			//If you want to display fps -> std::cout << 1. / timer.deltaTime;
			
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
			glClearColor(0.7f,0.7, 0.7f, 0.7f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glm::mat4 lightModel = glm::mat4(1.0f);
			lightModel = glm::rotate(lightModel, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
			lightModel = glm::translate(lightModel, glm::vec3(0.0f, 2.0f, -5.0f));

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
			defaultShader.SetFloat3("spec_color", glm::vec3(specular[0], specular[1], specular[2]));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
			// clear all relevant buffers
			glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT);

			screenShader.Use();
			glBindVertexArray(quadVAO);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
			//glDrawArrays(GL_TRIANGLES, 0, 6);


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::SetNextWindowPos({ 0.0,0.0 });
			ImGui::SetNextWindowSize({ 200.,600. });
			

			ImGui::Begin("Demo  window", &open,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			ImGui::PushFont(font1);
			ImGui::Text("Object Color");
			ImGui::ColorPicker4("Color", colors1);
			//ImGui::Spacing();
			ImGui::Text("Light Rotation");
			
			ImGui::SliderFloat("degrees", &rotate, 0.0f, 360.0f, "%f",1.0f);
			//ImGui::Spacing();
			ImGui::Text("Specular Power");
			ImGui::InputFloat("", &specPow, 0.0f, 0.0f, "%f", 0);
			
			
		
			ImGui::Spacing();
			ImGui::Text("Ambient Power");
			ImGui::InputFloat("a", &ambientStrength);
			ImGui::Spacing();
			ImGui::Text("Specular Strength");
			ImGui::InputFloat("s", &specStrength);
			//ImGui::EndChild();

			ImGui::BeginChild("Colors");
			ImGui::Spacing();

			ImGui::Text("Ambient Light Color");
			ImGui::ColorEdit3("ambient", ambient,0 );
			ImGui::Text("Diffuse Light Color");
			ImGui::ColorEdit3("diffuse", diffuse, 0);
			ImGui::Text("Specular Light Color");
			ImGui::ColorEdit3("specular", specular, 0);
			ImGui::EndChild();
			ImGui::PopFont();
			ImGui::End();
			ImGui::SetNextWindowPos({ 200.0,0.0 });
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
