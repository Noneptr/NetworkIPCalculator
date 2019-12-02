#ifndef SORT_QVECTOR_HELPER_H
#define SORT_QVECTOR_HELPER_H

#include <QVector>

namespace __sort_qvector_helper__
{
    template <class T>
    void selectUpSortQVector(QVector<T> &vector);
}

#include "sort_qvector_helper.cpp"

#endif // SORT_QVECTOR_HELPER_H
