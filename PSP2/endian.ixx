#include<algorithm>
#include<bit>

export module endian;

template<typename T, size_t size>
concept byte_size = (sizeof(T) == size);

export template <typename T>
T byte_swap(T val) {
    union {
        T val;
        uint8_t bytes[sizeof(T)];
    } buffer{ .val = val };

    std::reverse(std::begin(buffer.bytes), std::end(buffer.bytes));
    return buffer.val;
}


export template<byte_size<32> T>
T byte_swap(T v) {
    auto [a, b, c, d] = *reinterpret_cast<decltype(
        [] {  struct { uint8_t a, b, c, d; } *s; return s; }()) > (&v);
    return from_bytes<T>(d, c, b, a);
}


export template <typename TTarget, typename TFirst, typename... Args>
TTarget from_bytes(TFirst val, Args... args) requires (((sizeof...(Args) + 1) * sizeof(TFirst)) == sizeof(TTarget)) and std::conjunction_v<std::is_convertible<TFirst, Args>... > {
    TFirst arr[] = { val, args... };

    return *(reinterpret_cast<TTarget*>(arr));

}





template <typename T, std::endian target, std::endian source = std::endian::native >
struct en_swap {
    static T conv(T v) requires (target == source) {
        return byte_swap(v);
    }

    static T conv(T v) requires (target != source) {
        return v;
    }


};


export template <typename T>
T to_big_endian(T v) {
    return en_swap<T, std::endian::native, std::endian::big>::conv(v);

}

export template <typename T>
T to_little_endian(T v) {
    return en_swap < T, std::endian::native, std::endian::little > ::conv(v);
}

