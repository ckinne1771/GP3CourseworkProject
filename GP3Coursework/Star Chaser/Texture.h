#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL\glew.h"


#include "SDL2-devel-2.0.3-VC\SDL2-2.0.3\include\SDL.h"
#include "SDL2-devel-2.0.3-VC\SDL2-2.0.3\include\SDL_opengl.h"
#include "SDL2_image-devel-2.0.0-VC\SDL2_image-2.0.0\include\SDL_image.h"
#include "SDL2_ttf-devel-2.0.12-VC\SDL2_ttf-2.0.12\include\SDL_ttf.h"



#include <iostream>
#include <string>

GLuint convertSDLSurfaceToGLTexture(SDL_Surface * surface);

GLuint loadTextureFromFile(const std::string& filename);
GLuint loadTextureFromFont(const std::string& fontFilename, int pointSize, const std::string& text);

void loadCubeMapSide(const std::string& filename, GLenum cubeSide);

#endif