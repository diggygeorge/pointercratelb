#include <Geode/Geode.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>

#include "lb.hpp"

using namespace geode::prelude;

class $modify(DemonlistLayer, LeaderboardsLayer) {

    struct Fields {
        TaskHolder<web::WebResponse> m_listener;
    };

    void onButton(CCObject* sender) {
        geode::log::info("Button clicked!");
        
        

        m_fields->m_listener.spawn(
            fetchLeaderboardData(),
            [](web::WebResponse res) {
                log::info("{}", res.string().unwrapOr("Uh oh!"));
            }
        );
    }

    void onTop(CCObject* sender) {
        LeaderboardsLayer::onTop(sender);
        
        auto rightMenu = this->getChildByID("right-side-menu");

        // check if button exists
        if (rightMenu->getChildByID("demonlist-button"))
            return;

        auto topSprite = CCSprite::create("leaderboardLogo.png"_spr);

        auto spr = CircleButtonSprite::create(topSprite);

        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(DemonlistLayer::onButton));

        btn->setPosition(rightMenu->getPosition() - CCPoint { 492.5f, 180.f });
        btn->setID("demonlist-button");
        rightMenu->addChild(btn);
    }
    void onCreators(CCObject* sender) {
        LeaderboardsLayer::onCreators(sender);
        // geode::log::info("Creator tab!");

        auto rightMenu = this->getChildByID("right-side-menu");
        if (rightMenu->getChildByID("demonlist-button")) {
            rightMenu->removeChildByID("demonlist-button");
            return;
        }
    }
    void onGlobal(CCObject* sender) {
        LeaderboardsLayer::onGlobal(sender);
        // geode::log::info("Global tab!");

        auto rightMenu = this->getChildByID("right-side-menu");
        if (rightMenu->getChildByID("demonlist-button")) {
            rightMenu->removeChildByID("demonlist-button");
            return;
        }
    }
    void onWeek(CCObject* sender) {
        LeaderboardsLayer::onWeek(sender);
        // geode::log::info("Friends tab!");
        
        auto rightMenu = this->getChildByID("right-side-menu");
        if (rightMenu->getChildByID("demonlist-button")) {
            rightMenu->removeChildByID("demonlist-button");
            return;
        }

    }
};