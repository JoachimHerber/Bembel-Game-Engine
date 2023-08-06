#include <string_view>
#include <vector>
import bembel.examples.gui;

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args{argv, argv + argc};

    bembel::GuiExample example;
    example.run(args);
}
