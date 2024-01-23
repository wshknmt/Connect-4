#ifndef PAIRINT64_H
#define PAIRINT64_H

#include <functional>
#include <utility>

struct PairInt64 {
    uint64_t boardColor;
    uint64_t boardOccupancy;
    constexpr auto operator<=>(const PairInt64&) const = default;
};

struct PairInt64Hash {
    std::size_t operator()(const PairInt64& obj) const;
};

struct PairInt64Equal {
    bool operator()(const PairInt64& ths, const PairInt64& oth) const;
};

template<>
struct std::hash<PairInt64> {
    std::size_t operator()(const PairInt64& obj) const noexcept {
        return std::hash<uint64_t>{}(obj.boardColor) ^ (std::hash<uint64_t>{}(obj.boardOccupancy) << 1);
    }
};

#endif // PAIRINT64_H
