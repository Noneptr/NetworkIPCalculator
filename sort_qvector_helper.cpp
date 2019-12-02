#include "sort_qvector_helper.h"

namespace __sort_qvector_helper__
{
    template <class T>
    void selectUpSortQVector(QVector<T> &vector)
    {
        for (int i = 0; i < vector.size(); i++)
        {
            int max_i = i;

            for (int j = i + 1; j < vector.size(); j++)
            {
                if (vector[max_i] < vector[j])
                {
                    max_i = j;
                }
            }

            if (max_i != i)
            {
                T buf = vector[i];
                vector[i] = vector[max_i];
                vector[max_i] = buf;
            }
        }
    }
}
