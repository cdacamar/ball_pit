#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class Stopwatch
{
public:
    using Clock = std::chrono::high_resolution_clock;

    void start()
    {
        start_ = Clock::now();
    }

    void stop()
    {
        stop_  = Clock::now();
    }

    Clock::duration ticks() const
    {
        return stop_ - start_;
    }

    // helpers
    template <typename Tick>
    Tick to_ticks() const
    {
        return std::chrono::duration_cast<Tick>(ticks());
    }

    std::chrono::milliseconds to_ms() const
    {
        return to_ticks<std::chrono::milliseconds>();
    }

private:
    Clock::time_point start_ = { };
    Clock::time_point stop_ = { };
};

#endif // STOPWATCH_H