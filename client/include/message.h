#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <cstdio>
#include <cstdlib>
#include <string>

class Message {
 public:
    enum { kHeaderLength = 4 };
    enum { kMaxBodyLength = 512 };

    Message()
        : body_length_(0) {
    }

    explicit Message(const std::string& message) {
        body_length(message.size());
        std::memcpy(body(), message.c_str(), body_length());
        encode_header();
    }

    const char* data() const {
        return data_;
    }

    char* data() {
        return data_;
    }

    std::size_t length() const {
        return kHeaderLength + body_length_;
    }

    const char* body() const {
        return data_ + kHeaderLength;
    }

    char* body() {
        return data_ + kHeaderLength;
    }

    std::size_t body_length() const {
        return body_length_;
    }

    void body_length(std::size_t new_length) {
        body_length_ = new_length;
        if (body_length_ > kMaxBodyLength)
            body_length_ = kMaxBodyLength;
    }

    bool decode_header() {
        char header[kHeaderLength + 1] = "";
        std::strncat(header, data_, kHeaderLength);
        body_length_ = std::atoi(header);
        if (body_length_ > kMaxBodyLength) {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    void encode_header() {
        char header[kHeaderLength + 1] = "";
        std::snprintf(header, sizeof(header), "%4d", static_cast<int>(body_length_));
        std::memcpy(data_, header, kHeaderLength);
    }

 private:
    char data_[kHeaderLength + kMaxBodyLength];
    std::size_t body_length_;
};

#endif  // MESSAGE_H_
