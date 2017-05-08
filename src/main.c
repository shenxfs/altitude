/*
 * main.c
 *
 *  Created on: 2017年5月5日
 *      Author: shenxf
 */

#include <stdio.h>
#include "altitude.h"
int main(void)
{
    qfxn_t ps;
    qfxn_t h;
    ps = (qfxn_t)(54.02*PS_ONE);
    h= get_high(ps);
    printf("Ps=%f,H=%f\n",ps/PS_ONE_FLOAT,h/HIGH_ONE_FLOAT);
    h = (qfxn_t)((qfxnw_t)5000*HIGH_ONE);
    ps = get_Ps(h);
    printf("Ps=%f,H=%f\n",ps/PS_ONE_FLOAT,h/HIGH_ONE_FLOAT);
    return 0;
}
