#ifndef NOTIFY_UTIL_COPYABLE_H
#define NOTIFY_UTIL_COPYABLE_H

namespace xce {
namespace notify {

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
class copyable {
};

}
}

#endif  // NOTIFY_UTIL_COPYABLE_H
