import std;
import bembel.examples.chess;

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args{argv, argv + argc};

    bembel::examples::chess::Application app;
    app.run(args);
}
