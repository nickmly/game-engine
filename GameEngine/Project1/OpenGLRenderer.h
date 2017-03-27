#pragma once
#include "Renderer.h"

class OpenGLRenderer : public Renderer
{
public:
	virtual void Enable() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void Destroy() override;
	OpenGLRenderer();
	~OpenGLRenderer();
};

