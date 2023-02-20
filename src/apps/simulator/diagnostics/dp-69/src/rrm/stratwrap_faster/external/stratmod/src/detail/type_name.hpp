#ifndef PRETTY_PRINT_TYPENAME_AT_COMPILE_TIME_IN_CLANG_GCC_MSVC_WHEN_USING_CPP17_OR_HIGHER
#define PRETTY_PRINT_TYPENAME_AT_COMPILE_TIME_IN_CLANG_GCC_MSVC_WHEN_USING_CPP17_OR_HIGHER

//
// Obtained from stackoverflow in April 30, 2020
// https://stackoverflow.com/a/20170989
//
// At the time the code was retrieved, it was licensed as CC BY-SA 4.0
// https://stackoverflow.com/legal/terms-of-service#licensing
// https://creativecommons.org/licenses/by-sa/4.0/
//

#include <string_view>

/// Return human readable type name at compile time (CAUTION: compiler dependent)
///
/// Usage example:
///
/// std::cout << type_name<decltype( foo )>() << std::endl;
///
/// where `foo` stands for whatever it is to be inspected.
///
template <typename T>
constexpr std::string_view type_name()
{
    std::string_view name, prefix, suffix;

#if defined(__clang__)
    name = __PRETTY_FUNCTION__;
    prefix = "std::string_view type_name() [T = ";
    suffix = "]";

#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr std::string_view type_name() [with T = ";
    suffix = "; std::string_view = std::basic_string_view<char>]";

#elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl type_name<";
    suffix = ">(void)";

#endif

    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());

    return name;
}

#endif
