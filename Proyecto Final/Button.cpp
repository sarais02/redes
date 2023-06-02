#include "Button.h"
#include "Player.h"
Button:: Button(SDL_Renderer* renderer, const string& fileName, SDL_Rect Dest, Player* player, void (Player::*onClick)())
        : SDLTexture(renderer, fileName, Dest), player_(player), onClick_(onClick) {}

void Button::handleEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && onClick_) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        if (mouseX >= dest.x && mouseX < dest.x + dest.w &&
            mouseY >= dest.y && mouseY < dest.y + dest.h) {
            (player_->*onClick_)();
        }
    }   
};

Button& Button::operator=(Button &&other) noexcept {
	this->~Button();
	texture_ = other.texture_;
	other.texture_ = nullptr;
	renderer_ = other.renderer_;
	other.renderer_ = nullptr;
	width_ = other.width_;
	height_ = other.height_;
	dest=other.dest;
	position=other.position;
    setOnClick(other.player_,other.onClick_);
	return *this;
}

Button::Button(Button &&other) noexcept {
	texture_ = other.texture_;
	other.texture_ = nullptr;
	renderer_ = other.renderer_;
	other.renderer_ = nullptr;
	width_ = other.width_;
	height_ = other.height_;
	dest=other.dest;
	position=other.position;
     setOnClick(other.player_,other.onClick_);
}