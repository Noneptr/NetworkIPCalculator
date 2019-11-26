#include "iprecord.h"

IPrecord::IPrecord(unsigned short one, unsigned short two, unsigned short three, unsigned short four)
{
    setIPrecord(one, two, three, four);
}


IPrecord::IPrecord(const QString &ip_rec)
{
    setIPrecord(ip_rec);
}


unsigned short IPrecord::oneOctet() const
{
    return __one_oct;
}


unsigned short IPrecord::twoOctet() const
{
    return __two_oct;
}


unsigned short IPrecord::threeOctet() const
{
    return __three_oct;
}


unsigned short IPrecord::fourOctet() const
{
    return __four_oct;
}


void IPrecord::setOneOctet(unsigned short one)
{
    if (one < 256)
    {
        __one_oct = one;
    }
    else
    {
        throw InvalidOneOctetError;
    }
}


void IPrecord::setTwoOctet(unsigned short two)
{
    if (two < 256)
    {
        __two_oct = two;
    }
    else
    {
        throw InvalidTwoOctetError;
    }
}


void IPrecord::setThreeOctet(unsigned short three)
{
    if (three < 256)
    {
        __three_oct = three;
    }
    else
    {
        throw InvalidThreeOctetError;
    }
}


void IPrecord::setFourOctet(unsigned short four)
{
    if (four < 256)
    {
        __four_oct = four;
    }
    else
    {
        throw InvalidFourOctetError;
    }
}


void IPrecord::setIPrecord(unsigned short one, unsigned short two, unsigned short three, unsigned short four)
{
    setOneOctet(one);
    setTwoOctet(two);
    setThreeOctet(three);
    setFourOctet(four);
}


void IPrecord::setIPrecord(const QString &ip_rec)
{
    if (ip_rec.size() < 16)
    {
        int i = 0;
        int j = 0;
        QString num = "";
        while (i < ip_rec.size())
        {
            if (ip_rec[i] == QChar('.') or i == ip_rec.size() - 1)
            {
                if (i == ip_rec.size() - 1)
                {
                    num += ip_rec[i];
                }
                unsigned short n = num.toUShort();
                if (j == 0)
                {
                    setOneOctet(n);
                }
                else if (j == 1)
                {
                    setTwoOctet(n);
                }
                else if (j == 2)
                {
                    setThreeOctet(n);
                }
                else
                {
                    setFourOctet(n);
                }
                j++;
                num = "";
            }
            else
            {
                num += ip_rec[i];
            }
            i++;
        }
    }
    else

    {
        throw InvalidIPrecord;
    }
}

IPrecord IPrecord::operator & (const IPrecord &obj)
{
    return IPrecord(this->oneOctet() & obj.oneOctet(),
             this->twoOctet() & obj.twoOctet(),
             this->threeOctet() & obj.threeOctet(),
             this->fourOctet() & obj.fourOctet());
}


QString IPrecord::toQString() const
{
    return QString::number(__one_oct) + "."
            + QString::number(__two_oct) + "."
            + QString::number(__three_oct) + "."
            + QString::number(__four_oct);
}
