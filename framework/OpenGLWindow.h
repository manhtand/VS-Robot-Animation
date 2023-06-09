#ifndef _GAME_WINDOW_H_
#define _GAME_WINDOW_H_
#include <libheaders.h>
#include <glerror.h>
#include <Input.h>
#include <memory>
#include <iostream>
#include <fw_config.h>
#include <fstream>



class OpenGLWindow : public InputHandler
{
public:
	//ctors/dtor
	//Don't copy!
	OpenGLWindow(const OpenGLWindow&) = delete;
	//Don't move!
	OpenGLWindow(OpenGLWindow&&) = delete;
	OpenGLWindow(const GLint width, const GLint height, bool fullscreen, bool vsync, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLint msaasamples = 0, const GLboolean uselatestglver = false, GLdouble updatefrequency = 120.0);
	virtual ~OpenGLWindow();

	//public interface
	GLvoid run();	//starts game loop
	GLvoid quit();	//stops the game loop and closes the application

	//override these in your application
	virtual GLvoid update(GLdouble dtime) {};	//physics and logic updates here
	virtual GLvoid render(GLdouble dtime) {};	//rendering a frame goes here
	virtual GLvoid init() {};					//initialization tasks
	virtual GLvoid shutdown() {};				//clean everything up

	//override these to i.e. call glViewport on a window resize
	virtual void onWindowResize(int width, int height) {};
	virtual void onFrameBufferResize(int width, int height) {};

	//empty input event handlers, override if needed
	virtual void onKey(Key key, Action action, Modifier modifier) override {};
	virtual void onMouseMove(MousePosition mouseposition) override {};
	virtual void onMouseButton(MouseButton button, Action action, Modifier modifier) override {};
	virtual void onMouseScroll(double xscroll, double yscroll) override {};

	//getters for window width/height
	GLint getFrameBufferWidth() const
	{
		return fbWidth;
	}

	GLint getFrameBufferHeight() const
	{
		return fbHeight;
	}

	GLint getWindowWidth() const
	{
		return windowWidth;
	}

	GLint getWindowHeight() const
	{
		return windowHeight;
	}

	Input& getInput();

	double getCurrentTime();

	void setCursorVisible(bool visible);

protected:
	//window width/height
	GLint windowWidth;
	GLint windowHeight;
	GLint fbWidth;
	GLint fbHeight;
	//window title
	std::string title;
	bool fullscreen;
	bool vsync;
	//access this to query input states
	std::unique_ptr<Input> input;
private:
	GLboolean initialize();
	GLboolean m_uselatestglcontext;
	GLint m_cvmaj;
	GLint m_cvmin;
	GLint m_samples;
	GLdouble m_updatefrequency;
	GLFWwindow* m_window;
	static OpenGLWindow* windowHandlerInstance;
	double m_currentTime;
	double m_startTime;

	struct PerfRecord
	{
		double avgframetime;
		double maxframetime;
		double avgfps;
		double minfps;
	};

	std::vector<PerfRecord> m_perlog;

	double movingAvg(double mn, size_t n, double ne);

	//glfw callback wrappers
	static void wrz_callback(GLFWwindow* window, int width, int height)
	{
		windowHandlerInstance->windowWidth = width;
		windowHandlerInstance->windowHeight = height;
		windowHandlerInstance->onWindowResize(width, height);
	}
	static void frz_callback(GLFWwindow* window, int width, int height)
	{
		windowHandlerInstance->fbWidth = width;
		windowHandlerInstance->fbHeight = height;
		windowHandlerInstance->onFrameBufferResize(width, height);
	}
};

#endif
