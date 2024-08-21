#pragma once
#include <base/math/HysteresisElement.h>
#include <chrono>

/// @brief 风扇控制器。
class FanControler
{
private:
    FanControler() = default;

    std::chrono::seconds TurnOnFanDelay();
    std::chrono::seconds TurnOffFanDelay();
    int64_t TurnOnDelayTick();
    int64_t TurnOffDelayTick();

    base::HysteresisElement _hysteresis_element{
        static_cast<double>(TurnOnDelayTick()),
        static_cast<double>(TurnOnDelayTick() - TurnOffDelayTick()),
    };

    /// @brief 基于计时的控制方式。
    void ControlByTick();

public:
    static FanControler &Instance()
    {
        static FanControler o;
        return o;
    }

    void Execute();
};