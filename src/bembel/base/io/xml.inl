namespace bembel::base::xml {

template <typename T>
inline bool setAttribute(Element* node, const std::string& name, const T& value) {
    if(!node) return false;

    std::stringstream sstream;
    sstream << value;
    node->SetAttribute(name.c_str(), sstream.str().c_str());
    return true;
}

template <typename T>
inline bool getAttribute(const Element* node, const std::string& name, T& value) {
    if(!node) return false;

    const char* attrib = node->Attribute(name.c_str());
    if(!attrib) return false;

    std::stringstream sstream;
    sstream << attrib;
    sstream >> value;
    return true;
}

template <typename T>
inline bool getAttribute(
    const Element* node, const std::string& child_node, const std::string& name, T& value) {
    if(!node) return false;

    return getAttribute<T>(node->FirstChildElement(child_node.c_str()), name, value);
}

inline ElementIterator IterateChildElements(const Element* element, const std::string& name = "") {
    return ElementIterator(element, name);
}

inline bool ElementIterator::operator!=(const ElementIterator& other) {
    if(this->element != other.element) return true;
    return this->element != nullptr && this->name != other.name;
}

inline ElementIterator& ElementIterator::operator++() {
    if(this->name.empty())
        this->element = this->element->NextSiblingElement();
    else
        this->element = this->element->NextSiblingElement(this->name.c_str());

    return *this;
}

inline ElementIterator ElementIterator::begin() {
    if(this->name.empty()) {
        return ElementIterator(this->element->FirstChildElement(), this->name);
    } else {
        return ElementIterator(this->element->FirstChildElement(this->name.c_str()), this->name);
    }
}

} // namespace bembel::base::xml
