/*
 * main.c
 *
 *  Created on: 2017年5月5日
 *      Author: shenxf
 */

#include <stdio.h>
#include "altitude.h"
const float fps[] =
{
    101.325,
    89.874562916,
    79.4952019341,
    70.1085264961,
    61.6402137396,
    54.0198881881,
    47.1810021852,
    41.0607170849,
    35.5997852126,
    30.7424326121,
    26.4362425927,
    22.632040095,
    19.3303825081,
    16.5103846732,
    14.1017800317,
    12.0445528072,
    10.287442578,
    8.78666700961,
    7.50483091908,
    6.40999449079,
    5.47487742428,
};
int main(void)
{
    qfxn_t ps;
    qfxn_t h;
    int16_t i;
    for(i=0;i<21;i++)
    {
        ps = (qfxn_t)(fps[i]*PS_ONE+0.5);
        h= get_high(ps);
        printf("Ps=%f,H=%f\n",ps/PS_ONE_FLOAT,h/HIGH_ONE_FLOAT);
    }
    for(i=0;i<21;i++)
    {
        h = (qfxn_t)i*1000*HIGH_ONE;
        ps = get_Ps(h);
        printf("Ps=%f,H=%f\n",ps/PS_ONE_FLOAT,h/HIGH_ONE_FLOAT);
    }
    ps = (qfxn_t)(54.02*PS_ONE);
    h= get_high(ps);
    printf("Ps=%f,H=%f\n",ps/PS_ONE_FLOAT,h/HIGH_ONE_FLOAT);
    h = (qfxn_t)((qfxnw_t)5000*HIGH_ONE);
    ps = get_Ps(h);
    printf("Ps=%f,H=%f\n",ps/PS_ONE_FLOAT,h/HIGH_ONE_FLOAT);
    return 0;
}
