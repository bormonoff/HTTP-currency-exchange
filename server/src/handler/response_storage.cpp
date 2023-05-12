#include "handler/response_storage.h"

namespace handler {

Response InvalidBet(const Request& req) {
    Response response{http::status::bad_request, req.version()};
    response.set(http::field::content_type, CONTENT_TYPE_JSON);
    response.set(http::field::cache_control, NO_CACHE);

    json::object json_res;
    json_res[CODE] = INVALID_BET_CODE;
    json_res[MESSAGE] = BET_HAS_ALREADY_CREATED;
    response.body() = json::serialize(json_res);
    response.content_length(response.body().size());
    return response;
}
}  // namespace handler
