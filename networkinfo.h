#ifndef NETWORKINFO_H
#define NETWORKINFO_H


#include "iprecord.h"
#include "netmask.h"
#include <QMetaType>


enum NetworkInfoError {ValueBusyHostsError};                                // исключения класса NetworkInfoError


class NetworkInfo
{
private:
    IPrecord __network;                                                     // адрес сети
    NetMask __mask;                                                         // маска подсети
    IPrecord __wildcard;                                                    // обратная маска подсети
    IPrecord __broadcast;                                                   // широковещательный адрес сети
    IPrecord __host_min;                                                    // адрес первого хоста
    IPrecord __host_max;                                                    // адрес последнего хоста
    unsigned int __busy_hosts = 0;                                          // количество занятых хостов
public:
    NetworkInfo() = default;
    NetworkInfo(const IPrecord &ip, const NetMask &m);
    NetworkInfo(const IPrecord &_net, const NetMask &_mask,
                const IPrecord &_wildcard, const IPrecord &_broadcast,
                const IPrecord &_host_min, const IPrecord &_host_max,
                unsigned int busy_hosts = 0);
    void calculateNet(const IPrecord &ip, const NetMask &m);                // вычисление параметров сети с помощью ip и маски подсети

    // ================================ Операции изменения данных сетевой информации ===================================
    void setInfo(const IPrecord &_net, const NetMask &_mask,
                 const IPrecord &_wildcard, const IPrecord &_broadcast,
                 const IPrecord &_host_min, const IPrecord &_host_max,
                 unsigned int busy_hosts = 0);
    void setNetwork(const IPrecord &n);
    void setMask(const NetMask &m);
    void setWildcard(const IPrecord &w);
    void setDirectBroadcast(const IPrecord &db);
    void setHostMin(const IPrecord &hmin);
    void setHostMax(const IPrecord &hmax);
    void setBusyHosts(unsigned int busy_hosts);
    //==================================================================================================================

    // ================================ Операции получения данных сетевой информации ===================================
    IPrecord network() const;
    NetMask mask() const;
    IPrecord wildcard() const;
    IPrecord directBroadcast() const;
    IPrecord hostMin() const;
    IPrecord hostMax() const;
    unsigned int busyHosts() const;
    //==================================================================================================================
};

Q_DECLARE_METATYPE(NetworkInfo);                                            // возможность QVariant преобразования

#endif // NETWORKINFO_H
