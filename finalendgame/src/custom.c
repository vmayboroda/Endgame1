#include <stddef.h>
#include <stdlib.h>

wchar_t custom(int i)
{
    wchar_t *alphabet = L" = ☠ ☗ ☯ ▒";
    return alphabet[i];
}
