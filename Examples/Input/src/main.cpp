#include <string_view>
#include <vector>
import bembel.examples.input;
import bembel.base;

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args{argv, argv + argc};

    bembel::InputExample example;

    bembel::base::logInfo((char*)u8"Testing unicode -- English -- Ελληνικά -- Español.");
    example.run(args);
}
