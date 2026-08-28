#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

#include "lb.hpp"

using namespace geode::prelude;

web::WebFuture fetchLeaderboardData() {
    auto req = web::WebRequest();
    req.param("count", "100");
    req.param("demon", "");
    req.header("Content-Type", "application/json");
    return req.get("https://gdbrowser.com/api/leaderboard");
}