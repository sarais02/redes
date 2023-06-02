#ifndef SDLTexture_H_
#define SDLTexture_H_
#include <SDL2/SDL_image.h>
#include "Vector2.h"
#include <string>
#include <SDL_ttf.h>
using namespace std;


class Font {
public:
	Font() : font_(nullptr) {}
    Font(const std::string& fontFile, int fontSize) {

        font_ = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (!font_) {
            // Manejar el error si no se pudo cargar la fuente
			std::cout<<"ERROR AL ABRIR LA FUENTE\n";
        }
    }
    
	// cannot copy textures
	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	// can be moved
	Font& operator=(Font &&other) noexcept;
	Font(Font &&other) noexcept;


    ~Font() {
        TTF_CloseFont(font_);
        TTF_Quit();
    }
    
	SDL_Surface* RenderText(const std::string& text, SDL_Color color) const {
		if(font_==nullptr)std::cout<<"curisoso\n";
       SDL_Surface* surface = nullptr;
	   
	   surface = TTF_RenderText_Blended(font_, text.data(), color);
		return surface;
    }	
    
private:
    TTF_Font* font_;
};

class SDLTexture {
protected:
	SDL_Texture* texture_;
	SDL_Renderer* renderer_;
	int width_;
	int height_;
    Vector2 position;
	SDL_Rect dest;
	bool visible=true;
public:
	SDLTexture();
	SDLTexture(SDL_Renderer* renderer, const string& fileName,SDL_Rect Dest);
	SDLTexture(SDL_Renderer* renderer, const string& text, const Font* font, const SDL_Color& color);
	SDLTexture(SDL_Renderer* renderer, SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dest, SDL_Rect* destRect);
	
	// cannot copy textures
	SDLTexture(const SDLTexture&) = delete;
	SDLTexture& operator=(const SDLTexture&) = delete;

	// can be moved
	SDLTexture& operator=(SDLTexture &&other) noexcept;
	SDLTexture(SDLTexture &&other) noexcept;

	inline void setVisibility(bool Visible){visible=Visible;};
	inline bool getVisibility(){return visible;};

	virtual ~SDLTexture();

	inline int getWidthDest() {
		return dest.w;
	}

	inline int getHeightDest() {
		return dest.h;
	}

	inline bool isReady() {
		return texture_ != nullptr;
	}
    void setSize(int wd,int hd);
    inline void setPosition(Vector2 pos){position=pos;dest.x=pos.getX();dest.y=pos.getY();};
    inline Vector2 getPosition(){return position;};
	inline void setDest(SDL_Rect uax){dest=uax;};
	inline SDL_Rect getDest(){return dest; };
	// Carga las texturas a partir de una imagen
	bool loadFromImg(SDL_Renderer* renderer, const string& fileName);

	//Carga las texturas a partir de un texto con una fuente
	bool loadFromText(SDL_Renderer* renderer, const string& text, const Font* font, const SDL_Color& color = { 0, 0, 0, 255 });

	void ChangeText(const std::string& newText, const Font& font, const SDL_Color& color);

	//Carga texturas a partir de dos surface para hacer un blit
	bool loadFromSurface(SDL_Renderer* renderer, SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dest, SDL_Rect* destRect);

	//Renderiza en posiciones directamente
	void render() const;

	//Renderiza un frame de la textura en el destRect, si no hay frame se renderiza toda la textura
	void render(const SDL_Rect& dest) const;
	void render(const SDL_Rect& dest, const SDL_Rect& frame) const;

	// Renderiza un frame de la textura en el destRect con una rotación, si no hay frame se renderiza toda la textura con un ángulo
	void render(const SDL_Rect& dest, double angle, const SDL_Rect& frame) const;
	void render(const SDL_Rect& dest, double angle) const;
	//Devuelve el SDL_Texture
	SDL_Texture* getSDLTex() { return texture_; }

	void close();
};
#endif