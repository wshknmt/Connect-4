#ifndef PAIRINT64_H
#define PAIRINT64_H

#include <functional>

struct PairInt64 {
    uint64_t boardColor;
    uint64_t boardOccupancy;
};

struct PairInt64Hash {
    std::size_t operator()(const PairInt64& obj) const;
};

struct PairInt64Equal {
    bool operator()(const PairInt64& ths, const PairInt64& oth) const;
};

#endif // PAIRINT64_H
