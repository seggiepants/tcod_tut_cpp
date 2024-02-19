#ifndef __MENU_HPP__
#define __MENU_HPP__

#include <list>
#include <string>

namespace Game {
    class Menu {
    public:
        enum MenuItemCode {
            NONE,
            NEW_GAME,
            CONTINUE,
            EXIT
        };
        ~Menu();
        void clear();
        void addItem(MenuItemCode code, const char* label);
        MenuItemCode pick();
    protected:
        struct MenuItem {
            MenuItemCode code;
            std::string label;        
        };
        std::list<MenuItem* > items;
    };
}
#endif