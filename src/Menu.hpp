#ifndef __MENU_HPP__
#define __MENU_HPP__

#include <list>
#include <string>

#include "Scene.hpp"

namespace Game {
class Menu : public Scene {
 public:
  enum DisplayMode { MENU, LEVELUP};
  enum MenuItemCode { NONE, NEW_GAME, CONTINUE, EXIT, CONSTITUTION, STRENGTH, AGILITY };
  Menu();
  ~Menu();
  void clear();
  void addItem(MenuItemCode code, const char* label);
  void init();
  void destroy();
  Scene* update();
  void render();
  DisplayMode mode;

 protected:
  struct MenuItem {
    MenuItemCode code;
    std::string label;
  };
  std::list<MenuItem*> items;
  int selectedItem;
  int mouseX;
  int mouseY;
  int menuX;
  int menuY;

  Scene* processMenu(MenuItemCode selection);
};
}  // namespace Game
#endif
