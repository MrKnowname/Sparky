#include "window.h"

namespace sparky {
	namespace graphics {
		void window_resize(GLFWwindow *window, int width, int height);

		Window::Window(const char *title, int width, int height) {
			m_title = title;
			m_width = width;
			m_height = height;
			if (!init()) glfwTerminate();

			for (int i = 0; i < MAX_KEYS; i++) m_keys[i] = false;
			for (int i = 0; i < MAX_BUTTONS; i++) m_mouseButtons[i] = false;
		}

		Window::~Window() {
			glfwTerminate();
		}

		bool Window::init() {
			if (!glfwInit()) {
				LOG("Failed to initialise GLFW");
				return false;
			}

			m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
			if (!m_window) {
				LOG("Failed to create GLFW window");
				return false;
			}
			glfwMakeContextCurrent(m_window);
			glfwSetWindowUserPointer(m_window, this);
			glfwSetWindowSizeCallback(m_window, window_resize);
			glfwSetMouseButtonCallback(m_window, mouse_button_callback);
			glfwSetCursorPosCallback(m_window, cursor_position_callback);
			glfwSetKeyCallback(m_window, key_callback);

			if (glewInit() != GLEW_OK) {
				LOG("Failed to initialise GLEW");
				return false;
			}
			LOG("OpenGL " << glGetString(GL_VERSION));

			return true;
		}

		bool Window::isKeyPressed(unsigned int keycode) const {
			if (keycode >= MAX_KEYS) return false;
			return m_keys[keycode];
		}

		bool Window::isMouseButtonPressed(unsigned int button) const {
			if (button >= MAX_BUTTONS) return false;
			return m_mouseButtons[button];
		}

		void Window::getMousePosition(double &x, double &y) const {
			x = mx;
			y = my;
		}

		void Window::clear() const {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Window::update() {
			glfwPollEvents();
			glfwSwapBuffers(m_window);
		}

		bool Window::closed() const {
			return glfwWindowShouldClose(m_window) == 1;
		}

		void window_resize(GLFWwindow *window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
			Window *win = (Window*) glfwGetWindowUserPointer(window);
			win->m_keys[key] = (action != GLFW_RELEASE);
		}

		void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
			Window *win = (Window*)glfwGetWindowUserPointer(window);
			win->m_mouseButtons[button] = (action != GLFW_RELEASE);
		}

		void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
			Window *win = (Window*)glfwGetWindowUserPointer(window);
			win->mx = xpos;
			win->my = ypos;
		}
	}
}