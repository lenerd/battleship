#include <cassert>
#include <fstream>
#include <cryptopp/sha.h>
#include "commitment.hpp"
#include "util.hpp"

Commitment::Commitment() {}

Commitment Commitment::create_open(bytes_t value)
{
    Commitment c = Commitment();
    c.value_ = value;
    c.committed_ = false;
    c.open_ = true;
    return c;
}
Commitment Commitment::create_hidden(bytes_t commitment)
{
    Commitment c = Commitment();
    c.commitment_ = commitment;
    c.committed_ = true;
    c.open_ = false;
    return c;
}

bytes_t Commitment::send_commitment() const
{
    assert(committed_);
    return commitment_;
}

void Commitment::recv_decommitment(bytes_t value, bytes_t padding)
{
    assert(!open_);
    value_ = value;
    padding_ = padding;
    open_ = true;
}

void Commitment::recv_decommitment(std::pair<bytes_t, bytes_t> value_padding)
{
    recv_decommitment(value_padding.first, value_padding.second);
}

void Commitment::commit()
{
    assert(open_);
    padding_ = random_bytes(16);

    CryptoPP::SHA1 sha1;
    sha1.Update(value_.data(), value_.size());
    sha1.Update(padding_.data(), padding_.size());
    commitment_.resize(sha1.DigestSize());
    sha1.Final(commitment_.data());
    committed_ = true;
}

bool Commitment::verify() const
{
    assert(open_);
    assert(committed_);
    CryptoPP::SHA1 sha1;
    sha1.Update(value_.data(), value_.size());
    sha1.Update(padding_.data(), padding_.size());
    return sha1.Verify(commitment_.data());
}

std::pair<bytes_t, bytes_t> Commitment::decommit() const
{
    assert(committed_);
    assert(open_);
    return {value_, padding_};
}


bytes_t Commitment::get_value() const
{
    assert(open_);
    return value_;
}

bytes_t Commitment::get_padding() const
{
    assert(committed_);
    return padding_;
}

bytes_t Commitment::get_commitment() const
{
    assert(committed_);
    return commitment_;
}

bool Commitment::is_open() const
{
    return open_;
}

bool Commitment::is_committed() const
{
    return committed_;
}
