#pragma once

#include "randomizerTypes.h"
#include <stdint.h>

namespace Rando {

enum class ItemPriority {
    Late = 0,
    Default = 1,
    Early = 2
};

ItemPriority GetItemPriority(RandomizerGet item);

} // namespace Rando
