#include <string_view>
#include <vector>
import bembel.examples.gm_helper;
import bembel.base;

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args{argv, argv + argc};

    bembel::examples::gm_helper::Application example;

    example.run(args);
}
