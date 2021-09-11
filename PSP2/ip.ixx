#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <WinSock2.h>
export module ip;
import endian;
 


namespace {

    std::vector<int> split_into_numbers(const std::string& str, char delim = ' ') {
        std::vector<int> ret;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delim)) {

            ret.push_back(std::stoi(token));

        };

        return ret;
    }
 
    bool nums_check(std::vector<int>& nums) {
        return std::all_of(nums.begin(), nums.end(), [](uint32_t num) {return num <= 255 and num >= 0; });
    }

    bool nums_check(uint8_t first, uint8_t second, uint8_t third, uint8_t forth) {

        return first <= 255 and second <= 255 and third <= 255 and forth <= 255;

    }


    bool dots_check(std::string& ip_address) {
        return std::count(ip_address.begin(), ip_address.end(), '.') == 3;
    }
}
export class basic_address {
public:


protected:
    union {
        uint32_t number;
        struct {
            uint8_t first;
            uint8_t second;
            uint8_t third;
            uint8_t forth;

        } nums;
    } address;



public:

    uint32_t& number = address.number;

    uint8_t& first = address.nums.first;
    uint8_t& second = address.nums.second;
    uint8_t& third = address.nums.third;
    uint8_t& forth = address.nums.forth;


    basic_address() : address{} {}

    basic_address(uint32_t number) : address{ .number = number } {}




    friend std::ostream& operator << (std::ostream& os, const basic_address& a) {
        os << static_cast<unsigned>(a.first) << '.' << static_cast<unsigned>(a.second) << '.' << static_cast<unsigned>(a.third) << '.' << static_cast<unsigned>(a.forth);
        return os;
    }


    virtual operator std::string() const {
        return std::to_string(first) + "." + std::to_string(second) + "." + std::to_string(third) + "." + std::to_string(forth);
    }

    basic_address as_big_endian() {
        return { to_big_endian(number) };
    }

    basic_address as_little_endian() {
        return { to_little_endian(number) };
    }

    operator uint32_t() const {
        return number;
    }

    basic_address operator ~() {
        return ~number;
    }

    basic_address operator |(basic_address another) {
        return number | another.number;
    }

    basic_address operator &(basic_address another) {
        return number & another.number;
    }


    basic_address& operator=(const basic_address& another) {
        number = another.number;
        return *this;
    }


};



export class ip_address : public basic_address {
public:


    using basic_address::basic_address;



    static ip_address any() {
        return { byte_swap(INADDR_ANY)};
    }

    static ip_address loopback() {
        return { byte_swap<uint32_t>(INADDR_LOOPBACK) };
    }
    static ip_address broadcast() {
        return { byte_swap(INADDR_BROADCAST) };
    }

    
    ip_address(uint8_t first, uint8_t second, uint8_t third, uint8_t forth) {
        number = from_bytes<uint32_t>(static_cast<uint8_t>(first), static_cast<uint8_t>(second), static_cast<uint8_t>(third), static_cast<uint8_t>(forth));
    }


    ip_address(std::string ip_address) {


        if (not dots_check(ip_address))
            throw std::invalid_argument("dots check failed");

        auto nums = split_into_numbers(ip_address, '.');

        if (not nums_check(nums))
            throw std::invalid_argument("nums check failed");

        number = from_bytes<uint32_t>(static_cast<uint8_t>(nums[0]), static_cast<uint8_t>(nums[1]), static_cast<uint8_t>(nums[2]), static_cast<uint8_t>(nums[3]));


    }

    ip_address& operator=(const basic_address& b) {
        number = b.number;
        return *this;
    }


    operator basic_address() const { return number; };

    friend std::istream& operator>>(std::istream& is, ip_address& m) {
        std::string s;
        is >> s;
        m = ip_address{ s };
        return is;
    }

};



class subnet_mask : public basic_address {
private:
    static bool is_valid_subnet_mask(uint32_t subnet_mask) {
        uint32_t reversed = byte_swap(subnet_mask);
        return (not ((~reversed + 1) & ~reversed) and reversed and ~reversed);
    }

public:



    subnet_mask() {}

    subnet_mask(uint32_t number) {


        if (not is_valid_subnet_mask(number)) {
            throw std::invalid_argument("invalid subnet subnet_mask");
        }

        this->number = number;
      
    }


    subnet_mask(uint8_t first, uint8_t second, uint8_t third, uint8_t forth) {

        uint32_t num = from_bytes<uint32_t>(first, second, third, forth);

        if (not is_valid_subnet_mask(num)) {
            throw std::invalid_argument("invalid subnet mask");
        }

        number = num;
    }


    subnet_mask(std::string subnet_mask) {

        if (not dots_check(subnet_mask))
            throw std::invalid_argument("dots check failed");

        auto nums = split_into_numbers(subnet_mask, '.');

        if (not nums_check(nums))
            throw std::invalid_argument("nums check failed");


        uint32_t num = from_bytes<uint32_t>(static_cast<uint8_t>(nums[0]), static_cast<uint8_t>(nums[1]), static_cast<uint8_t>(nums[2]), static_cast<uint8_t>(nums[3]));

        if (not is_valid_subnet_mask(num)) {
            throw std::invalid_argument("invalid subnet subnet_mask");
        }

        number = num;

    }

    operator basic_address() const { return number; };

    subnet_mask operator=(const basic_address& b) {
        return subnet_mask{ b.number };
    }


    friend std::istream& operator>>(std::istream& is, subnet_mask& m) {
        std::string s;
        is >> s;
        m = subnet_mask{ s };
        return is;
    }


};


class cidr : public ip_address
{
public:
    subnet_mask mask;
    cidr() {};
    cidr(ip_address ip)
    {
        this->number = ip.number;
    }

    cidr(ip_address ip, subnet_mask mask) : mask{ mask }
    {
        this->number = ip.number;
    }

    friend std::ostream& operator << (std::ostream& os, const cidr& c) {
        os << static_cast<unsigned>(c.first) << '.' << static_cast<unsigned>(c.second) << '.' << static_cast<unsigned>(c.third) << '.' << static_cast<unsigned>(c.forth) << '/' << std::popcount(c.mask.number);
        return os;
    }



    operator std::string() const override {
        return std::to_string(first) + "." + std::to_string(second) + "." + std::to_string(third) + "." + std::to_string(forth) + "/" + std::to_string(std::popcount(mask.number));
    }
};