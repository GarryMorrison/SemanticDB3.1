//
// Created by Garry Morrison on 12/09/2020.
//

#ifndef SDB3_1_SUPERPOSITIONITER_H
#define SDB3_1_SUPERPOSITIONITER_H

#include "Ket.h"
#include "Superposition.h"
class Superposition;
class Ket;

class SuperpositionIter : public std::iterator<std::random_access_iterator_tag, Superposition> {
private:
    ulong _pos;
    const Superposition *_p_sp;

public:
    using difference_type = typename std::iterator<std::random_access_iterator_tag, Superposition>::difference_type;

    SuperpositionIter(const Superposition* p_sp, ulong pos) { _pos = pos; _p_sp = p_sp; }

    SuperpositionIter& operator+=(difference_type rhs) { _pos += rhs; return *this; }
    SuperpositionIter& operator-=(difference_type rhs) { _pos -= rhs; return *this; }
    // Ket operator* () const { return _p_sp->get(_pos); }
    // Type* operator->() const;
    // Ket operator[](difference_type rhs) const { return _p_sp->get(rhs); }
    Ket operator* () const;
    Ket operator[](difference_type rhs) const;
    SuperpositionIter& operator=(const SuperpositionIter& rhs) { _pos = rhs._pos; _p_sp = rhs._p_sp; return *this;}

    SuperpositionIter& operator++() { ++_pos; return *this; }
    SuperpositionIter& operator--() { --_pos; return *this; }
    SuperpositionIter operator++(int) { SuperpositionIter tmp(_p_sp, _pos); ++_pos; return tmp; }
    SuperpositionIter operator--(int) { SuperpositionIter tmp(_p_sp, _pos); --_pos; return tmp; }

    difference_type operator-(const SuperpositionIter& rhs) const { return _pos - rhs._pos; }
    SuperpositionIter operator+(difference_type rhs) const { return SuperpositionIter(_p_sp, _pos + rhs); }
    SuperpositionIter operator-(difference_type rhs) const { return SuperpositionIter(_p_sp, _pos - rhs); }
    friend SuperpositionIter operator+(difference_type lhs, const SuperpositionIter& rhs) { return SuperpositionIter(rhs._p_sp, lhs + rhs._pos); }
    friend SuperpositionIter operator-(difference_type lhs, const SuperpositionIter& rhs) { return SuperpositionIter(rhs._p_sp, lhs - rhs._pos); }

    bool operator== (const SuperpositionIter& rhs) const { return _pos == rhs._pos; }
    bool operator!= (const SuperpositionIter& rhs) const { return _pos != rhs._pos; }
    bool operator> (const SuperpositionIter& rhs) const { return _pos > rhs._pos; }
    bool operator< (const SuperpositionIter& rhs) const { return _pos < rhs._pos; }
    bool operator>= (const SuperpositionIter& rhs) const { return _pos >= rhs._pos; }
    bool operator<= (const SuperpositionIter& rhs) const { return _pos <= rhs._pos; }

    // const SuperpositionIter& operator++ ();
    // SuperpositionIter& operator- (const SuperpositionIter& other);
};

#endif //SDB3_1_SUPERPOSITIONITER_H
