import bembel.examples.input;
import bembel.base;
int main() {
    bembel::InputExample example;

    bembel::base::log().info((char*)u8"Testing unicode -- English -- Ελληνικά -- Español.");
    example.run();
}
