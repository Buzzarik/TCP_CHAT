#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <queue>

namespace io = boost::asio;
using namespace std::placeholders;


namespace my_chat
{
    using tcp = io::ip::tcp;
    using error_code = boost::system::error_code;
    using handle_message = std::function<void(std::string)>;
    using handle_error = std::function<void()>;

    template<class TBuffer>
    class session : public std::enable_shared_from_this<session<TBuffer>>{
        public:
        session(tcp::socket &&socket) : socket(std::move(socket)) {}

        void start(handle_message &&on_message, handle_error &&on_error){
            this->on_message = on_message;
            this->on_error = on_error;

            async_read();
        }
        
        void post(const TBuffer &message){
            bool isEmpty = outgoing.empty();
            outgoing.push(message);

            if (isEmpty){
                async_write();
            }
        }

        std::string remote_endpoint() const{
            std::stringstream endpoint;
            endpoint << socket.remote_endpoint();
            return endpoint.str();
        }
        private:

        void async_read(){
            io::async_read_until(
                socket,
                streambuf,
                '\n',
                [&, self = this->shared_from_this()](error_code error, size_t transfered_bytes){
                    if (!error){
                        std::stringstream message;
                        message << socket.remote_endpoint() << ": "
                                << std::istream(&self->streambuf).rdbuf();
                        streambuf.consume(transfered_bytes);

                        on_message(message.str()); //по-любому в виде строки, так как консолька!
                        async_read();
                    }
                    else{
                        socket.close();
                        on_error();
                    }
                }
            );
        }

        void async_write(){
            io::async_write(
                socket,
                io::buffer(outgoing.front()),
                [&, self = this->shared_from_this()](error_code error, size_t transfered_bytes){
                    if (!error){
                        outgoing.pop();

                        if (!outgoing.empty()){
                            async_write();
                        }
                    }
                    else{
                        socket.close();
                        on_error();
                    }
                }
            );
        }
        tcp::socket socket;
        io::streambuf streambuf;
        handle_error on_error;
        handle_message on_message;
        std::queue<TBuffer> outgoing;
    };
}