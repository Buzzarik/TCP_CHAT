#pragma once
#include "my_chat/server_type.hpp"

using message_type = std::string;
using session_type = my_chat::session<message_type>;
using server_type = my_chat::server<session_type>;

class server {
  public:
    server(io::io_context& io_context, std::uint16_t port);
    void start();
  private:
    server_type srv;
};