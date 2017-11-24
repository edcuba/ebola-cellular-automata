#include "ca.h"

CA::CA (size_t rows, size_t columns)
    : generation (rows, columns)
    , rows (rows)
    , columns (columns)
{
}
