#include "Button.h"
#include "Player.h"
Button:: Button(SDL_Renderer* renderer, const string& fileName, SDL_Rect Dest, Player* player, void (Player::*onClick)(),void (Player::*OnClick2)(int,bool))
        : SDLTexture(renderer, fileName, Dest), player_(player), onClick_(onClick),onClick2_(OnClick2),wasclicked(false) {}

void Button::handleEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && (onClick_||onClick2_) && visible) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        if (mouseX >= dest.x && mouseX < dest.x + dest.w &&
            mouseY >= dest.y && mouseY < dest.y + dest.h) {			
           if(onClick_!=nullptr) (player_->*onClick_)();
		   if(onClick2_!=nullptr){
				wasclicked=!wasclicked;
				(player_->*onClick2_)(index,wasclicked);
			}
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

	player_=other.player_;
	onClick_=other.onClick_;
	onClick2_=other.onClick2_;

	index=other.index;
	wasclicked=other.wasclicked;
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

    player_=other.player_;
	onClick_=other.onClick_;
	onClick2_=other.onClick2_;

	index=other.index;
	wasclicked=other.wasclicked;
}