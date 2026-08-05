#include <Geode/Geode.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <matjson.hpp>
#include <iostream>

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
                // convert to json using .json()
                matjson::Value data = res.json().unwrapOr(matjson::makeObject({}));
                int size = data.size();
                if (data.isArray()) {
                    for (int i = 0; i < size; i++) {
                        geode::log::info("Player: {}, Rank: {}", data[i]["username"].asString().unwrapOr("Unknown"), data[i]["rank"].asInt().unwrapOr(0));
                    }
                }

                // open a new menu
                // for each entry, display a user box like the ones in the leaderboards section (click on a name, 
                // display it bigger, figure out how to replicate the same menu that robtop does) 
                // include a back button to exit out
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