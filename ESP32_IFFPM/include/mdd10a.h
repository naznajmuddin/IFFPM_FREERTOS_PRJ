/**
 * @file mdd10a.h
 * @brief Controlling DC Motors
 * @copyright IFFPM SPECTRE
 * @license Apache-2.0
 * @author Nazwa Najmuddin <naznajmuddin@gmail.com>
 */

#include <Arduino.h>

class motor
{

private:
    int digital_pin;
    int analog_pin;
    int channel_num;
    int direction;
    int pwm_val;
    float pwm_val_smooth;
    float pwm_val_prev;

public:
    void init_data(int dig, int ana, int channel)
    {
        digital_pin = dig;
        analog_pin = ana;
        channel_num = channel;

        pinMode(digital_pin, OUTPUT);

        ledcAttachPin(analog_pin, channel_num);
        ledcSetup(channel_num, 20000, 8);
    }

    float speed_control()
    {
        pwm_val_smooth = (pwm_val * 0.06) + (pwm_val_prev * 0.94);
        pwm_val_prev = pwm_val_smooth;
        return (pwm_val_smooth);
    }

    void set_spin(int pwm)
    {
        unsigned long time_now = millis();
        int delay_period = 5;
        pwm_val = pwm;
        float new_pwm = speed_control();
        Serial.println(new_pwm);
        digitalWrite(digital_pin, direction);
        ledcWrite(channel_num, new_pwm);
        while (millis() < time_now + delay_period)
        {
        }
    }

    void set_direction(int dir)
    {
        direction = dir;
    }

    void man_move(int dir, int speed)
    {

        direction = dir;
        digitalWrite(digital_pin, direction);
        ledcWrite(channel_num, speed);
        Serial.println(speed);
    }
};