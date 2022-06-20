#pragma once
#include "event.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window : public Sender
{
public:
	uint m_width, m_height;
	GLFWwindow* m_win;
	static Window* s_win;
	
	Window(uint, uint, const char*);
	~Window();
	void pollInput();
	void clear();
	void swapBuffers();
	bool isOpen();
};

// Window events //////////////////////////////////////////////////////////////////////////////////

struct WinCloseEvent : public Event { EVENT_ID(WIN_CLOSE) };

struct WinResizeEvent : public Event
{
	unsigned int width, height;
	WinResizeEvent(unsigned int _width, unsigned int _height)
		: width(_width), height(_height) {}
	EVENT_ID(WIN_RESIZE)
};

struct KeyEvent : public Event
{
	enum Action { RELEASE, PRESS};
	int key, scancode, action, mods;
	KeyEvent(int _key, int _scancode, int _action, int _mods)
		: key(_key), scancode(_scancode), action(_action), mods(_mods) {}
	EVENT_ID(KEY)
};

struct CharEvent : public Event
{
	int c;
	CharEvent(int _c) : c(_c) {}
	EVENT_ID(CHAR)
};

struct MouseButtonEvent : public Event
{
	enum Action { RELEASE, PRESS};
	int button, action, mods;
	MouseButtonEvent(int _button, int _action, int _mods) : button(_button), action(_action), mods(_mods) {}
	EVENT_ID(MOUSE_BUTTON)
};

struct ScrollEvent : public Event
{
	float xoffset, yoffset;
	ScrollEvent(float _xoffset, float _yoffset) : xoffset(_xoffset), yoffset(_yoffset) {}
	EVENT_ID(SCROLL)
};

struct CursorEvent : public Event
{
	float xpos, ypos;
	CursorEvent(float _xpos, float _ypos) : xpos(_xpos), ypos(_ypos) {}
	EVENT_ID(CURSOR)
};
