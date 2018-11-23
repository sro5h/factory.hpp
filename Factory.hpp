#pragma once

#include <unordered_map>
#include <memory>

namespace sro5h {

template <typename Type>
class Creator {
public:
        virtual std::unique_ptr<Type> create() = 0;
};

template <typename Type, typename ConcreteType>
class ConcreteCreator : public Creator<Type> {
public:
        std::unique_ptr<Type> create() override {
                return std::make_unique<ConcreteType>();
        }
};

template <typename Type, typename Identifier>
class Factory {
public:
        template <typename ConcreteType>
        void registerType(Identifier identifier) {
                static_assert(std::is_base_of<Type, ConcreteType>::value,
                                "Type must be a base of ConcreteType");
                mCreators[identifier] = std::make_unique<ConcreteCreator<Type, ConcreteType>>();
        }

        std::unique_ptr<Type> create(Identifier identifier) {
                if (mCreators.find(identifier) != mCreators.end()) {
                        return mCreators[identifier]->create();
                }

                return nullptr;
        }

private:
        std::unordered_map<Identifier, std::unique_ptr<Creator<Type>>> mCreators;
};

}
