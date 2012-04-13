#ifndef STRINGHELP_H
#define STRINGHELP_H

#include <iostream>

#ifndef PRINT
    #define PRINT( A ) \
        std::cout << "  " << #A << " -> " << (A) << std::endl
    #define PRINTN( A ) \
        std::cout << "  " << #A << " ->\n" << (A) << std::endl
    #define TODO \
        std::cerr << "TODO " << __FILE__ << ":" << __LINE__ << std::endl
#endif

inline bool is_a_number( char a ) { return a >= '0' and a <= '9'; }
std::string url_encode( const std::string &str );
std::string url_decode( const std::string &str );


#endif // STRINGHELP_H
