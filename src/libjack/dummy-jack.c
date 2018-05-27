#include "dummy-jack.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

void
dummy_jack()
{
    fprintf(stderr, "Hello, dummy jack !\n");
}
