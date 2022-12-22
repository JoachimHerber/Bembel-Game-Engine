module;
module bembel.base;

namespace bembel::base {

Exeption::Exeption(std::string const& what, std::source_location location)
  : std::runtime_error(what) {
    log(location).error(what);
}

Exeption::Exeption(char const* what, std::source_location location) : std::runtime_error(what) {
    log(location).error(what);
}

} // namespace bembel::base
