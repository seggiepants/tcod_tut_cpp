#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <libtcod.hpp>

namespace Game {
    
    class Gui {
    public:
        Gui();
        virtual ~Gui();
        void render();
        void message(const TCOD_ColorRGB& col, const char * text, ...);
        int get_width() { return con->get_width();}
        int get_height() { return con->get_height();}     
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);       
        struct Message {
            std::string text;
            TCOD_ColorRGB col;
            Message();
            Message(const char* text, const TCOD_ColorRGB& col);
            ~Message();

            void load(std::ifstream& stream);
            void save(std::ofstream& stream);
        };
        std::list<Game::Gui::Message*> log;

    protected:
        tcod::Console* con;        

        void renderBar(int x, int y, int width, const char* name, float value, float maxValue, const TCOD_ColorRGB& barColor, const TCOD_ColorRGB& backColor);
        void renderMouseLook();
    };
}

#endif