#include "Menu.hpp"
#include "Engine.hpp"

Game::Menu::~Menu() {
    clear();
}

void Game::Menu::clear() {
    for(auto & menuItem : items) {
        delete menuItem;
    }
    items.clear();
}

void Game::Menu::addItem(Game::Menu::MenuItemCode code, const char* label) {
    MenuItem* item = new MenuItem();
    item->code = code;
    item->label.assign(label);
    items.push_back(item);
}

Game::Menu::MenuItemCode Game::Menu::pick() {
    static TCODImage img((engine.GetRootDir() / "data" / "menu_background1.png").c_str());

    int selectedItem = 0;
    while (true) {
        tcod::draw_quartergraphics(engine.get_console(), img, {0, 0}, {0, 0, -1, -1});
        int currentItem = 0;
        for(auto& item : items) {
            tcod::print(engine.get_console(), {10, 10 + (currentItem * 3)}, item->label.c_str(), currentItem == selectedItem ? black : lightGrey, currentItem == selectedItem ? lighterOrange : black, TCOD_LEFT, TCOD_BKGND_SET);        
            currentItem++;        
        }
        engine.flush();
        // Wait for a key up or exit
        int currentKey, dy = 0, mouseX = 0, mouseY = 0;
        int tileWidth = engine.get_tile_width(), tileHeight = engine.get_tile_height();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            dy = 0;
            switch (event.type) {
            case SDL_QUIT:
                engine.Stop();
                return MenuItemCode::EXIT;
                break;
            case SDL_KEYDOWN:
                {
                    currentKey = event.key.keysym.sym;
                    switch(currentKey) {
                        case SDLK_ESCAPE:
                            return MenuItemCode::EXIT;
                            break;
                        case SDLK_RETURN:
                        case SDLK_RETURN2:
                        case SDLK_KP_ENTER:
                            {
                                std::list<MenuItem*>::iterator iter =items.begin();
                                std::advance(iter, selectedItem);
                                return (*iter)->code;
                            }
                            break;
                        case SDLK_UP:
                        case SDLK_KP_8:
                            dy = -1;
                            break;
                        case SDLK_DOWN:
                        case SDLK_KP_2:
                            dy = 1;
                            break;
                        
                    }
                    if (dy != 0) {
                        selectedItem += dy;
                        if (selectedItem < 0) selectedItem = items.size() - 1;
                        if (selectedItem >= (int)items.size()) selectedItem = 0;
                    }
                }    
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    switch (event.button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        {
                            // Select and return on click.
                            int index = 0;
                            for(auto& item : items) {
                                if (mouseX >= 10 && 
                                    mouseX <= 10 + (int)item->label.size() &&
                                    mouseY == 10 + (3 * index)) {
                                    return item->code;
                                    break;
                                }
                                index++;
                            }
                        }
                        break;
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                // change selection on mouse move.
                {
                    mouseX = event.motion.x / tileWidth;
                    mouseY = event.motion.y / tileHeight;
                    int index = 0;
                    for(auto& item : items) {
                        if (mouseX >= 10 && 
                            mouseX <= 10 + (int)item->label.size() &&
                            mouseY == 10 + (3 * index)) {
                            selectedItem = index;
                            break;
                        }
                        index++;
                    }
                }
                break;
            }
        }

    }
    return MenuItemCode::NONE;
}