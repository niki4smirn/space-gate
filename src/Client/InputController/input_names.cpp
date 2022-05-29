#include "input_names.h"

#include <QMetaEnum>

namespace input {

QString InputNameToString(Name type) {
  return {QMetaEnum::fromType<Name>().valueToKey(static_cast<int>(type))};
}

}  // namespace input
