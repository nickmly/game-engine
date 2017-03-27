#pragma once

class Renderer 
{
public:
	virtual void Enable() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual void Destroy() = 0;
};