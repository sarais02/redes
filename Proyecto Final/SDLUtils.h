#ifndef SDLUTILS_H
#define SDLUTILS_H

#include <string>
#include <map>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Singleton.h"
#include "RandomNumberGenerator.h"
#include "macros.h"

class SDLUtils: public Singleton<SDLUtils> {

	friend Singleton<SDLUtils> ; // needed to give access to private constructors

public:

	virtual ~SDLUtils();

	// cannot copy/move
	SDLUtils(SDLUtils&) = delete;
	SDLUtils(SDLUtils&&) = delete;
	SDLUtils& operator=(SDLUtils&) = delete;
	SDLUtils& operator=(SDLUtils&&) = delete;

	// access to the underlying SDL_Window -- in principle not needed
	inline SDL_Window* window() {
		return window_;
	}

	// access to the underlying SDL_Renderer -- needed when creating textures
	// other than those initialized in this class
	inline SDL_Renderer* renderer() {
		return renderer_;
	}

	// clear the renderer with a given SDL_Color
	inline void clearRenderer(SDL_Color bg = build_sdlcolor(0xFFFFFFF)) {
		SDL_SetRenderDrawColor(renderer_, COLOREXP(bg));
		SDL_RenderClear(renderer_);
	}

	// present the current content of the renderer
	inline void presentRenderer() {
		// std::cout << "SDL Presenting:\n";
		// std::cout << renderer_ <<   "\n";
		SDL_RenderPresent(renderer_);
	}

	// the window's width
	inline int width() {
		return width_;
	}

	// the window's height
	inline int height() {
		return height_;
	}

	// toggle to full-screen/window mode
	inline void toggleFullScreen() {
		auto flags = SDL_GetWindowFlags(window_);
		if (flags & SDL_WINDOW_FULLSCREEN) {
			SDL_SetWindowFullscreen(window_, 0);
		} else {
			SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
		}
	}

	// show the cursor when mouse is over the window
	inline void showCursor() {
		SDL_ShowCursor(1);
	}

	// hide the cursor when mouse is over the window
	inline void hideCursor() {
		SDL_ShowCursor(0);
	}

	// All resource maps can be modified from outside, this way you can store
	// your own dynamically. Be careful when modifying them!

	// Access to the random number generator. It is important to always
	// use this generator, this way you can regenerate the same sequence
	// if you start from the same seed
	inline RandomNumberGenerator& rand() {
		return random_;
	}

	// Access to real time
	inline Uint32 currRealTime() const {
		return SDL_GetTicks();
	}
	void closeWindow();

private:
	SDLUtils();
	SDLUtils(std::string windowTitle, int width, int height);

	void initWindow();
	
	void initSDLExtensions(); // initialize resources (fonts, textures, audio, etc.)
	void closeSDLExtensions(); // free resources the

	std::string windowTitle_; // window title
	int width_; // window width
	int height_; // window height

	SDL_Window *window_; // the window
	SDL_Renderer *renderer_; // the renderer

	RandomNumberGenerator random_; // (pseudo) random numbers generator
};
#endif

// This macro defines a compact way for using the singleton Environment, instead of
// writing Environment::instance()->method() we write Environment().method()
//
inline SDLUtils& sdlutils() {
	return *SDLUtils::instance();
}