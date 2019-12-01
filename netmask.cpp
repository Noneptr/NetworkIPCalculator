#include "netmask.h"


NetMask::NetMask(unsigned short count_bits)
{
    setMask(count_bits);
}


NetMask::NetMask(const IPrecord &ip_rec)
{
    setMask(ip_rec);
}


NetMask::NetMask(const QString &ip_rec)
{
    setMask(ip_rec);
}


IPrecord NetMask::mask() const
{
    return __mask;
}


unsigned int NetMask::countHosts() const
{
    if (countBits() == 32)
    {
        return 0;
    }
    else
    {
        return static_cast<unsigned int>(pow(2, 32 - countBits()) - 2);
    }
}


unsigned short NetMask::countBits() const
{
    return __count_bits;
}

void NetMask::setMask(unsigned short count_bits)
{
    if (count_bits < 33)
    {
        __count_bits = count_bits;
        __mask.setIPrecord(0, 0, 0, 0);

        if (count_bits <= 8)
        {
            __mask.setOneOctet(allow_octets[count_bits]);
        }
        else if (count_bits <= 16)
        {
            __mask.setOneOctet(255);
            __mask.setTwoOctet(allow_octets[count_bits - 8]);
        }
        else if (count_bits <= 24)
        {
            __mask.setOneOctet(255);
            __mask.setTwoOctet(255);
            __mask.setThreeOctet(allow_octets[count_bits - 16]);
        }
        else
        {
            __mask.setOneOctet(255);
            __mask.setTwoOctet(255);
            __mask.setThreeOctet(255);
            __mask.setFourOctet(allow_octets[count_bits - 24]);
        }
    }
    else
    {
        throw InvalidCountBitsError;
    }
}


void NetMask::setMask(const IPrecord &ip_rec)
{
    int b1, b2, b3, b4;
    b1 = allow_octets.indexOf(ip_rec.oneOctet());
    b2 = allow_octets.indexOf(ip_rec.twoOctet());
    b3 = allow_octets.indexOf(ip_rec.threeOctet());
    b4 = allow_octets.indexOf(ip_rec.fourOctet());
    if (b1 != -1 && b2 != -1 && b3 != -1 && b4 != -1)
    {
        __mask = ip_rec;
        __count_bits = static_cast<unsigned short>(b1 + b2 + b3 + b4);
    }
    else
    {
        throw InvalidNetMaskError;
    }
}


void NetMask::setMask(const QString &ip_rec)
{
    setMask(IPrecord(ip_rec));
}
