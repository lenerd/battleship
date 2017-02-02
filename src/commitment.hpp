#ifndef COMMITMENT_HPP
#define COMMITMENT_HPP

#include <bitset>
#include <tuple>
#include <vector>
#include "util.hpp"


class Commitment
{
public:
    static Commitment create_open(bytes_t value);
    static Commitment create_hidden(bytes_t commitment);

    bytes_t send_commitment() const;
    void recv_decommitment(bytes_t value, bytes_t padding);
    void recv_decommitment(std::pair<bytes_t, bytes_t> value_padding);

    std::pair<bytes_t, bytes_t> decommit() const;

    void commit();
    bool verify() const;
    void open();

    bytes_t get_value() const;
    bytes_t get_padding() const;
    bytes_t get_commitment() const;
    bool is_open() const;
    bool is_committed() const;
private:
    Commitment();

    bytes_t value_;
    bytes_t commitment_;
    bytes_t padding_;

    bool open_;
    bool committed_;
};

#endif // COMMITMENT_HPP
