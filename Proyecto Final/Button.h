#ifndef Button_H_
#define Button_H_
#include "SDLTexture.h"
#include <SDL2/SDL_image.h>
#include <functional>
class Player;
class Button : public SDLTexture {
public:
    Button() : SDLTexture(), player_(nullptr), onClick_(nullptr) {}

    Button(SDL_Renderer* renderer, const string& fileName, SDL_Rect Dest, Player* player, void (Player::*onClick)());

    // cannot copy textures
	Button(const Button&) = delete;
	Button& operator=(const Button&) = delete;

	// can be moved
	Button& operator=(Button &&other) noexcept;
	Button(Button &&other) noexcept;

    ~Button()=default;

    void setOnClick(Player* player, void (Player::*onClick)()) {
        player_ = player;
        onClick_ = onClick;
    }

    void handleEvent(SDL_Event& event);

private:
    Player* player_;
    void (Player::*onClick_)();
};


#endif