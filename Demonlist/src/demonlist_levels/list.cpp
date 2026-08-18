#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <iostream>
#include "list.hpp"

using namespace geode::prelude;

web::WebFuture fetchListData(std::string page) {
    auto req = web::WebRequest();
    req.param("count", "10");
    req.param("type", "demonlist");
    req.param("page", page);
    req.header("Content-Type", "application/json");
    return req.get("https://gdbrowser.com/api/search/*");
}