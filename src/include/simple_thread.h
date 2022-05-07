#ifndef SIMPLE_THREADS_COMMON_H
#define SIMPLE_THREADS_COMMON_H

#define DISALLOW_COPY_CONSTRUCT_ASSIGN(TypeName)    \
    TypeName(const TypeName&) = delete;             \
    TypeName& operator=(const TypeName&) = delete;


#endif