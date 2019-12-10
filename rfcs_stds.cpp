#include "rfcs_stds.h"

namespace __check_rfc__
{
    bool check_rfc5737(const IPrecord &net_address)
    {
        NetMask RFC_5737_m = NetMask(RFC_5737_b);
        return ((net_address >= RFC_5737[0]) && (net_address <= RFC_5737[0] + RFC_5737_m.countHosts() + 1)) ||
            ((net_address >= RFC_5737[1]) && (net_address <= RFC_5737[1] + RFC_5737_m.countHosts() + 1)) ||
            ((net_address >= RFC_5737[2]) && (net_address <= RFC_5737[2] + RFC_5737_m.countHosts() + 1));
    }

    bool check_rfc1918(const IPrecord &net_address)
    {
        for (int i = 0; i < RFC_1918.size(); i++)
        {
            NetMask RFC_1918_m = NetMask(RFC_1918_bs[i]);
            if ((net_address >= RFC_1918[i]) && (net_address <= RFC_1918[i] + RFC_1918_m.countHosts() + 1))
            {
                return true;
            }
        }
        return false;
    }


    bool check_rfc3068(const IPrecord &net_address)
    {
        NetMask RFC_3068_m = NetMask(RFC_3068_b);
        return ((net_address >= RFC_3068) && (net_address <= RFC_3068 + RFC_3068_m.countHosts() + 1));
    }

    bool check_rfc3927(const IPrecord &net_address)
    {
        NetMask RFC_3927_m = NetMask(RFC_3927_b);
        return ((net_address >= RFC_3927) && (net_address <= RFC_3927 + RFC_3927_m.countHosts() + 1));
    }

    bool check_rfc1122(const IPrecord &net_address)
    {
        for (int i = 0; i < RFC_1122.size(); i++)
        {
            NetMask RFC_1122_m = NetMask(RFC_1122_bs[i]);
            if ((net_address >= RFC_1122[i]) && (net_address <= RFC_1122[i] + RFC_1122_m.countHosts() + 1))
            {
                return true;
            }
        }
        return false;
    }


    bool check_rfc2544(const IPrecord &net_address)
    {
        NetMask RFC_2544_m = NetMask(RFC_2544_b);
        return ((net_address >= RFC_2544) && (net_address <= RFC_2544 + RFC_2544_m.countHosts() + 1));
    }


    bool check_rfc3171(const IPrecord &net_address)
    {
        NetMask RFC_3171_m = NetMask(RFC_3171_b);
        return ((net_address >= RFC_3171) && (net_address <= RFC_3171 + RFC_3171_m.countHosts() + 1));
    }
}
