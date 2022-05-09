#include "reflect_factory.h"

#include <iostream>

class Message {
 public:
  virtual std::string ToString() = 0;
  virtual ~Message() {}
};

class Request : public Message {
 public:
  Request(const std::string& msg) : msg_(msg) {}
  std::string ToString() override { return msg_ + "Request"; }

 private:
  std::string msg_;
};

class Response : public Message {
 public:
  Response() : code_(200) {}
  Response(std::uint32_t code) : code_(code) {}

  std::string ToString() override { return std::to_string(code_) + "Response"; }

 private:
  std::uint32_t code_;
};

REGISTER(Request, std::string);
REGISTER(Response, std::uint32_t);

int main() {
  auto request = Factory::New<Message, std::string>("Request", "123");
  auto response = Factory::New<Message, std::uint32_t>("Response", 123);

  std::cout << request->ToString() << std::endl;
  std::cout << response->ToString() << std::endl;
  return 0;
}
