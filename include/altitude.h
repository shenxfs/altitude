/**
 * @file altitude.h
 * @version V1.0.0
 * @date Created on: 2017年5月5日
 * @author  shenxf
 */
#ifndef INCLUDE_ALTITUDE_H_
#define INCLUDE_ALTITUDE_H_
#include <stdint.h>

typedef int16_t qfxn_t;
typedef int32_t qfxnw_t;
typedef uint16_t uqfxn_t;
typedef uint32_t uqfxnw_t;

qfxn_t get_Ps(qfxn_t high);
qfxn_t get_high(qfxn_t ps);

#endif /* INCLUDE_ALTITUDE_H_ */
