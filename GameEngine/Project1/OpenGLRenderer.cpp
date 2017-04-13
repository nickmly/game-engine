#include "OpenGLRenderer.h"
#include "GL\glew.h"


void OpenGLRenderer::Enable()
{
	glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::PreRender()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Render()
{

}

void OpenGLRenderer::PostRender()
{

}

void OpenGLRenderer::Destroy()
{

}

OpenGLRenderer::OpenGLRenderer()
{

}


OpenGLRenderer::~OpenGLRenderer()
{

}
