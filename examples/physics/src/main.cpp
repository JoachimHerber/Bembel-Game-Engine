#include <string_view>
#include <vector>
import bembel.examples.physics;

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args{argv, argv + argc};

    bembel::examples::physics::PhysicsExample app;
    app.run(args);
}
