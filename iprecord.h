#ifndef IPRECORD_H
#define IPRECORD_H

#include <QString>
#include <cmath>

enum IPrecordError {InvalidOneOctetError, InvalidTwoOctetError,
                    InvalidThreeOctetError, InvalidFourOctetError, InvalidIPrecord};


class IPrecord
{
private:
    unsigned short __one_oct = 0;
    unsigned short __two_oct = 0;
    unsigned short __three_oct = 0;
    unsigned short __four_oct = 0;

    static bool isValidQStringForIP(const QString &ip_rec);             // проверка на правильность сотавления QString строки с ip
public:
    IPrecord() = default;
    IPrecord(unsigned short one, unsigned short two, unsigned short three, unsigned short four);
    IPrecord(const QString &ip_rec);

    // =============== методы получения значений октетов ip ===========================
    unsigned short oneOctet() const;
    unsigned short twoOctet() const;
    unsigned short threeOctet() const;
    unsigned short fourOctet() const;
    QString toQString() const;
    unsigned int toUInt() const;
    // ======================================================================

    // =============== методы изменения значений октетов ip ===========================
    void setOneOctet(unsigned short one);
    void setTwoOctet(unsigned short two);
    void setThreeOctet(unsigned short three);
    void setFourOctet(unsigned short four);
    void setIPrecord(unsigned short one, unsigned short two, unsigned short three, unsigned short four);
    void setIPrecord(const QString &ip_rec);
    // ================================================================================

    // ================ операции с ip записями ==========================================
    IPrecord operator & (const IPrecord &obj) const;
    IPrecord operator - (const IPrecord &obj) const;
    IPrecord operator + (const IPrecord &obj) const;

    IPrecord operator + (unsigned int val) const;
    IPrecord operator - (unsigned int val) const;

    bool operator == (const IPrecord &obj) const;
    bool operator < (const IPrecord &obj) const;
    bool operator > (const IPrecord &obj) const;
    bool operator <= (const IPrecord &obj) const;
    bool operator >= (const IPrecord &obj) const;
    bool operator != (const IPrecord &obj) const;

    static unsigned int IPrecordToUInt(const IPrecord &rec);
    static IPrecord uintToIPrecord(unsigned int val);

    friend IPrecord operator + (unsigned int val, const IPrecord &obj);
    // ==================================================================================
};

#endif // IPRECORD_H
