#include "include/server.hpp"

int main() {
    io::io_context io_context;
    server srv(io_context, 15500);
    srv.start();
    io_context.run();
    return 0;
}