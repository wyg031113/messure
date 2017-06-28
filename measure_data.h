#ifndef MEASURE_DATA_H
#define MEASURE_DATA_H
class MeasureData
{
public:
    double zero_voltage;
    double in_resis;
    double out_resis;
    double ins_resis;
    double self_test_voltage;
    bool   pressure_messuring;
    double pressure;
    double pressure_tm10;
    double temperature;

    double pk;
    double pb;
};


#endif // MEASURE_DATA_H

