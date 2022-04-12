#ifndef SIMPLE_THREADS_COMMON_H
#define SIMPLE_THREADS_COMMON_H

#include <unistd.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    TypeName& operator=(const TypeName&)

#endif