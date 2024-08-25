#include "CentripetalMode.h"

CentripetalMode::CentripetalMode(std::shared_ptr<Cmd> cmd,
                                 std::shared_ptr<ICentripetalMode_InfomationGetter> infos)
{
    _cmd = cmd;
    _infos = infos;

    _tension_linear_interpolator = std::shared_ptr<base::LinearInterpolator>{
        new base::LinearInterpolator{
            base::LinearInterpolator_StartVlaue{0},
            base::LinearInterpolator_EndVlaue{_infos->Option_Tension_kg()},
            base::LinearInterpolator_StepLength{0.03},
        },
    };
}

void CentripetalMode::Execute()
{
    _cmd->SetSpeed(_infos->Option_WindingSpeed_rpm());

    _last_tension = _infos->Option_Tension_kg();
    if (_last_tension != _infos->Option_Tension_kg())
    {
        _tension_linear_interpolator->SetEndValue(_infos->Option_Tension_kg());

        _filter = std::shared_ptr<base::ChXFilter>{
            new base::ChXFilter{
                base::ChXFilter_KError{10},
                base::ChXFilter_FeedbackDiv{1000},
            },
        };
    }

    double tension = ++(*_tension_linear_interpolator);
    double torque = tension * _infos->Option_TorqueRatio();

    if (_infos->Servo_FeedbackSpeed() > -50)
    {
        torque = torque / _infos->Option_CentripetalRatio();
    }

    torque = _filter->Input(torque);
    _cmd->SetTorque(torque);
}
