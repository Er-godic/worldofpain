#include "window.hpp"

Window* Window::s_win;

// callback for debugging opengl
// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDebugMessageControl.xhtml
// https://www.khronos.org/opengl/wiki/Example/OpenGL_Error_Testing_with_Message_Callbacks
void GLAPIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* msg, const void* userParam )
{
	const char* source_str = [source]()
	{
		switch(source)
		{
			case GL_DEBUG_SOURCE_API               : return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM     : return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER   : return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY       : return "THIRD PARTY";   
			case GL_DEBUG_SOURCE_APPLICATION       : return "APPLICATION";    
			case GL_DEBUG_SOURCE_OTHER             : return "OTHER";
			default                                : return "";	 
		}
	}();

	const char* type_str = [type]()
	{
		switch(type)
		{
			case GL_DEBUG_TYPE_ERROR               : return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR : return "DEPRECATED BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  : return "UNDEFINED BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY         : return "PORTABILITY";   
			case GL_DEBUG_TYPE_PERFORMANCE         : return "PERFORMANCE";    
			case GL_DEBUG_TYPE_MARKER              : return "MARKER";         
			case GL_DEBUG_TYPE_PUSH_GROUP          : return "PUSH GROUP";     
			case GL_DEBUG_TYPE_POP_GROUP           : return "POP GROUP";      
			case GL_DEBUG_TYPE_OTHER               : return "OTHER";
			default                                : return "";		 
		}
	}();	

	Log::Msg msgtype = [severity]()
	{
		switch(severity)
		{
			case GL_DEBUG_SEVERITY_NOTIFICATION : return Log::info;
			case GL_DEBUG_SEVERITY_LOW          : return Log::warn;
			case GL_DEBUG_SEVERITY_MEDIUM       : return Log::error;
			case GL_DEBUG_SEVERITY_HIGH         : return Log::critical;
			default                             : return Log::info;
		}
	}();

	const char* output = "** GL {0} , Source : {1} ** \n {2}";
	Log::print(Log::render, msgtype, output, type_str, source_str, msg);
}

Window::Window(uint w, uint h, const char* name)
{
	// init GLFW
	if(!glfwInit())
		Log::print(Log::core, Log::critical, "GLFW could not be initialized");

	// GLFW error callback
	glfwSetErrorCallback([](int err, const char* msg) 
		{ Log::print(Log::core, Log::error, "** GLFW {0} ** \n {1}", err, msg); });

	// GL 4.6 + GLSL 460
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	m_win = glfwCreateWindow(w, h, name, NULL, NULL);
	// make context current, do this before using GLAD
	glfwMakeContextCurrent(m_win);

	// load all OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		Log::print(Log::core, Log::critical, "GLAD could not be initialized");

	// enable OpenGL debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLDebugCallback, 0);

	// store pointer
	glfwSetWindowUserPointer(m_win, reinterpret_cast<void*>(this));

	// event callbacks ///////////////////////////////////////////////////////////////////////////////

	glfwSetWindowCloseCallback(m_win, [](GLFWwindow* window)
		{
			Window* win  = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			win->send<WinCloseEvent>();
		});

	glfwSetWindowSizeCallback(m_win, [](GLFWwindow* window, int width, int height)
		{
			Window* win  = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

			// make sure the viewport matches the new window dimensions
			glViewport(0, 0, width, height);

			win->m_width  = width;
			win->m_height = height;
			win->send<WinResizeEvent>(width, height);
		});

	glfwSetKeyCallback(m_win, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* win  = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			win->send<KeyEvent>(key, scancode, action, mods);
		});

	glfwSetCharCallback(m_win, [](GLFWwindow* window, uint character)
		{
			Window* win  = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			win->send<CharEvent>(character);
		});

	glfwSetMouseButtonCallback(m_win, [](GLFWwindow* window, int button, int action, int mods)
		{
			Window* win  = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			win->send<MouseButtonEvent>(button, action, mods);
		});

	glfwSetScrollCallback(m_win, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			Window* win  = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			win->send<ScrollEvent>(xoffset, yoffset);
		});

	glfwSetCursorPosCallback(m_win, [](GLFWwindow* window, double xpos, double ypos)
		{
			Window* win  = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			win->send<CursorEvent>(xpos, ypos);
		});

	///////////////////////////////////////////////////////////////////////////////////////////////////
}

Window::~Window()
{
	glfwDestroyWindow(m_win);
	glfwTerminate();
}

void Window::pollInput() { glfwPollEvents(); }

void Window::clear()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapBuffers() { glfwSwapBuffers(m_win); }

bool Window::isOpen() { return !glfwWindowShouldClose(m_win); }
