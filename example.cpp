#include "Factory.hpp"

#include <iostream>

struct Scene {
        virtual void printName() = 0;
};

struct SceneA : public Scene {
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
        factory.registerType<SceneA>(Scenes::A);
        factory.registerType<SceneB>(Scenes::B);

        std::unique_ptr<Scene> sceneA = factory.create(Scenes::A);
        std::unique_ptr<Scene> sceneB = factory.create(Scenes::B);

        sceneA->printName();
        sceneB->printName();

        return 0;
}
