#include <filesystem>
import bembel.tools.editor;

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args{argv, argv + argc};

    bembel::tools::Application app;
    app.run(args);
}
