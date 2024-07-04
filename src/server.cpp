#include "../include/server.hpp"

server::server(io::io_context& io_context, std::uint16_t port) :
    srv(io_context, port) {
    srv.on_join([&](session_type& client) {
        client.post("Добро пожаловать в TCP-chat\n\r");
        srv.broadcast("К нам присоединился новый пользователь: " + client.remote_endpoint() + "\n\r");
    });

    srv.on_leave([&] {
        srv.broadcast("Нас покинул пользователь\n\r");
    });

    srv.on_message([&](message_type const& message) {
        srv.broadcast(message); 
    });
}

void server::start() {
    srv.start();
}