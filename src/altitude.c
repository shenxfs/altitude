/**
 * @file altitude.c
 * @date Created on: 2017年5月5日
 * @author shenxf
 */
#include "altitude.h"

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

#if defined (FIXED32)
#define ONE     0x40000000L /**< 1,Q=30*/
#define LN2     0x2C5C85FEL /**< LN2,Q=30*/
#define LAYOUTS 6
#define CHIGH   (0x80000000LL)
#define CGNBR   (0x2000000L)   
#define CGNTR   (0x10000000LL)
#define CBTRG   (0x200000000LL)
const atmos_t coef[LAYOUTS] =
{
    {
        .Pb = 0x65533333L,/*101.325kPa,Q=24*/
        .Hb = 0,
        .Beta = 0xA162DBD6L,/*(-0.0065)/288.15 = -2.25576956446e-05 , Q=46*/
        .Ca = 0x0A8302ADL,/*-gn/(-0.0065)/R = 5.25587981272,Q=25*/
        .Cb = 0x616A2A57L,/*(-0.0065)*R/(-gn) = 0.190263102589,Q=33*/
    },
    {
        .Pb = 0x16A1CD61L,/*22.632040095kPa,Q=24*/
        .Hb = 0x157C0000L,/*11000m,Q=15*/
        .Beta = 0L,
        .Ca = 0xAD536791L,/*-gn/R/216.65 = -0.000157688524268,Q = 43*/
        .Cb = 0x9CE989A9L,/*-216.65*R/gn = -6341.61556551,Q=18 */
    },
    {
        .Pb = 0x5799191L,/*5.47487742428kPa,Q=24*/
        .Hb = 0x27100000L,/*20000m,Q=15*/
        .Beta = 0x135C1CDCL,/*0.001/216.65=4.61573967228e-06,Q=46*/
        .Ca = 0xBBAC6E96L,/*-gn/(0.001)/R=-34.1632187827,Q=25*/
        .Cb = 0xF1035BF3L,/*(-0.001)*R/gn=-0.029271246552,Q=33*/
    },
    {
        .Pb = 0xDE3648L,/* 0.868015776618kPa,Q=24*/
        .Hb = 0x3E800000L,/*32000m,Q=15*/
        .Beta = 0x335CD144L,/*(0.0028/228.65=1.22457905095e-05,Q=46*/
        .Ca = 0xE79902EDL,/*-gn/(0.0028/R=-12.2011495652*/
        .Cb = 0xD60967DBL,/*-0.0819594903461*/
    },
    {
        .Pb = 0x1C6452L,/* 0.110905773367kPa,Q=24*/
        .Hb = 0x5BCC0000L,/* 47000m,Q=15*/
        .Beta = 0L,
        .Ca = 0xBDD22862L,/*-gn/R/270.65 = -0.000126226561177,Q=43*/
        .Cb = 0x8436F2D0L,/*-270.65*R/gn = -7922.26287932,Q=18*/
    },
    {
        .Pb = 0x1122E2L,/*0.066938528121kPa,Q=24*/
        .Hb = 0x639C0000L,/*51000m,Q=15*/
        .Beta = 0xD49BA457L,/*-1.03454646222e-05*/
        .Ca = 0x1866FD13L,/*-gn/R/(-0.0028) = 12.2011495652,Q=25*/
        .Cb = 0x29F69825L,/*(-0.0028*R/gn = 0.0819594903458,Q=33*/
    }
};
#else
#define ONE     0x4000 /**< 1,Q=14*/
#define LN2     0x2C5D /**< LN2,Q=14*/
#define LAYOUTS 4
#define CHIGH   (0x10000L)
#define CGNBR   (0x200)
#define CGNTR   (0x2000)
#define CBTRG   (0x10000L)
const atmos_t coef[LAYOUTS] =
{
    {
        .Pb = 0x6553,/*101.325kPa,Q=8*/
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
    }
};
#endif

qfxn_t get_Ps(qfxn_t high)
{
    qfxn_t ps;
    qfxn_t tmp;
    qfxn_t tn;
    int16_t i = LAYOUTS-1;
    int16_t m;
    const atmos_t *p;
    
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
    if((high == p->Hb)||(p->Hb == coef[LAYOUTS-1].Hb))
    {
        ps = p->Pb;
    } 
    else 
    {
        if(p->Beta != 0)
        {
            tmp = (qfxn_t)((qfxnw_t)p->Beta*(high-p->Hb)/CHIGH);
            tn = (qfxn_t)(-(qfxnw_t)tmp*tmp/2/ONE);
            ps = tmp;
            i = 1;
            while(tn != 0)
            {
                ps += tn;
                tn =(qfxnw_t)tn*(-((qfxnw_t)tmp*(i + 1))/(i + 2))/ONE;
                i++;
            }
            tmp =(qfxn_t)((qfxnw_t)ps*p->Ca/CGNBR);
        }
        else
        {
            tmp = (qfxn_t)((qfxnw_t)p->Ca*(high-p->Hb)/CGNTR);
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
    const atmos_t *p;
    i = LAYOUTS-1;
    while(ps > coef[i].Pb)
    {
        i--;
        if(i == 0)
        {
            break;
        }
    }
    p = &coef[i];
    if((ps == p->Pb)||(p->Pb == coef[LAYOUTS-1].Pb))
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
        tmp = (qfxn_t)((qfxnw_t)high*p->Cb/CBTRG);
        if(p->Beta != 0)
        {
#if !defined(FIXED32)
            tmp /= 2;
#endif            
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
#if defined(FIXED32)            
            high = ((qfxnw_t)high*0x40000000L)/(p->Beta/2);
#else
            high = ((qfxnw_t)high*0x4000L)/(p->Beta/4);
#endif
            high += p->Hb;
        }
        else
        {            
            high = p->Hb + tmp;
        }
    }
    return high;
}
