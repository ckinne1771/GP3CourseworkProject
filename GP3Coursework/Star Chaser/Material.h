#ifndef Material_h
#define Material_h


#include "GL\glew.h"

#include "glm\glm.hpp"
using glm::vec4;

#include <string>
#include "SDL2-devel-2.0.3-VC\SDL2-2.0.3\include\SDL.h"
#include "SDL2-devel-2.0.3-VC\SDL2-2.0.3\include\SDL_opengl.h"

#include "gl\GLU.h"


#include "Component.h"

class BaseMaterial :public Component{

public:

	BaseMaterial()
	{
		m_Type = "BaseMaterial";
	};

	virtual ~BaseMaterial(){};

	virtual void bind(){};
	virtual void unbind(){};

	bool loadShader(const std::string& vsFilename, const std::string& fsFilename);
	GLint getUniformLocation(const std::string& name);

private:
protected:
	GLuint m_ShaderProgram;

};

class Material :public BaseMaterial{
public:
	Material();
	~Material();

	void destroy();

	void bind();

	bool loadShader(const std::string& vsFilename, const std::string& fsFilename);
	GLint getUniformLocation(const std::string& name);

	vec4& getAmbientColour();
	void setAmbientColour(float r, float g, float b, float a);

	vec4& getDiffuseColour();
	void setDiffuseColour(float r, float g, float b, float a);

	vec4& getSpecularColour();
	void setSpecularColour(float r, float g, float b, float a);

	float getSpecularPower();
	void setSpecularPower(float power);

	void loadDiffuseMap(const std::string& filename);
	GLuint getDiffuseMap();

	void loadSpecularMap(const std::string& filename);
	GLuint getSpecularMap();

	void loadBumpMap(const std::string& filename);
	GLuint getBumpMap();

	void loadHeightMap(const std::string& filename);
	GLuint getHeightMap();
protected:
private:
	GLuint m_ShaderProgram;
	vec4 m_AmbientColour;
	vec4 m_DiffuseColour;
	vec4 m_SpecularColour;
	float m_SpecularPower;
	GLuint m_DiffuseMap;
	GLuint m_SpecularMap;
	GLuint m_BumpMap;
	GLuint m_HeightMap;
};

#endif
