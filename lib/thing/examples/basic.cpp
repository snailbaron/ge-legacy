#include <ecosnail/thing.hpp>

#include <iostream>
#include <string>

namespace th = ecosnail::thing;

using namespace std::string_literals;

int main()
{
    th::EntityManager entities;

    auto e1 = entities.createEntity();
    auto e2 = entities.createEntity();

    entities.add(e1, 1);
    entities.add(e1, "abc"s);
    entities.add(e2, 2);

    std::cout << "e1 int: " << entities.component<int>(e1) << "\n";
    std::cout << "e1 string: " << entities.component<std::string>(e1) << "\n";
    std::cout << "e2 int: " << entities.component<int>(e2) << "\n";

    for (int x : entities.components<int>()) {
        std::cout << "int component: " << x << "\n";
    }
}
