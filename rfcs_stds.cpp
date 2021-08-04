#include "rfcs_stds.h"

namespace __check_rfc__
{
    bool check_line(const IPrecord &rfc, unsigned short rfc_b, const IPrecord &net_address)
    {
        NetMask RFCm = NetMask(rfc_b);
        return ((net_address >= rfc) && (net_address <= rfc + RFCm.countHosts() + 1));
    }

    bool check_block(const QVector<IPrecord> &rfc, const QVector<unsigned short> &rfc_bs,
                     const IPrecord &net_address)
    {
        for (int i = 0; i < rfc.size(); i++)
        {
            NetMask RFCm = NetMask(rfc_bs[i]);
            if ((net_address >= rfc[i]) && (net_address <= rfc[i] + RFCm.countHosts() + 1))
            {
                return true;
            }
        }
        return false;
    }


    bool check_rfc5737(const IPrecord &net_address)
    {
        NetMask RFC_5737_m = NetMask(RFC_5737_b);
        return (((net_address >= RFC_5737[0]) && (net_address <= RFC_5737[0] + RFC_5737_m.countHosts() + 1)) ||
            ((net_address >= RFC_5737[1]) && (net_address <= RFC_5737[1] + RFC_5737_m.countHosts() + 1)) ||
            ((net_address >= RFC_5737[2]) && (net_address <= RFC_5737[2] + RFC_5737_m.countHosts() + 1)));
    }

    bool check_rfc1918(const IPrecord &net_address)
    {
        return check_block(RFC_1918, RFC_1918_bs, net_address);
    }


    bool check_rfc3068(const IPrecord &net_address)
    {
        return check_line(RFC_3068, RFC_3068_b, net_address);
    }

    bool check_rfc3927(const IPrecord &net_address)
    {
        return check_line(RFC_3927, RFC_3927_b, net_address);
    }

    bool check_rfc1122(const IPrecord &net_address)
    {
        return check_block(RFC_1122, RFC_1122_bs, net_address);
    }


    bool check_rfc2544(const IPrecord &net_address)
    {
        return check_line(RFC_2544, RFC_2544_b, net_address);
    }


    bool check_rfc3171(const IPrecord &net_address)
    {
        return check_line(RFC_3171, RFC_3171_b, net_address);
    }

    bool check_rfc(const IPrecord &net_address)
    {
        return __check_rfc__::check_rfc5737(net_address) ||
               __check_rfc__::check_rfc1122(net_address) ||
               __check_rfc__::check_rfc2544(net_address) ||
               __check_rfc__::check_rfc3068(net_address) ||
               __check_rfc__::check_rfc3171(net_address) ||
               __check_rfc__::check_rfc3927(net_address);
    }

    bool check_rfc_with_error(const IPrecord &net_address)
    {
        if (!__check_rfc__::check_rfc5737(net_address))
        {
            if (!__check_rfc__::check_rfc1122(net_address))
            {
                if (!__check_rfc__::check_rfc2544(net_address))
                {
                    if (!__check_rfc__::check_rfc3068(net_address))
                    {
                        if (!__check_rfc__::check_rfc3171(net_address))
                        {
                            if (!__check_rfc__::check_rfc3927(net_address))
                            {
                                return false;
                            }
                            else
                            {
                                throw __check_rfc__::RFC_3927_ERROR;
                            }
                        }
                        else
                        {
                            throw __check_rfc__::RFC_3171_ERROR;
                        }
                    }
                    else
                    {
                        throw __check_rfc__::RFC_3068_ERROR;
                    }
                }
                else
                {
                    throw __check_rfc__::RFC_2544_ERROR;
                }
            }
            else
            {
                throw __check_rfc__::RFC_1122_ERROR;
            }
        }
        else
        {
            throw __check_rfc__::RFC_5737_ERROR;
        }
    }
}
