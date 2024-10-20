#include "logger.h"
#include "Window.h"

#include <GL/glew.h>

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Renderer.h"
#include "InputSystem.h"

int main(int argc, char* argv[])
{
	//Engine::WindowsWindow window;
	Engine::Window* window = Engine::Window::GenerateWindow();

	window->Create(640, 480, "Paintball Splatter");
	window->CreateOpenGLContext();

	//if (glewInit() != GLEW_OK) std::cout << "GLEW Error!" << std::endl;
	PS_ASSERT(glewInit() == GLEW_OK, "GLEW Error!");

	OpenGLAttribs* attributes = new OpenGLAttribs();
	attributes->SetWindowHint(WINDOW_OPENGL_PROFILE, WINDOW_OPENGL_V_CORE_PROFILE);
	attributes->SetWindowHint(WINDOW_OPENGL_VERSION_MAJOR, 3);
	attributes->SetWindowHint(WINDOW_OPENGL_VERSION_MINOR, 3);

	window->CreateOpenGLExtensions(nullptr);

	//delete attributes;

	//PS_LOGGER_INFO(glGetString(GL_VERSION));
	PS_LOGGER_INFO("Vendor: "		<< glGetString(GL_VENDOR));
	PS_LOGGER_INFO("Renderer: "		<< glGetString(GL_RENDERER));
	PS_LOGGER_INFO("Version: "		<< glGetString(GL_VERSION));
	PS_LOGGER_INFO("GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION));
	PS_LOGGER_INFO("Profile: "		<< (attributes->GetWindowHint(WINDOW_OPENGL_PROFILE) == 1 ? "WINDOW_OPENGL_V_CORE_PROFILE" : attributes->GetWindowHint(WINDOW_OPENGL_PROFILE) == 2 ? "WINDOW_OPENGL_V_COMPATIBILITY_PROFILE" : "Unknown"));
	//PS_LOGGER_INFO("Extensions: "	<< glGetString(GL_EXTENSIONS));

	delete attributes;

	Renderer::Initialize();

	while (!window->WindowShouldClose())
	{
		window->UpdateWindow();
		window->PollEvents();

		/*float positions[] =
		{
			-50.5f, -50.5f, 0.0f, 0.0f, // 0
			 50.5f, -50.5f, 1.0f, 0.0f, // 1
			 50.5f,  50.5f, 1.0f, 1.0f, // 2
			-50.5f,  50.5f, 0.0f, 1.0f  // 3
		};*/
		float positions[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};

		unsigned int indices[] = 
		{ 
			0, 1, 2//,
			//2, 3, 0
		};

		Renderer::SetViewport(640, 480);
		Renderer::ClearColor(0.3f, 0.2f, 0.8f, 1.0f);
		Renderer::Clear();

		/*VertexBuffer vbo(positions, sizeof(positions), GL_DYNAMIC_DRAW);

		vbo.LayoutPush<float>(0, 3, 3 * sizeof(float), 0);
		vbo.FinalizeLayout();

		IndexBuffer ibo(indices, 3);
		ibo.Bind();*/

		VertexBuffer vbo1(GL_DYNAMIC_DRAW);

		Vertex v0(-1.0f, -1.0f, -0.5f);
		Vertex v1(-0.5f, 0.0f, -0.5f);
		Vertex v2(0.0f, -1.0f, -0.5f);
		vbo1.PushVertex(v0);
		vbo1.PushVertex(v1);
		vbo1.PushVertex(v2);
		vbo1.FinalizeVertices();

		vbo1.PushLayout<float>(0, 3, 3 * sizeof(float), 0);
		vbo1.FinalizeLayout();

		VertexBuffer vbo2(GL_DYNAMIC_DRAW);
		Vertex v3(0.0f, 0.0f, 0.5f);
		Vertex v4(0.5f, 1.0f, 0.5f);
		Vertex v5(1.0f, 0.0f, 0.5f);
		vbo2.PushVertex(v3);
		vbo2.PushVertex(v4);
		vbo2.PushVertex(v5);
		vbo2.FinalizeVertices();

		vbo2.PushLayout<float>(0, 3, 3 * sizeof(float), 0);
		vbo2.FinalizeLayout();

		IndexBuffer ibo1(indices, 3);
		IndexBuffer ibo2(indices, 3);

		std::string vs = "#version 330 core\n layout (location = 0) in vec4 position;\n uniform mat4 u_MVP;\n		void main()\n {\n 	gl_Position = u_MVP * position;\n				};\n";
		std::string fs = "#version 330 core\n layout (location = 0) out vec4 color;\n								void main()\n {\n 	color = vec4(1.0, 0.0, 0.0, 1.0);\n				};\n";
		Shader shader(vs, fs);
		shader.Bind();
		
		Math::Matrix4x4 model = glm::translate(Math::Matrix4x4(1.0f), Math::Vector3(0.0f, 0.0f, 0.0f));

		Renderer::Render(vbo1, ibo1, shader, model);
		Renderer::Render(vbo2, ibo2, shader, model);

		Math::Matrix4x4 proj = Math::Ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		Math::Matrix4x4 view = glm::translate(Math::Matrix4x4(1.0f), Math::Vector3(0, 0, 0));

		Renderer::SetProjectionMatrix(Math::Ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f));
		Renderer::SetViewMatrix(glm::translate(Math::Matrix4x4(1.0f), Math::Vector3(0, 0, 0)));

		Math::Matrix4x4 mvp = proj * view * model;

		if (Input::GetKeyPress(KeyState::KeyDown, PS_KEY_W))
		{
			PS_LOGGER_LOG("w was pressed");
		}
		if (Input::GetKeyPress(KeyState::KeyDown, PS_KEY_S))
		{
			PS_LOGGER_LOG("s has been pressed");
		}
		if (Input::GetKeyPress(KeyState::KeyHeld, PS_KEY_S))
		{
			PS_LOGGER_LOG("s is being held");
		}
		if (Input::GetKeyPress(KeyState::KeyUp, PS_KEY_D))
			PS_LOGGER_LOG("d was released");

		Input::ClearKeyPresses();

		window->SwapBuffers();
	}

	window->TerminateOpenGLContext();
	window->Terminate();

	return 0;
}