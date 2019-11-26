#ifndef IPRECORD_H
#define IPRECORD_H

#include <QString>

enum IPrecordError {InvalidOneOctetError, InvalidTwoOctetError,
                    InvalidThreeOctetError, InvalidFourOctetError, InvalidIPrecord};


class IPrecord
{
private:
    unsigned short __one_oct;
    unsigned short __two_oct;
    unsigned short __three_oct;
    unsigned short __four_oct;
public:
    IPrecord(unsigned short one = 255, unsigned short two = 255, unsigned short three = 255 , unsigned short four = 255);
    IPrecord(const QString &ip_rec);

    unsigned short oneOctet() const;
    unsigned short twoOctet() const;
    unsigned short threeOctet() const;
    unsigned short fourOctet() const;

    void setOneOctet(unsigned short one);
    void setTwoOctet(unsigned short two);
    void setThreeOctet(unsigned short three);
    void setFourOctet(unsigned short four);
    void setIPrecord(unsigned short one, unsigned short two, unsigned short three, unsigned short four);
    void setIPrecord(const QString &ip_rec);

    IPrecord operator & (const IPrecord &obj);

    QString toQString() const;
};

#endif // IPRECORD_H
