# factory.hpp

A factory that instantiates types based on a unique identifier. Arguments of the concrete type's
constructor can be forwarded upon registering the type.
The factory class is provided as a single header file with no other dependencies besides
the c++ standard library.

# Example

A simple example, can also be found in `example.cpp`.

```c++
#include "Factory.hpp"

#include <iostream>

struct Scene {
        virtual void printName() = 0;
};

struct SceneA : public Scene {
        explicit SceneA(int a) {
                std::cout << "A is " << a << std::endl;
        }

        void printName() override {
                std::cout << "I'm scene A" << std::endl;
        }
};

struct SceneB : public Scene {
        void printName() override {
                std::cout << "I'm scene B" << std::endl;
        }
};

enum class Scenes {
        A,
        B,
};

int main() {
        sro5h::Factory<Scene, Scenes> factory;
        factory.registerType<SceneA>(Scenes::A, 3);
        factory.registerType<SceneB>(Scenes::B);

        std::unique_ptr<Scene> sceneA = factory.create(Scenes::A);
        std::unique_ptr<Scene> sceneB = factory.create(Scenes::B);

        sceneA->printName();
        sceneB->printName();

        return 0;
}
```
