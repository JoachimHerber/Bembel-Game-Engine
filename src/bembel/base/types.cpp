module;
module bembel.base;

import std;

namespace bembel::base {

Exeption::Exeption(std::string const& what) : std::runtime_error(what) {
    logError(what);
}

Exeption::Exeption(char const* what) : std::runtime_error(what) {
    logError(what);
}

} // namespace bembel::base
