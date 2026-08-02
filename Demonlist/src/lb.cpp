#include <Geode/utils/web.hpp>

using namespace std;

web::WebRequest req = web::WebRequest();

req.param("count", 200);
req.param("demon", "");

req.header("Content-Type", "application/json");

string url = "https://gdbrowser.com/api/leaderboard";

auto future = req.get(url);

