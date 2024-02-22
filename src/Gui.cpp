#include "Gui.hpp"

#include <cstdarg>
#include <cstring>

#include "Engine.hpp"
#include "PlayerAi.hpp";

static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;
static const int MSG_X = BAR_WIDTH + 2;
static const int MSG_HEIGHT = PANEL_HEIGHT - 1;

Game::Gui::Gui() { con = new tcod::Console(engine.get_width(), PANEL_HEIGHT); }

Game::Gui::~Gui() {
  delete con;
  clear();
}

void Game::Gui::clear() {
  if (log.size() > 0) {
    for (auto& message : log) {
      delete message;
    }
    log.clear();
  }
}

void Game::Gui::render() {
  const int BUFFER_LEN = 128;
  char buffer[BUFFER_LEN];
  Game* game = (Game*)engine.scenes[GameScene::GAME];

  // Clear the GUI console
  TCOD_ConsoleTile tile;
  tile.bg = BLACK;
  tile.fg = WHITE;
  tile.ch = ' ';
  con->clear(tile);

  // fill it out
  renderBar(1, 1, BAR_WIDTH, "HP", game->player->destructible->hp, game->player->destructible->maxHp, lightRed, darkerRed);

  int y = 1;
  for (auto& message : log) {
    tcod::print(*con, {MSG_X, y}, message->text.c_str(), message->col, black, TCOD_LEFT, TCOD_BKGND_SET);
    y++;
  }

  // draw the XP bar
  PlayerAi* ai = (PlayerAi*)game->player->ai;
#ifdef _WIN32
  sprintf_s(buffer, (size_t)BUFFER_LEN, "XP: %d", ai->xpLevel);
#else
  std::snprintf(buffer, (size_t)BUFFER_LEN, "XP: %d", ai->xpLevel);
#endif
  renderBar(1, 5, BAR_WIDTH, buffer, game->player->destructible->xp, ai->getNextLevelXp(), lightViolet, darkerViolet);


  renderMouseLook();

#ifdef _WIN32
  sprintf_s(buffer, (size_t)BUFFER_LEN, "Dungeon Level: %d", game->get_level());
#else
  std::snprintf(buffer, (size_t)BUFFER_LEN, "Dungeon Level: %d", game->get_level());
#endif

  tcod::print(*con, {3, 3}, buffer, lightGrey, black, TCOD_LEFT, TCOD_BKGND_SET);

  // Blit it
  tcod::blit(
      engine.get_console(),
      *con,
      {0, engine.get_console().get_height() - PANEL_HEIGHT},
      {0, 0, con->get_width(), con->get_height()},
      1.0f,
      1.0f);
}

void Game::Gui::renderBar(
    int x,
    int y,
    int width,
    const char* name,
    float value,
    float maxValue,
    const TCOD_ColorRGB& barColor,
    const TCOD_ColorRGB& backColor) {
  tcod::draw_rect(*con->get(), {x, y, width, 1}, (int)' ', white, backColor, TCOD_BKGND_SET);

  int barWidth = (int)((value * width) / maxValue);
  if (barWidth > 0) {
    // draw the bar
    tcod::draw_rect(*con->get(), {x, y, barWidth, 1}, (int)' ', white, barColor, TCOD_BKGND_SET);
  }

  // print text on top of the bar.
  TCOD_console_printf_ex(
      con->get(), x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name, value, maxValue);
}

void Game::Gui::message(const TCOD_ColorRGB& col, const char* text, ...) {
  // build the text
  const int BUFFER_LEN = 128;
  char buffer[BUFFER_LEN];
  va_list args;
  va_start(args, text);
#ifdef _WIN32
  vsprintf_s(buffer, BUFFER_LEN, text, args);
#else
  std::vsnprintf(buffer, (size_t)BUFFER_LEN, text, args);
#endif
  va_end(args);

  char* lineBegin = buffer;
  char* lineEnd;

  do {
    // make room for the new message
    if (log.size() >= MSG_HEIGHT) {
      Message* top = log.front();
      log.pop_front();
      delete top;
    }

    lineEnd = std::strchr(lineBegin, '\n');

    if (lineEnd) {
      *lineEnd = '\0';
    }
    log.push_back(new Message(lineBegin, col));
    // go to next line;
    lineBegin = lineEnd + 1;
  } while (lineEnd);
}

void Game::Gui::renderMouseLook() {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (!game->map->isInFov(engine.mouseX, engine.mouseY)) {
    // if mouse is out of fov, nothing to render
    return;
  }
  std::string buffer;
  bool first = true;

  for (auto const& actor : game->actors) {
    if (actor->x == engine.mouseX && actor->y == engine.mouseY) {
      if (!first) {
        buffer.append(", ");
      } else {
        first = false;
      }
      buffer.append(actor->name.c_str());
    }
  }

  tcod::print(*con, {1, 0}, buffer, lightGrey, black, TCOD_LEFT, TCOD_BKGND_SET);
}

void Game::Gui::load(std::ifstream& stream) {
  char delim = ',';
  int countMessages = 0;
  log.clear();
  stream >> countMessages >> delim;
  for (int i = 0; i < countMessages; ++i) {
    Message* message = new Message("", white);
    message->load(stream);
    log.push_back(message);
  }
}

void Game::Gui::save(std::ofstream& stream) {
  const char delim = ',';
  stream << log.size() << delim;
  for (auto& message : log) {
    message->save(stream);
  }
}

Game::Gui::Message::Message() : text(nullptr), col({255, 255, 255}) {}

Game::Gui::Message::Message(const char* text, const TCOD_ColorRGB& col) : col(col) { this->text.assign(text); }

Game::Gui::Message::~Message() { text.clear(); }

void Game::Gui::Message::load(std::ifstream& stream) {
  char delim = ',';
  stream >> col.r >> delim >> col.g >> delim >> col.b >> delim;
  int strLen;
  stream >> strLen >> delim;
  char* buffer = new char[strLen + 1];
  stream.read(buffer, strLen);
  buffer[strLen] = '\0';
  stream >> delim;
  text.assign(buffer);
  delete[] buffer;
}

void Game::Gui::Message::save(std::ofstream& stream) {
  const char delim = ',';
  stream << col.r << delim << col.g << delim << col.b << delim << text.size() << delim << text.c_str() << delim;
}
