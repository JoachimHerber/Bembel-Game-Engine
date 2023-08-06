#include <string_view>
#include <vector>
import bembel.examples.rendering;

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args{argv, argv + argc};

    bembel::RenderingExample app;
    app.run(args);
}
