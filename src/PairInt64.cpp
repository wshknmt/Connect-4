#include "PairInt64.h"

std::size_t PairInt64Hash::operator()(const PairInt64& obj) const {
    return std::hash<uint64_t>()(obj.boardColor) ^ (std::hash<uint64_t>()(obj.boardOccupancy) << 1);
}

bool PairInt64Equal::operator()(const PairInt64& ths, const PairInt64& oth) const {
    return ths.boardColor == oth.boardColor && ths.boardOccupancy == oth.boardOccupancy;
}
