#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <libtcod.hpp>
#include <list>
#include <string>

namespace Game {
    class Gui {
    public:
        Gui();
        ~Gui();
        void render();
        void message(const TCOD_ColorRGB& col, const char * text, ...);
        int get_width() { return con->get_width();}
        int get_height() { return con->get_height();}    

    protected:
        tcod::Console* con;
        struct Message {
            std::string* text;
            TCOD_ColorRGB col;
            Message(const char* text, const TCOD_ColorRGB& col);
            ~Message();
        };
        std::list<Message*> log;

        void renderBar(int x, int y, int width, const char* name, float value, float maxValue, const TCOD_ColorRGB& barColor, const TCOD_ColorRGB& backColor);
        void renderMouseLook();
    };
}

#endif