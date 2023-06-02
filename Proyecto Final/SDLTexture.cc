#include "SDLTexture.h"
#include <SDL2/SDL_image.h>

#include <iostream>
using namespace std;

SDLTexture::SDLTexture() : texture_(nullptr), renderer_(nullptr), width_(0), height_(0),position(0,0) {
}

SDLTexture::SDLTexture(SDL_Renderer* renderer, const string& fileName,SDL_Rect Dest) : texture_(nullptr), width_(0), height_(0),position(0,0),dest(Dest) {
	loadFromImg(renderer, fileName);
	setPosition({(double)dest.x,(double)dest.y});
}
/*
Texture::Texture(SDL_Renderer* renderer, const string& text, const Font* font, const SDL_Color& color) :
	texture_(nullptr), width_(0), height_(0) {
	loadFromText(renderer, text, font, color);
}*/

SDLTexture::SDLTexture(SDL_Renderer* renderer, SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dest, SDL_Rect* destRect) :
	texture_(nullptr), width_(0), height_(0) {
	loadFromSurface(renderer, src, srcRect, dest, destRect);
}

SDLTexture::~SDLTexture() {
	close();
}


SDLTexture& SDLTexture::operator=(SDLTexture &&other) noexcept {
	this->~SDLTexture();
	texture_ = other.texture_;
	other.texture_ = nullptr;
	renderer_ = other.renderer_;
	other.renderer_ = nullptr;
	width_ = other.width_;
	height_ = other.height_;
	dest=other.dest;
	position=other.position;
	return *this;
}

SDLTexture::SDLTexture(SDLTexture &&other) noexcept {
	texture_ = other.texture_;
	other.texture_ = nullptr;
	renderer_ = other.renderer_;
	other.renderer_ = nullptr;
	width_ = other.width_;
	height_ = other.height_;
	dest=other.dest;
	position=other.position;
}


void SDLTexture::close() {
	if (texture_ != nullptr) {
		SDL_DestroyTexture(texture_); // destruye la textura actual
		texture_ = nullptr;
		width_ = 0;
		height_ = 0;
	}
}

bool SDLTexture::loadFromImg(SDL_Renderer* renderer, const string& fileName) {
	SDL_Surface* surface = IMG_Load(fileName.c_str());
	if (surface != nullptr) {
		close(); // destruye la textura actual para sustituirla
		texture_ = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture_ != nullptr) {
			width_ = surface->w;
			height_ = surface->h;           
		}
		SDL_FreeSurface(surface);
	}
	else {
		throw "No se puede cargar la imagen: " + fileName;
	}
	renderer_ = renderer;
	return texture_ != nullptr;
}

bool SDLTexture::loadFromSurface(SDL_Renderer* renderer, SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dest, SDL_Rect* destRect)
{
	if (SDL_BlitSurface(src, srcRect, dest, destRect) == 0) {
		if (dest != nullptr) {
			close(); // destruye la textura actual para sustituirla
			texture_ = SDL_CreateTextureFromSurface(renderer, dest);
			if (texture_ != nullptr) {
				width_ = dest->w;
				height_ = dest->h;
			}
			SDL_FreeSurface(dest);
		}
		else {
			throw "No se ha podido hacer BlitSurface";
		}
	}
	renderer_ = renderer;
	return texture_ != nullptr;
}

void SDLTexture::setSize(int wd,int hd){
	dest.w=wd;
	dest.h=hd;
}

void SDLTexture::render() const {
	render(dest);
}

void SDLTexture::render(const SDL_Rect& dest, const SDL_Rect& frame) const {
	if(!visible){return;}
	if (texture_) {
		if(SDL_RenderCopy(renderer_, texture_, &frame, &dest)==-1){
			std::cout<<SDL_GetError()<<"\n";
		};
	}
	else std::cout << "NO hay textura\n";
}

void SDLTexture::render(const SDL_Rect& dest) const {
	SDL_Rect frame = { 0, 0, width_, height_ };
	render(dest, frame);
}


void SDLTexture::render(const SDL_Rect& dest, double angle, const SDL_Rect& frame) const {
	if (texture_) {
		SDL_RenderCopyEx(renderer_, texture_, &frame, &dest, angle, nullptr, SDL_FLIP_NONE);
	}
}

void SDLTexture::render(const SDL_Rect& dest, double angle) const {
	SDL_Rect frame = { 0, 0, width_, height_ };
	render(dest, angle, frame);
}