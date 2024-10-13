//
// Created by Syl Morrison on 13/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_MACROS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_MACROS_H
#include <iostream>
#ifdef NDEBUG
#define MH_LOG(TEXT)
#else
#define MH_LOG(TEXT) std::cout << TEXT << "\n"
#endif
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_MACROS_H
