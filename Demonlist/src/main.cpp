#include <Geode/Geode.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <matjson.hpp>
#include <iostream>

#include "lb.hpp"

using namespace geode::prelude;

bool showLayer = false;
matjson::Value data = matjson::makeObject({});
class $modify(DemonlistLayer, LeaderboardsLayer) {

    struct Fields {
        TaskHolder<web::WebResponse> m_listener;
    };
    private:
    void insertDemonlistLeaderboards() {
        auto entries = cocos2d::CCArray::create();
        if (data.isArray()) {
            for (int i = 0; i < data.size(); i++) {
                auto item = data[i];
                auto score = GJUserScore::create();
                    
                score->m_userName = item["username"].asString().unwrapOr("Unknown");
                score->m_userID = std::stoi(item["playerID"].asString().unwrapOr("0"));
                score->m_accountID = std::stoi(item["accountID"].asString().unwrapOr("0"));

                score->m_stars = item["stars"].asInt().unwrapOr(0);
                score->m_moons = item["moons"].asInt().unwrapOr(0);
                score->m_diamonds = item["diamonds"].asInt().unwrapOr(0);
                score->m_demons = item["demons"].asInt().unwrapOr(0);
                score->m_creatorPoints = item["cp"].asInt().unwrapOr(0);
                score->m_secretCoins = item["coins"].asInt().unwrapOr(0);
                score->m_userCoins = item["userCoins"].asInt().unwrapOr(0);

                score->m_playerRank = item["rank"].asInt().unwrapOr(0);
                score->m_globalRank = item["rank"].asInt().unwrapOr(0); 

                score->m_iconID = item["icon"]["icon"].asInt().unwrapOr(1);
                score->m_color1 = item["icon"]["col1"].asInt().unwrapOr(0);
                score->m_color2 = item["icon"]["col2"].asInt().unwrapOr(0);
                score->m_color3 = item["icon"]["colG"].asInt().unwrapOr(0); 
                score->m_glowEnabled = item["icon"]["glow"].asBool().unwrapOr(false);

                std::string form = item["icon"]["form"].asString().unwrapOr("cube");
                if (form == "ship") score->m_iconType = IconType::Ship;
                else if (form == "ball") score->m_iconType = IconType::Ball;
                else if (form == "ufo") score->m_iconType = IconType::Ufo;
                else if (form == "wave") score->m_iconType = IconType::Wave;
                else if (form == "robot") score->m_iconType = IconType::Robot;
                else if (form == "spider") score->m_iconType = IconType::Spider;
                else if (form == "swing") score->m_iconType = IconType::Swing;
                else if (form == "jetpack") score->m_iconType = IconType::Jetpack;
                else score->m_iconType = IconType::Cube;
                
                entries->addObject(score);
            }
        }

        float listWidth = 358.f;
        float listHeight = 220.f;

        auto listView = CustomListView::create(entries, BoomListType::Score, listHeight, listWidth);
        listView->setPosition(CCPoint { 0.f, 0.f });
        auto listLayer = this->getChildByID("GJListLayer");
        listLayer->addChild(listView);
    }
    public:
    void onButton(CCObject* sender) {
        geode::log::info("Button clicked!");
        showLayer = !(showLayer && showLayer);
        if (data.size() != 0) {
            insertDemonlistLeaderboards();
            geode::log::info("Fetched CACHED data!");
        } else {
            m_fields->m_listener.spawn(
                fetchLeaderboardData(),
                [this](web::WebResponse res) {
                    data = res.json().unwrapOr(matjson::makeObject({}));
                    insertDemonlistLeaderboards();
                    geode::log::info("Fetched NEW data!");
                    /*
                    - prevent duplicate button presses
                    - make the menu behind disappear
                    - change z-layer
                    - fix bug with user levels not appearing
                    - attach demonlist points
                    */
                }
            );
        }
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