#pragma once
#include "Prerequisites.h"

class ShaderLibrary {
public:
	static ShaderLibrary* get();

	static void initialize();
	static void destroy();
private:
	ShaderLibrary();
	~ShaderLibrary();

	ShaderLibrary(ShaderLibrary const&);
	ShaderLibrary& operator=(ShaderLibrary const&) {};
	static ShaderLibrary* sharedInstance;
public:
	VertexShaderPtr getVS() { return m_vs; }
	PixelShaderPtr getPS() { return m_ps; }

	void* getVSByteCode() { return vs_byte_code; }
	size_t getVSSize() { return vs_size; }
private:
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;

	void* vs_byte_code = nullptr;
	size_t vs_size = 0;
	void* ps_byte_code = nullptr;
	size_t ps_size = 0;
};

