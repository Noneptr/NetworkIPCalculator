#ifndef NETMASK_H
#define NETMASK_H

#include "iprecord.h"
#include <cmath>
#include <QVector>

const QVector<unsigned short> allow_octets = {0, 128, 192, 224, 240, 248, 252, 254, 255};           // вектор возможных значений для октетов маски
                                                                                                    // индекс вектора - количество бит
                                                                                                    // значение по этому индексу - это значение октета

enum NetMaskError {InvalidCountBitsError, InvalidNetMaskError};

class NetMask
{
private:
    IPrecord __mask;                                // маска подсети в виде ip записи
    unsigned short __count_bits;                    // количество бит маски
public:
    NetMask(unsigned short count_bits = 8);
    NetMask(const IPrecord &ip_rec);
    NetMask(const QString &ip_rec);

    void setMask(unsigned short count_bits);
    void setMask(const IPrecord &ip_rec);
    void setMask(const QString &ip_rec);

    IPrecord mask() const;
    unsigned int countHosts() const;
    unsigned short countBits() const;
};

#endif // NETMASK_H
