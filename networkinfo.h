#ifndef NETWORKINFO_H
#define NETWORKINFO_H


#include "iprecord.h"
#include "netmask.h"
#include <QMetaType>


class NetworkInfo
{
private:
    IPrecord __network;                                                     // адрес сети
    NetMask __mask;                                                         // маска подсети
    IPrecord __wildcard;                                                    // обратная маска подсети
    IPrecord __broadcast;                                                   // широковещательный адрес сети
    IPrecord __host_min;                                                    // адрес первого хоста
    IPrecord __host_max;                                                    // адрес последнего хоста
public:
    NetworkInfo() = default;
    NetworkInfo(const IPrecord &ip, const NetMask &m);
    NetworkInfo(const IPrecord &_net, const NetMask &_mask,
                const IPrecord &_wildcard, const IPrecord &_broadcast,
                const IPrecord &_host_min, const IPrecord &_host_max);
    void calculateNet(const IPrecord &ip, const NetMask &m);                // вычисление параметров сети с помощью ip и маски подсети
    void setInfo(const IPrecord &_net, const NetMask &_mask,
                 const IPrecord &_wildcard, const IPrecord &_broadcast,
                 const IPrecord &_host_min, const IPrecord &_host_max);
    void setNetwork(const IPrecord &n);
    void setMask(const NetMask &m);
    void setWildcard(const IPrecord &w);
    void setDirectBroadcast(const IPrecord &db);
    void setHostMin(const IPrecord &hmin);
    void setHostMax(const IPrecord &hmax);

    IPrecord network() const;
    NetMask mask() const;
    IPrecord wildcard() const;
    IPrecord directBroadcast() const;
    IPrecord hostMin() const;
    IPrecord hostMax() const;
};

Q_DECLARE_METATYPE(NetworkInfo);                                            // возможность QVariant преобразования

#endif // NETWORKINFO_H
