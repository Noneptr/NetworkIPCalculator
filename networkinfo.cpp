#include "networkinfo.h"

NetworkInfo::NetworkInfo(const IPrecord &ip, const NetMask &m)
{
    calculateNet(ip, m);
}


NetworkInfo::NetworkInfo(const IPrecord &_net, const NetMask &_mask,
            const IPrecord &_wildcard, const IPrecord &_broadcast,
            const IPrecord &_host_min, const IPrecord &_host_max)
{
    setInfo(_net, _mask, _wildcard, _broadcast, _host_min, _host_max);
}


void NetworkInfo::calculateNet(const IPrecord &ip, const NetMask &m)
{
    __mask = m;
    __network = ip & __mask.mask();
    __wildcard = IPrecord(255, 255, 255, 255) - __mask.mask();
    __broadcast = __network + __wildcard;
    __host_min = __network + 1;
    __host_max = __broadcast - 1;
}


void NetworkInfo::setInfo(const IPrecord &_net, const NetMask &_mask,
             const IPrecord &_wildcard, const IPrecord &_broadcast,
             const IPrecord &_host_min, const IPrecord &_host_max)
{
    setNetwork(_net);
    setMask(_mask);
    setWildcard(_wildcard);
    setDirectBroadcast(_broadcast);
    setHostMin(_host_min);
    setHostMax(_host_max);
}


void NetworkInfo::setNetwork(const IPrecord &n)
{
    __network = n;
}


void NetworkInfo::setMask(const NetMask &m)
{
    __mask = m;
}


void NetworkInfo::setWildcard(const IPrecord &w)
{
    __wildcard = w;
}


void NetworkInfo::setDirectBroadcast(const IPrecord &db)
{
    __broadcast = db;
}


void NetworkInfo::setHostMin(const IPrecord &hmin)
{
    __host_min = hmin;
}


void NetworkInfo::setHostMax(const IPrecord &hmax)
{
    __host_max = hmax;
}


IPrecord NetworkInfo::network() const
{
    return __network;
}


NetMask NetworkInfo::mask() const
{
    return __mask;
}


IPrecord NetworkInfo::wildcard() const
{
    return __wildcard;
}


IPrecord NetworkInfo::directBroadcast() const
{
    return __broadcast;
}


IPrecord NetworkInfo::hostMin() const
{
    return __host_min;
}


IPrecord NetworkInfo::hostMax() const
{
    return __host_max;
}
