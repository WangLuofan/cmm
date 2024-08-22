#include "utils.h"

int align_to(int bytes, int align) {
    return (bytes + align - 1) / align * align;
}