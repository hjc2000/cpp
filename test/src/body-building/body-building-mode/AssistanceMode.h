#pragma once
#include <base/IExecutable.h>
#include <base/math/DirectionDetecter.h>
#include <base/math/LinearInterpolator.h>
#include <Cmd.h>
#include <memory>
#include <Option.h>

class AssistanceMode :
    public base::IExecutable
{
private:
    int _unwinding_tick = 0;
    double _tension = 0;
    bool _has_effective_unwinding = false;
    bool _has_effective_winding = false;
    std::shared_ptr<Cmd> _cmd;

    base::DirectionDetecter _direction_detecter{
        base::DirectionDetecter_RisingThreshold{20},
        base::DirectionDetecter_FallenThreshold{-20},
        base::DirectionDetecter_Direction::Falling,
        0,
    };

    base::LinearInterpolator _tension_linear_interpolator{
        base::LinearInterpolator_StartVlaue{0},
        base::LinearInterpolator_EndVlaue{Option::Instance().Tension_kg()},
        base::LinearInterpolator_StepLength{0.03},
    };

    void OnFromUnwindingToWinding();
    void OnFromWindingToUnwinding();
    double CalSubKg(double base_kg);

public:
    AssistanceMode(std::shared_ptr<Cmd> cmd)
    {
        _cmd = cmd;
    }

    void Execute() override;
};
