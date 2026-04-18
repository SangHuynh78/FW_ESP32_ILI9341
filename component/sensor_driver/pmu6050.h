#ifndef PMU6050_H
#define PMU6050_H

#include <stdint.h>
#include "i2c_driver.h"

typedef struct {
    i2c_type i2c;
    int i2c_address;
    uint8_t status;
} pmu6050_dev_t;

void PMU6050_Init(pmu6050_dev_t *dev);
void PMU6050_ReadID(pmu6050_dev_t *dev, uint8_t *id);

#endif // PMU6050_H