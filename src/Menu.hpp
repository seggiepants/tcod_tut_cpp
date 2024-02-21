#ifndef __MENU_HPP__
#define __MENU_HPP__

#include <list>
#include <string>

#include "Scene.hpp"

namespace Game {
class Menu : public Scene {
 public:
  enum MenuItemCode { NONE, NEW_GAME, CONTINUE, EXIT };
  Menu();
  ~Menu();
  void clear();
  void addItem(MenuItemCode code, const char* label);
  void init();
  void destroy();
  Scene* update();
  void render();

 protected:
  struct MenuItem {
    MenuItemCode code;
    std::string label;
  };
  std::list<MenuItem*> items;
  int selectedItem;
  int mouseX;
  int mouseY;

  Scene* processMenu(MenuItemCode selection);
};
}  // namespace Game
#endif
