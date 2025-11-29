//
// Created by Syl Morrison on 29/11/2025.
//

#ifndef MOSTLYHARMLESS_VISITOR_H
#define MOSTLYHARMLESS_VISITOR_H
namespace mostly_harmless::utils {
    /**
     * \brief Util for use with `std::visit` to avoid a bunch of `if constexpr(...)`s.
     *
     * Usage::
     * ```
     *      std::variant<int, double> data;
     *      std::visit(Visitor{
     *          [](int x) { std::cout << "Was int!\n"; },
     *          [](double x) { std::cout << "Was double!\n"; }
     *      }, data);
     * ```
     * @tparam Callable
     */
    template <typename... Callable>
    struct Visitor : Callable... {
        using Callable::operator()...;
    };
} // namespace mostly_harmless::utils
#endif // GLEO_MOSTLYHARMLESS_VISITOR_H
