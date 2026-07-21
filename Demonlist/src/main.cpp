#include <Geode/modify/LeaderboardsLayer.hpp>

using namespace geode::prelude;

class $modify(LeaderboardsLayer) {
    bool init(LeaderboardType type, LeaderboardStat stat) {
        if (!LeaderboardsLayer::init(type, stat))
            return false;

        auto rightMenu = this->getChildByID("right-side-menu");

        auto spr = ButtonSprite::create("Demonlist");
        
        auto btn = CCMenuItemSpriteExtra::create(spr, this, nullptr);

        btn->setPosition(rightMenu->getPosition() - CCPoint { 492.5f, 160.f });
        rightMenu->addChild(btn);

        return true;
    }
};