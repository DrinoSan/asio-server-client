#include "con_handler.h"
//importing libraries

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

class Server
{
private:
    tcp::acceptor acceptor_;
    void start_accept()
    {
        // socket
        con_handler::pointer connection = con_handler::create(
                static_cast<io_service &>(acceptor_.get_executor().context()));

        // asynchronous accept operation and wait for a new connection.
        acceptor_.async_accept(connection->socket(),
                               boost::bind(&Server::handle_accept, this, connection,
                                           boost::asio::placeholders::error));
    }
public:
//constructor for accepting connection from client
    Server(boost::asio::io_service& io_service): acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234))
    {
        start_accept();
    }
    void handle_accept(con_handler::pointer connection, const boost::system::error_code& err)
    {
        if (!err) {
            connection->start();
        }
        start_accept();
    }
};


int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_service io_service;
        Server server(io_service);
        io_service.run();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << endl;
    }
    return 0;
}