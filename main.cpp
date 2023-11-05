#include <iostream>
#include "Reader.h"

int main() {
    Reader reader("example/test.vio");
    std::cout << reader.get_content() << std::endl;
    reader.close();
    return 0;
}
