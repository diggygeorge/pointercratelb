#include <Geode/Geode.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/binding/InfoAlertButton.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <matjson.hpp>
#include <iostream>
#include <Geode/modify/GJScoreCell.hpp>

#include "lb.hpp"

using namespace geode::prelude;

matjson::Value data = matjson::makeObject({});
class GJPointercrateScore : public GJUserScore {
    private:
        double m_listPoints;
    public:
    static GJPointercrateScore* create() {
        auto ret = new GJPointercrateScore();
        
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() {
        if (!GJUserScore::init()) {
            return false;
        }
        return true;
    }

    void setPoints(double listPoints) {
        this->m_listPoints = listPoints;
    }

    double getPoints() {
        return this->m_listPoints;
    }
};
class $modify(GJScoreCell) {
    void loadFromScore(GJUserScore* score) {
        GJScoreCell::loadFromScore(score);
        if (auto p_score = dynamic_cast<GJPointercrateScore*>(score)) {
            double listPoints = p_score->getPoints();
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << listPoints;
            std::string pointsText = ss.str() + " Points";

            auto pointsLabel = cocos2d::CCLabelBMFont::create(pointsText.c_str(), "bigFont.fnt");
            pointsLabel->setScale(0.45f);
            pointsLabel->setID("pointercrate-points-label");

            if (auto mainLayer = static_cast<cocos2d::CCLayer*>(this->getChildren()->objectAtIndex(1))) {
                
                if (auto starsIcon = mainLayer->getChildByID("stars-icon")) starsIcon->setVisible(false);
                if (auto starsLabel = mainLayer->getChildByID("stars-label")) starsLabel->setVisible(false);
                if (auto moonsIcon = mainLayer->getChildByID("moons-icon")) moonsIcon->setVisible(false);
                if (auto moonsLabel = mainLayer->getChildByID("moons-label")) moonsLabel->setVisible(false);

                if (auto referenceNode = mainLayer->getChildByID("stars-label")) {
                    pointsLabel->setAnchorPoint({0.f, 0.5f});
                    pointsLabel->setPosition(referenceNode->getPosition());
                } else {
                    pointsLabel->setAnchorPoint({0.f, 0.5f});
                    pointsLabel->setPosition({ 45.f, 25.f });
                }

                pointsLabel->setColor({255, 57, 80});

                mainLayer->addChild(pointsLabel);
            }
        }
    }
};
class $modify(DemonlistLayer, LeaderboardsLayer) {

    struct Fields {
        TaskHolder<web::WebResponse> m_listener;
    };
    private:
    void deleteLbLayer() {
        auto listLayer = this->getChildByID("GJListLayer");
        if (listLayer == nullptr) {
            listLayer = this->getChildByID("list-layer");
        }

        if (listLayer->getChildByID("list-view")) {
            listLayer->removeChildByID("list-view");
        }
    }
    void insertDemonlistLeaderboards() {
        auto entries = CCArray::create();
        if (data.isArray()) {
            for (int i = 0; i < data.size(); i++) {
                auto item = data[i];
                auto score = GJPointercrateScore::create();
                // demonlist points: score->;
                    
                score->m_userName = item["username"].asString().unwrapOr("Unknown");
                score->m_userID = std::stoi(item["playerID"].asString().unwrapOr("0"));
                score->m_accountID = std::stoi(item["accountID"].asString().unwrapOr("0"));
                score->setPoints(item["demonList"]["points"].asDouble().unwrapOr(0.0));
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
                GameLevelManager::sharedState()->storeUserName(score->m_userID, score->m_accountID, score->m_userName);
                entries->addObject(score);
            }
        }

        float listWidth = 358.f;
        float listHeight = 220.f;
        deleteLbLayer();
        auto listView = CustomListView::create(entries, BoomListType::Score, listHeight, listWidth);
        auto listLayer = this->getChildByID("GJListLayer");
        if (listLayer == nullptr) {
            listLayer = this->getChildByID("list-layer");
        }

        listView->setID("list-view");
        listLayer->addChild(listView);
        listView->setPosition(CCPoint { 0.f, 0.f });
    }
    void onButton(CCObject* sender) {
        geode::log::info("Button clicked!");
        if (data.size() != 0) {
            // Cache data
            insertDemonlistLeaderboards();
        } else {
            // fetch leaderboard data
            m_fields->m_listener.spawn(
                fetchLeaderboardData(),
                [this](web::WebResponse res) {
                    data = res.json().unwrapOr(matjson::makeObject({}));
                    insertDemonlistLeaderboards();
                }
            );
        }
    }
    public:
    bool init(LeaderboardType type, LeaderboardStat stat) {
        if (!LeaderboardsLayer::init(type, stat)) {
            return false;
        }

        auto top = this->getChildByID("top-100-menu");
        if (top->getZOrder() == 2) {
            auto rightMenu = this->getChildByID("right-side-menu");
            // create demonlist button
            auto topSprite = CCSprite::create("leaderboardLogo.png"_spr);

            auto spr = CircleButtonSprite::create(topSprite);

            auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(DemonlistLayer::onButton));

            btn->setID("demonlist-button");
            rightMenu->addChild(btn);
            btn->setPosition(rightMenu->getPosition() - CCPoint { 492.5f, 180.f });
            auto infoButton = InfoAlertButton::create("Demonlist Leaderboards", "Calculated by Pointercrate.  Details can be found on https://pointercrate.com/.", 1.0f);
            infoButton->setID("demonlist-info");
            rightMenu->addChild(infoButton);
            infoButton->setPosition(rightMenu->getPosition() - CCPoint { 452.5f, 180.f });
        }
        return true;
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
        btn->setID("demonlist-button");
        rightMenu->addChild(btn);
        btn->setPosition(rightMenu->getPosition() - CCPoint { 492.5f, 180.f });
        auto infoButton = InfoAlertButton::create("Demonlist Leaderboards", "The Demonlist leaderboards is sorted by Demonlist Points, managed by Pointercrate.  Details can be found on https://pointercrate.com/", 1.0f);
        infoButton->setID("demonlist-info");
        rightMenu->addChild(infoButton);
        infoButton->setPosition(rightMenu->getPosition() - CCPoint { 448.5f, 180.f });
    }
    void onCreators(CCObject* sender) {
        LeaderboardsLayer::onCreators(sender);
        // geode::log::info("Creator tab!");

        auto rightMenu = this->getChildByID("right-side-menu");
        if (rightMenu->getChildByID("demonlist-button")) {
            rightMenu->removeChildByID("demonlist-button");
            rightMenu->removeChildByID("demonlist-info");
        }
        return;
    }

    void onBack(CCObject* sender) {
        LeaderboardsLayer::onBack(sender);
        // geode::log::info("Creator tab!");

        auto rightMenu = this->getChildByID("right-side-menu");
        if (rightMenu->getChildByID("demonlist-button")) {
            rightMenu->removeChildByID("demonlist-button");
            rightMenu->removeChildByID("demonlist-info");
        }
    }
    void onGlobal(CCObject* sender) {
        LeaderboardsLayer::onGlobal(sender);
        // geode::log::info("Global tab!");

        auto rightMenu = this->getChildByID("right-side-menu");
        if (rightMenu->getChildByID("demonlist-button")) {
            rightMenu->removeChildByID("demonlist-button");
            rightMenu->removeChildByID("demonlist-info");
        }
    }
    void onWeek(CCObject* sender) {
        LeaderboardsLayer::onWeek(sender);
        // geode::log::info("Friends tab!");
        
        auto rightMenu = this->getChildByID("right-side-menu");
        if (rightMenu->getChildByID("demonlist-button")) {
            rightMenu->removeChildByID("demonlist-button");
            rightMenu->removeChildByID("demonlist-info");
        }
    }
};