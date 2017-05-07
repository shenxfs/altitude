/**
 * @file altitude.c
 * @date Created on: 2017年5月5日
 * @author shenxf
 */
#include "altitude.h"

#define ONE    0x4000 /**< 1,Q=30*/
#define LN2    0x2C5D /**< LN2,Q=30*/

/**
 * @struct atmosphere
 *   gn = 9.80665 R = 287.05287
 */
typedef struct atmosphere
{
    qfxn_t Pb;      /**< layout static press(kPa),Q=8 */
    qfxn_t Hb;      /**< layout altitude(m),Q=0*/
    qfxn_t Beta;    /**< constatnt B/Tb ,Q=30 */
    qfxn_t Ca;      /**< constant :Beta? -gn/Beta/R(Q= 9):-gn/Tb/R(Q=27) */
    qfxn_t Cb;      /**< reciprocal of Ca,Beta?(Q = 17):(Q=2) */
}atmos_t;

atmos_t coef[4] =
{
    {
        .Pb = 0x6553L,/*101.325kPa,Q=8*/
        .Hb = 0,
        .Beta = 0xA163,/*(-0.0065)/288.15 = -2.25576956446e-05 , Q=30*/
        .Ca = 0x0A83,/*-gn/(-0.0065)/R = 5.25587981272,Q=9*/
        .Cb = 0x616A,/*(-0.0065)*R/(-gn) = 0.190263102589,Q=17*/
    },
    {
        .Pb = 0x16A2,/*22.632040095kPa,Q=8*/
        .Hb = 0x2AF8,/*11000m,Q=0*/
        .Beta = 0,
        .Ca = 0xAD53,/*-gn/R/216.65 = -0.000157688524268,Q = 27*/
        .Cb = 0x9CEA,/*-216.65*R/gn = -6341.61556551,Q=2 */
    },
    {
        .Pb = 0x57A,/*5.47487742428kPa,Q=24*/
        .Hb = 0x4E20,/*20000m,Q=15*/
        .Beta = 0x135C,/*0.001/216.65=4.61573967228e-06,Q=30*/
        .Ca = 0xBBAC,/*-gn/(0.001)/R=-34.1632187827,Q=9*/
        .Cb = 0xF103,/*(-0.001)*R/gn=-0.029271246552,Q=17*/
    },
    {
        .Pb = 0xDE,/* 0.868015776618kPa,Q=8*/
        .Hb = 0x7D00,/*32000m,Q=0*/
        .Beta = 0x335D,/*(0.0028/228.65=1.22457905095e-05,Q=30*/
        .Ca = 0xE799,/*-gn/(0.0028/R=-12.2011495652,Q=9*/
        .Cb = 0xD609,/*-0.0819594903461,Q=17*/
    },
};

qfxn_t get_Ps(qfxn_t high)
{
    qfxn_t ps;
    qfxn_t tmp;
    qfxn_t tn;
    int16_t i = 3;
    int16_t m;
    atmos_t *p;
    
    /*Get the atmosphere */
    while(high < coef[i].Hb)
    {
        i--;
        if(i == 0)
        {
            break;
        }
    }
    p = &coef[i];
    
    /*Cal altitude*/
    if((high == p->Hb)||(p->Hb == coef[3].Hb))
    {
        ps = p->Pb;
    } 
    else 
    {
        if(p->Beta != 0)
        {
            tmp = (qfxn_t)((qfxnw_t)p->Beta*(high-p->Hb)/0x10000L);
            tn = (qfxn_t)(-(qfxnw_t)tmp*tmp/2/ONE);
            ps = tmp;
            i = 1;
            while(tn != 0)
            {
                ps += tn;
                tn =(qfxnw_t)tn*(-((qfxnw_t)tmp*(i + 1))/(i + 2))/ONE;
                i++;
            }
            tmp =(qfxn_t)((qfxnw_t)ps*p->Ca/0x200);
        }
        else
        {
            tmp = (qfxn_t)((qfxnw_t)p->Ca*(high-p->Hb)/0x2000);
        }
        m = tmp/LN2;
        tmp = (qfxn_t)(((qfxnw_t)tmp*ONE -(qfxnw_t)LN2*ONE*m)/ONE);
        if(tmp<0)
        {
            m--;
            tmp += LN2;
        }
        ps = ONE;
        tn = tmp;
        i = 1;
        while(tn != 0)
        {
            ps += tn;
            tn =(qfxn_t)((qfxnw_t)tn*tmp/(i+1)/ONE);
            i++;
        }
        while(m>0)
        {
            m--;
            ps *= 2;
        }
        while(m<0)
        {
            m++;
            ps /= 2;
        }
        ps = ((qfxnw_t)ps*p->Pb)/ONE;
    }
    return ps;
}

qfxn_t get_high(qfxn_t ps)
{
    qfxn_t high;
    qfxn_t tmp;
    qfxn_t tn;
    int16_t i;
    int16_t m;
    atmos_t *p;
    i = 3;
    while(ps > coef[i].Pb)
    {
        i--;
        if(i == 0)
        {
            break;
        }
    }
    p = &coef[i];
    if((ps == p->Pb)||(p->Pb == coef[3].Pb))
    {
        high = p->Hb;
    }
    else
    {
        tmp = (qfxnw_t)ps*ONE/p->Pb;/*Ps/Pb*/

        /* Ps/Pb = 1 + z,|z|<1, z -> tmp*/
        if(tmp < ONE)
        {
            tmp = tmp - ONE;
        }
        else
        {
            tmp = ONE - tmp;
        }

        /* ln(1+z) */
        tn = -((qfxnw_t)tmp*tmp)/ONE/2;
        i = 1;
        high  = tmp;
        while(tn != 0)
        {
            high += tn;
            tn =(qfxnw_t)tn*(-((qfxnw_t)tmp*(i + 1))/(i + 2))/ONE;
            i++;
        }

        /* B ==0,-Tb*R/gn*ln(Ps/Pb),Q=16-16=0 */
        /* B !=0,-B*R/gn*ln(Ps/Pb),Q=31-16=15*/
        tmp = (qfxn_t)((qfxnw_t)high*p->Cb/0x10000L);
        if(p->Beta != 0)
        {
            tmp /= 2;
            m = tmp / LN2;
            tmp = tmp - m*LN2;
            if(tmp < 0)
            {
                m--;
                tmp += LN2;
            }
            high = ONE;
            i = 1;
            tn = tmp;
            while(tn != 0)
            {
                high += tn;
                tn = (qfxnw_t)tn*tmp/(i+1)/ONE;
                i++;
            }
            while((m<0))
            {
                high /= 2;
                m++;
            }
            while((m > 0))
            {
                high *= 2;
                m--;
            }
            high -= ONE;
            high = ((qfxnw_t)high*0x4000L)/(p->Beta/4);
            high += p->Hb;
        }
        else
        {            
            high = p->Hb + tmp;
        }
    }
    return high;
}
