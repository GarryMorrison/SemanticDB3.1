//
// Created by Garry Morrison on 12/09/2020.
//
#include "SuperpositionIter.h"

Ket SuperpositionIter::operator* () const {
    return _p_sp->get(_pos);
}

Ket SuperpositionIter::operator[](difference_type rhs) const {
    return _p_sp->get(rhs);
}
