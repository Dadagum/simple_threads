#ifndef SIMPLE_THREADS_COMMON_H
#define SIMPLE_THREADS_COMMON_H

#include <unistd.h>
#include <pthread.h>


#define COPY_CONSTRUCT_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    TypeName& operator=(const TypeName&)


#endif