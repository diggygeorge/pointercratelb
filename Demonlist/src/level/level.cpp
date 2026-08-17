#include <Geode/Geode.hpp>
#include <Geode/binding/InfoAlertButton.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <matjson.hpp>
#include <iostream>
#include <string>
#include <Geode/modify/DemonFilterSelectLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/binding/GJSearchObject.hpp>

#include "list.hpp" 
using namespace geode::prelude;

class $modify(DemonBrowserLayer, LevelBrowserLayer) {
    struct Fields {
        int m_customPage;
        TaskHolder<web::WebResponse> m_listener;
    };
    private:
    void customSceneWithIndex(int page) {
        std::string pg = std::to_string(page);
        m_fields->m_listener.spawn(
             fetchListData(pg),
             [](web::WebResponse res) {
                matjson::Value data = res.json().unwrapOr(matjson::makeObject({}));
                // take all the ids, join them by id
                std::string values = "";
                for (int i = 0; i < data.size(); i++) {
                    auto item = data[i];
                    auto id = item["id"].asString().unwrapOr("0");
                    values = values + id + ",";
                }
                if (!values.empty()) {
                    values.resize(values.size() - 1);
                }
                geode::log::info("{}", values);
            }
         );
    }

    public:

    // creates demonbrowserlayer object 
    static LevelBrowserLayer* customCreate() {
        std::string customIDs = "127323087,119544028,126242564,119550490,86407629,142896409,133175713,109780665,117692518,110816181";
        GJSearchObject* object = GJSearchObject::create(SearchType::Type19, customIDs);
        return LevelBrowserLayer::create(object);
    }

    bool init(GJSearchObject* object) {
        if (!LevelBrowserLayer::init(object)) return false;

        auto winSize = CCDirector::get()->getWinSize();

        auto arrowMenu = CCMenu::create();
        arrowMenu->setID("arrow-menu");
        arrowMenu->setPosition(0, 0);

        if (m_fields->m_customPage < 14) {
            auto leftArrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            leftArrowSprite->setFlipX(true);
            auto nextBtn = CCMenuItemSpriteExtra::create(
                leftArrowSprite,
                this,
                menu_selector(DemonBrowserLayer::customNextPage)
            );
            arrowMenu->addChild(nextBtn);
            nextBtn->setPosition(winSize.width - 25.f, winSize.height / 2.f);
            nextBtn->setID("next-arrow");
        }

        if (m_fields->m_customPage > 0) {
            auto prevBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
                this,
                menu_selector(DemonBrowserLayer::customPrevPage)
            );
            arrowMenu->addChild(prevBtn);
            prevBtn->setPosition(25.f, winSize.height / 2.f);
            prevBtn->setID("prev-arrow");
        }

        this->addChild(arrowMenu, 10);

        return true;
    }

    void customNextPage(cocos2d::CCObject* sender) {
        m_fields->m_customPage++;
        this->customSceneWithIndex(m_fields->m_customPage);
        //CCDirector::get()->replaceScene(scene);
        /*
        auto newLayer = DemonBrowserLayer::create(searchObj);
        newLayer->m_customPage = this->m_customPage;
        auto scene = CCScene::create();
        scene->addChild(newLayer);
        CCDirector::get()->replaceScene(scene);

        OR
        auto scene = customSceneWithIndex(m_fields->m_customPage)
        CCDirector::get()->replaceScene(scene);
        */
    }

    void customPrevPage(cocos2d::CCObject* sender) {
        LevelBrowserLayer::onPrevPage(sender);
        m_fields->m_customPage--;
        this->customSceneWithIndex(m_fields->m_customPage);
        //CCDirector::get()->replaceScene(scene);
    }
};

class $modify(MyDemonFilterSelectLayer, DemonFilterSelectLayer) {

    private:
    void onButton(CCObject* sender) {
        geode::log::info("Button clicked!");
        // TO DO: call IDs from an API, string them together, and store it into chosenIDs
        // OR you could do the API stuff but then create a new type of levelbrowserlayer if you wish to display the rank within each of the sections
        // PLAN THIS OUT
        
        auto newLayer = DemonBrowserLayer::customCreate();
        auto scene = CCScene::create();
        scene->addChild(newLayer);
        CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
    }

    public:
    bool init() {
        if (!DemonFilterSelectLayer::init()) {
            return false;
        }
        auto mainLayer = this->getChildByID("main-layer");
        auto scaleSprite = mainLayer->getChildByID("background");
        scaleSprite->setContentSize(cocos2d::CCSize(450.f, 180.f));
        auto demonFilters = mainLayer->getChildByID("demon-filters");

        auto allDemon = demonFilters->getChildByID("all-demon-filter-button");
        allDemon->setPosition({-180.f, 3.f});
        auto easyDemon = demonFilters->getChildByID("easy-demon-filter-button");
        easyDemon->setPosition({-120.f, 3.f});
        auto mediumDemon = demonFilters->getChildByID("medium-demon-filter-button");
        mediumDemon->setPosition({-60.f, 3.f});
        auto hardDemon = demonFilters->getChildByID("hard-demon-filter-button");
        hardDemon->setPosition({0.f, 3.f});
        auto insaneDemon = demonFilters->getChildByID("insane-demon-filter-button");
        insaneDemon->setPosition({60.f, 3.f});
        auto extremeDemon = demonFilters->getChildByID("extreme-demon-filter-button");
        extremeDemon->setPosition({120.f, 3.f});

        auto topSprite = CCSprite::create("list.png"_spr);
        auto btn = CCMenuItemSpriteExtra::create(topSprite, this, menu_selector(MyDemonFilterSelectLayer::onButton));
        btn->setID("list-filter-button");
        demonFilters->addChild(btn);
        btn->setPosition({180.f, 3.f});

        return true;
    }
};