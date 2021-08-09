module;
#include <concepts>
#include <random>
export module Util.RandomGenerator;

import Util.EnumUtils;

enum class RandomSeed : decltype(std::random_device{}()) { };

export
template <std::integral I>
using IntDistribution = std::uniform_int_distribution<I>;

export
template <std::floating_point I>
using RealDistribution = std::uniform_real_distribution<I>;

class RandomNumberGenerator
{
public:
    RandomNumberGenerator() = default;
    RandomNumberGenerator(RandomSeed seed):
        seed{ seed } { }

    template <typename T>
    auto generate(T&& distribution)
    {
        return std::forward<T>(distribution)(generator);
    }

    template <std::integral I>
    auto from_1_to_100()
    {
        IntDistribution<I> dis{1, 100};
        return generate(dis);
    }

    template <std::integral I>
    auto from(I min, I max)
    {
        IntDistribution<I> dis{ min, max };
        return generate(dis);
    }

    template <std::floating_point F>
    auto from_0_to_1()
    {
        RealDistribution<F> dis{ 0, 1 };
        return generate(dis);
    }

    template <std::floating_point F>
    auto from(F min, F max)
    {
        RealDistribution<F> dis{ min, max };
        return generate(dis);
    }

    auto initial_seed() const
    {
        return seed;
    }

    std::mt19937& raw()
    {
        return generator;
    }
private:
    RandomSeed seed = RandomSeed(std::random_device{}());
    std::mt19937 generator{ rep(seed) };
};

export
RandomNumberGenerator& random_generator()
{
    // The random number generator in <random> is HUGE and expensive to construct,
    // so we will only have one.
    static RandomNumberGenerator generator{ };
    return generator;
}