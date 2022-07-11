#include "fonts.h"

namespace fonts {
  const QFont game_title_font = QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":Paladins.otf")).at(0));

}  // namespace fonts