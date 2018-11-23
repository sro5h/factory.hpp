#pragma once

#include <unordered_map>
#include <tuple>
#include <memory>
#include <utility>

namespace sro5h {

/**
 * @brief An interface that specifies the instatiation of a type.
 *
 * @tparam Type The Type which will be instantiated.
 */
template <typename Type>
class Creator {
public:
        /**
         * @brief Instantiates a type and returns an std::unique_ptr to it.
         *
         * @return The std::unique_ptr to the instance.
         */
        virtual std::unique_ptr<Type> create() = 0;
};

/**
 * @brief A concrete creator that instantiates a concrete type.
 *
 * If the constructor of ConcreteType needs custom arguments, they can be passed
 * to the constructor of the ConcreteCreator.
 *
 * @tparam Type The base type.
 * @tparam ConcreteType The concrete type that will be instantiated.
 * @tparam Args The arguments that will be forwarded to the constructor
 */
template <typename Type, typename ConcreteType, typename... Args>
class ConcreteCreator : public Creator<Type> {
public:
        explicit ConcreteCreator(Args&&... args)
                : mArgs(std::forward<Args>(args)...) {
        }

        std::unique_ptr<Type> create() override {
                return createImpl(std::make_index_sequence<sizeof... (Args)> {});
        }

private:
        template <size_t... Is>
        std::unique_ptr<Type> createImpl(std::index_sequence<Is...>) {
                return std::make_unique<ConcreteType>(std::get<Is>(mArgs)...);
        }

        std::tuple<Args...> mArgs;
};

/**
 * @brief A factory that instantiates concrete types of base type Type.
 *
 * Before a type can be instantiated with create, it must be registered using
 * registerType. Constructor arguments of ConcreteType can be passed to
 * registerType.
 *
 * @tparam Type The base type.
 * @tparam Identifier The identifier.
 */
template <typename Type, typename Identifier>
class Factory {
public:
        template <typename ConcreteType, typename... Args>
        void registerType(Identifier identifier, Args&&... args) {
                static_assert(std::is_base_of<Type, ConcreteType>::value,
                                "Type must be a base of ConcreteType");
                mCreators[identifier] = std::make_unique<ConcreteCreator<Type, ConcreteType, Args...>>(
                                std::forward<Args>(args)...);
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
