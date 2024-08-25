#include "ModeSelector.h"
#include <AdditionMode.h>
#include <AssistanceMode.h>
#include <BurnOutMode.h>
#include <CalibrateZeroPointMode.h>
#include <CentrifugalMode.h>
#include <CentripetalMode.h>
#include <Cmd.h>
#include <ConstantSpeedMode.h>
#include <IntelligentMode.h>
#include <Option.h>
#include <Servo.h>
#include <SpringMode.h>
#include <StallProtectionMode.h>
#include <StandardMode.h>

void ModeSelector::HandleAfterBodyBuildingMode()
{
}

void ModeSelector::CreateBodyBuildingModeExecutable()
{
    switch (Option::Instance().BodyBuildingMode())
    {
    case Option_BodyBuildingMode::Standard:
        {
            class InfoGetter :
                public IStandardMode_InformationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new StandardMode{
                    _cmd,
                    std::shared_ptr<InfoGetter>{new InfoGetter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::IntelligentMode:
        {
            class Getter :
                public IIntelligentMode_InfomationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_WindingTorque() override
                {
                    return SRV_PARA(1, 34);
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Option_k() override
                {
                    return SRV_PARA(1, 45) / 100.0;
                }

                double Option_b() override
                {
                    return SRV_PARA(1, 46) / 100.0;
                }

                double Option_MaxTorque() override
                {
                    return Option::Instance().MaxTorque();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new IntelligentMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::CentripetalMode:
        {
            class Getter :
                public ICentripetalMode_InfomationGetter
            {
            public:
                /// @brief 向心比例
                /// @note 大于 1 的值。向心模式下，顺从它向心，拉力会减小。减小拉力是通过将转矩除以本系数。
                /// @return
                double Option_CentripetalRatio() override
                {
                    return SRV_PARA(1, 43);
                }

                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new CentripetalMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::CentrifugalMode:
        {
            class Getter :
                public ICentrifugalMode_InfomationGetter
            {
            public:
                /// @brief 离心比例
                /// @note 大于 1 的值。离心模式下，顺从它离心，拉力会减小。减小拉力是通过将转矩除以本系数。
                /// @return
                double Option_CentrifugalRatio() override
                {
                    return SRV_PARA(1, 44);
                }

                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new CentrifugalMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::SpringMode:
        {
            class Getter : public ISpringMode_InfomationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return 50;
                }

                int Option_OneMeterPosition() override
                {
                    return 1000;
                }

                int Servo_FeedbackPosition() override
                {
                    return 100;
                }

                /// @brief 弹簧劲度系数。
                /// @return
                virtual double Option_SpringRatio() override
                {
                    return 1.5;
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new SpringMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter},
                },
            };

            break;
        }
    default:
    case Option_BodyBuildingMode::Standby:
        {
            _body_building_executable = nullptr;
            _cmd->SetSpeed(0);
            _cmd->SetTorque(0);
            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode1:
        {
            _body_building_executable = std::shared_ptr<base::IExecutable>{new ConstantSpeedMode{0, _cmd}};
            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode2:
        {
            _body_building_executable = std::shared_ptr<base::IExecutable>{new ConstantSpeedMode{1, _cmd}};
            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode3:
        {
            _body_building_executable = std::shared_ptr<base::IExecutable>{new ConstantSpeedMode{2, _cmd}};
            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode4:
        {
            _body_building_executable = std::shared_ptr<base::IExecutable>{new ConstantSpeedMode{3, _cmd}};
            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode5:
        {
            _body_building_executable = std::shared_ptr<base::IExecutable>{new ConstantSpeedMode{4, _cmd}};
            break;
        }
    case Option_BodyBuildingMode::BurnOutMode:
        {
            _body_building_executable = std::shared_ptr<base::IExecutable>{new BurnOutMode{_cmd}};
            break;
        }
    case Option_BodyBuildingMode::AssistanceMode:
        {
            class Getter :
                public IAssistanceMode_InfomationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                int Servo_FeedbackPosition() override
                {
                    return Servo::Instance().FeedbackPosition();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new AssistanceMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter},
                },
            };

            break;
        }
    }
}

void ModeSelector::Execute()
{
    if (!CalibrateZeroPointMode::Instance().IsCompleted())
    {
        CalibrateZeroPointMode::Instance().Execute();
        return;
    }

    if (Option::Instance().AdditionalModeCodeChanged())
    {
        AdditionMode::Instance().SetMode(static_cast<AdditionMode_ModeEnum>(Option::Instance().AdditionalMode()));
    }

    if (static_cast<int>(AdditionMode::Instance().Mode()) > 0)
    {
        AdditionMode::Instance().Execute();
        return;
    }

    // 失速保护模式
    if (StallProtectionMode::Instance().StallFlag())
    {
        StallProtectionMode::Instance().Execute();
        return;
    }

    // 健身模式
    if (Option::Instance().BodyBuildingModeChanged())
    {
        CreateBodyBuildingModeExecutable();
    }

    if (_body_building_executable)
    {
        _body_building_executable->Execute();
        HandleAfterBodyBuildingMode();
    }
}
