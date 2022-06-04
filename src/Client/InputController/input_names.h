#pragma once

#include <unordered_map>
#include <QString>
#include <QObject>

namespace input {

Q_NAMESPACE

enum class Name {
  kEsc,
  kF1,
  kF2,
  kF3,
  kF4,
  kF5,
  kF6,
  kF7,
  kF8,
  kF9,
  kF10,
  kF11,
  kF12,
  kScrollLock,
  kPause,
  kApostrophe,
  k1,
  k2,
  k3,
  k4,
  k5,
  k6,
  k7,
  k8,
  k9,
  k0,
  kMinus,
  kEqualSign,
  kBackspace,
  kInsert,
  kHome,
  kPageUp,
  kNumLock,
  kRightSlash,
  kStar,
  kTab,
  kQ,
  kW,
  kE,
  kR,
  kT,
  kY,
  kU,
  kI,
  kO,
  kP,
  kLeftSquareBrace,
  kRightSquareBrace,
  kEnter,
  kDelete,
  kEnd,
  kPageDown,
  kCapsLock,
  kA,
  kS,
  kD,
  kF,
  kG,
  kH,
  kJ,
  kK,
  kL,
  kSemiColon,
  kTopComa,
  kLeftSlash,
  kLShift,
  kZ,
  kX,
  kC,
  kV,
  kB,
  kN,
  kM,
  kComa,
  kPoint,
  kRShift,
  kArrowUp,
  kREnter,
  kLCtrl,
  kLAlt,
  kSpace,
  kRAlt,
  kWindows,
  kActionList,
  kRCtrl,
  kArrowLeft,
  kArrowDown,
  kArrowRight,
  kPlus,
  kLeftMouseButton,
  kRightMouseButton
};

Q_ENUM_NS(Name)

QString InputNameToString(Name type);

extern const std::unordered_map<uint32_t, Name> kNativeCodeToName;

}  // namespace input
