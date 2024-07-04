#pragma once
#include "session_type.hpp"
#include <optional>
#include <unordered_set>
#include <memory>

namespace my_chat{
    template<class T>
    using handle_accept = std::function<void(T&)>;

    template<class C>
    class server{
        public:
            server(io::io_context& io_context, uint16_t port) :
                io_context(io_context),
                acceptor(io_context, tcp::endpoint(tcp::v4(), port)){}

            void on_join(handle_accept<C> &&callback_accept){
                this->callback_accept = callback_accept;
            }

            void start(){
                async_accept();
            }

            void on_leave(handle_error &&callback_error){
                this->callback_error = callback_error;
            }

            void on_message(handle_message &&callback_message){
                this->callback_message = callback_message;
            }

            template <class T>
            void broadcast(const T& message){
                for (auto& client : clients){
                    client->post(message);
                }
            }
        private:
            void async_accept(){
                socket.emplace(io_context);

                acceptor.async_accept(*socket, [&](error_code error){
                    auto client = std::make_shared<C>(std::move(*socket));
                    callback_accept(*client);
                    clients.insert(client);
                    client->start(std::bind(server::callback_message, _1),
                        [&, weak = std::weak_ptr(client)](){ 
                            if (auto shared = weak.lock();
                                shared && clients.erase(shared)){
                                    callback_error();
                            }
                        }
                    );
                    async_accept();
                });
            }

            io::io_context& io_context;
            handle_error callback_error;
            handle_message callback_message;
            handle_accept<C> callback_accept;
            tcp::acceptor acceptor;
            std::optional<tcp::socket> socket;
            std::unordered_set<std::shared_ptr<C>> clients;
    };
} // namespace chat