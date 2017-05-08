/**
 * @file altitude.h
 * @version V1.0.0
 * @date Created on: 2017年5月5日
 * @author  shenxf
 */
#ifndef INCLUDE_ALTITUDE_H_
#define INCLUDE_ALTITUDE_H_
#include <stdint.h>
#if defined(FIXED32)
typedef int32_t qfxn_t;
typedef int64_t qfxnw_t;
typedef uint32_t uqfxn_t;
typedef uint64_t uqfxnw_t;
#define PS_ONE_FLOAT     (16777216.0) 
#define HIGH_ONE_FLOAT   (32768.0) 
#define PS_ONE     (0x1000000L) 
#define HIGH_ONE   (0x8000L) 
#else
typedef int16_t qfxn_t;
typedef int32_t qfxnw_t;
typedef uint16_t uqfxn_t;
typedef uint32_t uqfxnw_t;
#define PS_ONE_FLOAT     (256.0) 
#define HIGH_ONE_FLOAT   (1.0) 
#define PS_ONE     (0x100) 
#define HIGH_ONE   (0x1) 
#endif
qfxn_t get_Ps(qfxn_t high);
qfxn_t get_high(qfxn_t ps);

#endif /* INCLUDE_ALTITUDE_H_ */
