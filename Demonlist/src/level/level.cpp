#include <Geode/Geode.hpp>
#include <Geode/binding/InfoAlertButton.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <matjson.hpp>
#include <iostream>
#include <Geode/modify/DemonFilterSelectLayer.hpp>

// #include "list.hpp"

using namespace geode::prelude;

class $modify(MyDemonFilterSelectLayer, DemonFilterSelectLayer) {
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

        auto spr = ButtonSprite::create(topSprite);
        auto btn = CCMenuItemSpriteExtra::create(topSprite, this, nullptr);
        btn->setID("list-filter-button");
        demonFilters->addChild(btn);
        btn->setPosition({180.f, 3.f});

        return true;
    }
};