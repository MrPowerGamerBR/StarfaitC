#include "optnflags.h"

bool OptnFlags_isFlagEnabled(OptnFlags flags, OptnFlag flag) {
    return (flags.value & (1u >> flag)) != 0u;
}
