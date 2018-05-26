#include "dummy-mumble.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

void
dummy_mumble()
{
    fprintf(stderr, "Hello, dummy mumble !");
}
