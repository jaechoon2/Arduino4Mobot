#ifndef __mobotpower_h
#define __mobotpower_h

#define ADC_MSR_COUNTER 32

//definicje dla pomiaru
#define MEASURE_1       1 //Left Motors Current
#define MEASURE_2       2 //Right Motors Current
#define MEASURE_3       3 //VBat
#define MEASURE_DUMMY   0 //Zeby ilosc pomiarow dopelnic do liczy podzielnej przez 2

static signed int measure=0;
static unsigned char ADC_channel = MEASURE_1;
static unsigned char measurescnt = ADC_MSR_COUNTER;

class Measure
{
    public:
        Measure(void);
        void startConversion(void);
        void turnOn(void);
        void turnOff(void);

};

class MotorsCurrent
{
    public:
        MotorsCurrent(void);
        void setCurrent(uint8_t motor, uint16_t value);
        uint16_t getCurrent(uint8_t motor);
    private:
        volatile uint16_t currentMotor1;
        volatile uint16_t currentMotor2;

};

class Battery
{
    public:
        Battery(void);
        void setVoltage(float voltage);
        float getVoltage(void);
    private:
        volatile float vbatt;
};

extern MotorsCurrent MotorsCMeasure;
extern Measure AdcMeasure;
extern Battery VBat;
#endif