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
    qfxn_t ps = 0x3605;
    qfxn_t h = get_high(ps);
    printf("Ps=%f,H=%f\n",ps/256.0,(float)h);
    h = 5000;
    ps = get_Ps(h);
    printf("Ps=%f,H=%f\n",ps/256.0,(float)h);
    return 0;
}
