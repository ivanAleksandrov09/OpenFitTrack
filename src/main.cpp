#include <iostream>
#include "Set.h"

int main()
{
    Set s(1, "2026-05-08", 8, 100.0f, 90);
    std::cout << s << std::endl;
    std::cout << "Volume: " << s.getVolume() << "kg" << std::endl;
    std::cout << s.serialize() << std::endl;

    std::cin >> s;

    std::cout << "\n"
              << s << std::endl;
    std::cout << s.serialize() << std::endl;
    return 0;
}