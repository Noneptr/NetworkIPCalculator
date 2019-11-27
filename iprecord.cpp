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


bool IPrecord::isValidQStringForIP(const QString &ip_rec)
{
    if (ip_rec.size() > 6 && ip_rec.size() < 16 && ip_rec.count(".") == 3)
    {
        if (ip_rec[0] != QChar('.') && ip_rec[ip_rec.size() - 1] != QChar('.'))
        {
            int i = 0;
            while (i < ip_rec.size())
            {
                if (i < ip_rec.size() - 1)
                {
                    if (ip_rec[i] == QChar('.') && ip_rec[i + 1] == QChar('.'))
                    {
                        return false;
                    }
                }
                i++;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
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
    if (IPrecord::isValidQStringForIP(ip_rec))                  // проверка на правильность оформления строки
    {
        int i = 0;                                              // счетчик сиволов строки
        int j = 0;                                              // счётчик пройденных октетов
        QString num = "";
        while (i < ip_rec.size())
        {
            if (ip_rec[i] == QChar('.') or i == ip_rec.size() - 1)          // если встретили '.' или достигли конца строки
            {
                if (i == ip_rec.size() - 1)                                 // если достигли конца строки, то последний символ числа нужно считать
                                                                            // т.к. он ещё не был считан
                {
                    num += ip_rec[i];
                }

                unsigned short n = num.toUShort();

                if (j == 0)                                                 // установка считанного значения октета
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
                num += ip_rec[i];                                   // посимвольное считывание числа
            }
            i++;
        }
    }
    else
    {
        throw InvalidIPrecord;
    }
}

IPrecord IPrecord::operator & (const IPrecord &obj) const
{
    return IPrecord(this->oneOctet() & obj.oneOctet(),
             this->twoOctet() & obj.twoOctet(),
             this->threeOctet() & obj.threeOctet(),
             this->fourOctet() & obj.fourOctet());
}


IPrecord IPrecord::operator - (const IPrecord &obj) const
{
    return IPrecord(this->oneOctet() - obj.oneOctet(),
                    this->twoOctet() - obj.twoOctet(),
                    this->threeOctet() - obj.threeOctet(),
                    this->fourOctet() - obj.fourOctet());
}


IPrecord IPrecord::operator + (const IPrecord &obj) const
{
    return IPrecord(this->oneOctet() + obj.oneOctet(),
                    this->twoOctet() + obj.twoOctet(),
                    this->threeOctet() + obj.threeOctet(),
                    this->fourOctet() + obj.fourOctet());
}


IPrecord IPrecord::operator + (unsigned short val) const
{
    IPrecord res;
    unsigned short vfour = this->fourOctet() + val;
    res.setFourOctet(vfour % 256);
    unsigned short vthree = this->threeOctet() + vfour / 256;
    res.setThreeOctet(vthree % 256);
    unsigned short vtwo = this->twoOctet() + vthree / 256;
    res.setTwoOctet(vtwo % 256);
    unsigned short vone = this->oneOctet() + vtwo / 256;
    res.setOneOctet(vone % 256);
    return res;
}


IPrecord operator + (unsigned short val, const IPrecord &obj)
{
    return obj + val;
}


IPrecord IPrecord::operator - (unsigned short val) const
{
    IPrecord res;
    short vfour = static_cast<short>(this->fourOctet() - val);
    res.setFourOctet(vfour < 0? 256 - (abs(vfour) % 256): static_cast<unsigned short>(vfour));
    short vthree = this->threeOctet() + vfour/ 256;
    vthree = vfour < 0? vthree - 1: vthree;
    res.setThreeOctet(vthree < 0? 256 - (abs(vthree) % 256): static_cast<unsigned short>(vthree));
    short vtwo = this->twoOctet() + vthree/ 256;
    vtwo = vthree < 0? vtwo - 1: vtwo;
    res.setTwoOctet(vtwo < 0? 256 - (abs(vtwo) % 256): static_cast<unsigned short>(vtwo));
    short vone = this->oneOctet() + vtwo/ 256;
    vone = vtwo < 0? vone - 1: vone;
    res.setOneOctet(vone < 0? 256 - (abs(vone) % 256): static_cast<unsigned short>(vone));
    return res;
}


QString IPrecord::toQString() const
{
    return QString::number(__one_oct) + "."
            + QString::number(__two_oct) + "."
            + QString::number(__three_oct) + "."
            + QString::number(__four_oct);
}
