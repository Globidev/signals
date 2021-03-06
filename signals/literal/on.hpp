#pragma once

#include "signal.hpp"

namespace signals {
    namespace literal {

        template <class LiteralT, LiteralT literal, class... Args, class F>
        void on(F && f) {
            signal<LiteralT, literal, Args...>::bind(std::forward<F>(f));
        }

        // Self deducing version from NON AMBIGUOUS F only
        template <class LiteralT, LiteralT literal, class F>
        struct signal_deducer;

        template <class LiteralT, LiteralT literal, class F>
        void on_(F && f) {
            using deduced_signal_t =
                typename signal_deducer<LiteralT, literal, F>::type;

            deduced_signal_t::bind(std::forward<F>(f));
        }

        template <class LiteralT, LiteralT literal, class F>
        struct signal_deducer {

        private:

            template <class T> struct helper;

            template <class R, class... Args>
            struct helper<R (F::*)(Args...) const> {
                // Removing cv-qualifiers for all parameters
                using type = signal<
                    LiteralT,
                    literal,
                    typename std::decay<Args>::type...
                >;
            };

        public:

            using type = typename helper<decltype(&F::operator())>::type;

        // The magic below is just for easier understanding of compiler errors
        // in case of an ambiguous F
        private:

            template <class T>
            static std::false_type overloaded_check(decltype(&T::operator()));
            template <class T>
            static std::true_type overloaded_check(...);

            static_assert(
                !decltype(overloaded_check<F>(nullptr))::value,
                "\033[38;5;33m" // A nice blue for an error pleasing to the eye
                "The given callable is overloaded and therefore prevents "
                "the signal type inference. Please explicit the types in its "
                "parameters."
                "\033[0m"
            );

        };

    }
}
