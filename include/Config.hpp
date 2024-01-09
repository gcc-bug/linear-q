#ifndef Config
#define Config

#include <type_traits>
#include <string>
#include <iostream>
namespace lq{
    // Helper template to check if a type is printable
    template<typename T, typename = void>
    struct is_printable : std::false_type {};

    template<typename T>
    struct is_printable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> : std::true_type {};

    using label = int ;
    // Static assert to ensure label is printable
    static_assert(is_printable<label>::value, "label type must be printable");
}

#endif /* Config */
