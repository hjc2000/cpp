#pragma once
#include <Option.h>
#include <Servo.h>
#include <State.h>

void main_handle()
{
    while (true)
    {
        Servo::Instance().Refresh_FeedbackPosition();
        Option::Instance().Refresh_AdditionalMode();
        Option::Instance().Refresh_BodyBuildingMode();
        Option::Instance().Refresh_Tension_kg();
        State::Instance().Refresh_ReleasedLengthOfLine();
    }
}
