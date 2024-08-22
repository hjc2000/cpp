#include "CentripetalMode.h"
#include <Cmd.h>
#include <Option.h>
#include <Servo.h>
#include <TensionLinearInterpolator.h>

void CentripetalMode::Execute()
{
    Cmd::Instance().SetSpeed(Option::Instance().WindingSpeed());

    if (Option::Instance().BodyBuildingModeChanged() || Option::Instance().Tension_kg_Changed())
    {
        TensionLinearInterpolator::Instance().ChangeEndValue(Option::Instance().Tension_kg());

        _filter = std::shared_ptr<base::InertialElement>{
            new base::InertialElement{
                base::InertialElement_TimeConstant{0.025},
                base::InertialElement_SampleInterval{0.002},
            },
        };
    }

    double tension = TensionLinearInterpolator::Instance().StepForward();
    double torque = tension * Option::Instance().TorqueRatio();

    if (Servo::Instance().FeedbackSpeed() < 50)
    {
        torque = torque / Ratio();
    }

    torque = _filter->Input(torque);
    Cmd::Instance().SetTorque(torque);
}
