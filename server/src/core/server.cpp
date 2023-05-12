#include "server.h"

namespace http_server {

void ReportError(boost::system::error_code& error, std::string_view what) {
    std::cout << error.what() << '\t' << what;
}

void SessionBase::Run() {
        net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(&SessionBase::Read, GetSharedThis()));
}

void SessionBase::Read() {
        using namespace std::literals;
        request_ = {};
        stream_.expires_after(600s);
        http::async_read(stream_, buffer_, request_, 
                         beast::bind_front_handler(&SessionBase::OnRead, GetSharedThis()));
}

void SessionBase::OnRead(beast::error_code error, std::size_t bytes_read) {
        if(error == http::error::end_of_stream) {
            return Close();
        }
        if(error) {
            return ReportError(error, "read");
        }
        HandleRequest(std::move(request_));
}

void SessionBase::OnWrite(bool close, beast::error_code error, 
                          std::size_t bytes_written) {
        if(error) {
            return ReportError(error, "write");
        }

        if(close) {
            return Close();
        }
        Read(); 
}

void SessionBase::Close() {
        beast::error_code error;
        stream_.socket().shutdown(tcp::socket::shutdown_send, error);
}

tcp::endpoint SessionBase::GetEndpoint() {
    return stream_.socket().local_endpoint();
}
}  // namespace http_server