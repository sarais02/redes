#ifndef Button_H_
#define Button_H_
#include "SDLTexture.h"
#include <SDL2/SDL_image.h>
#include <functional>
class Player;
class Button : public SDLTexture {
public:
    Button() : SDLTexture(), player_(nullptr), onClick_(nullptr),onClick2_(nullptr) {}

    Button(SDL_Renderer* renderer, const string& fileName, SDL_Rect Dest, Player* player, void (Player::*onClick)(),void (Player::*OnClick2)(int,bool)=nullptr);

    // cannot copy textures
	Button(const Button&) = delete;
	Button& operator=(const Button&) = delete;

	// can be moved
	Button& operator=(Button &&other) noexcept;
	Button(Button &&other) noexcept;

    ~Button()=default;

    void setOnClick(Player* player, void (Player::*onClick)(),void (Player::*onClick2)(int,bool)=nullptr) {
        player_ = player;
        onClick_ = onClick;
        onClick2_= onClick2_;
    }

    void handleEvent(SDL_Event& event);
    inline void setIndex(int Index){index=Index;};
private:
    Player* player_;
    void (Player::*onClick_)();
    //PARA HIPOTECAR Y DESHIPOTECAR
    void (Player::*onClick2_)(int,bool);
    int index=0;
    bool wasclicked=true;
};


#endif