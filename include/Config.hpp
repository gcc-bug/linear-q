/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-01-27 22:26:28
 * @Description  : this file is about the basic type
 * @TODO         : add more type
 */
#ifndef Config
#define Config

#include <type_traits>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <stdexcept>
#include <set>
// Helper template to check if a type is printable
template<typename T, typename = void>
struct is_printable : std::false_type {};

template<typename T>
struct is_printable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> : std::true_type {};

using label = short ;
// Static assert to ensure label is printable
static_assert(is_printable<label>::value, "label type must be printable");

#endif /* Config */
