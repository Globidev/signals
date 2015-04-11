#pragma once

#include "signal.hpp"

namespace signals {
    namespace literal {

        template <class LiteralT, LiteralT literal, class... Args>
        void emit(Args &&... args) {
            signal<LiteralT, literal, Args...>::emit(
                std::forward<Args>(args)...
            );
        }

    }
}
