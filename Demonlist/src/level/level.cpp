#include <Geode/Geode.hpp>
#include <Geode/binding/InfoAlertButton.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <matjson.hpp>
#include <iostream>
#include <Geode/modify/DemonFilterSelectLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/binding/GJSearchObject.hpp>

#include "list.hpp" 
using namespace geode::prelude;

class $modify(DemonBrowserLayer, LevelBrowserLayer) {
    public:
    static LevelBrowserLayer* create(GJSearchObject* object) {
        return LevelBrowserLayer::create(object);
    }
};

class $modify(MyDemonFilterSelectLayer, DemonFilterSelectLayer) {
    struct Fields {
        TaskHolder<web::WebResponse> m_listener;
    };

    private:
    void onButton(CCObject* sender) {
        geode::log::info("Button clicked!");
        // TO DO: call IDs from an API, string them together, and store it into chosenIDs
        // OR you could do the API stuff but then create a new type of levelbrowserlayer if you wish to display the rank within each of the sections
        // PLAN THIS OUT
        
        std::string chosenIDs = "127323087,119544028,126242564,119550490,86407629,142896409,133175713,109780665,117692518,110816181,135661111,116174063,134942736,4125776,62912799,138704906,114283297,138781722,127997391,112313819,89496627,73667628,110815379,107805281,49896559,120012581,91351939,86018142,110534288,93917076,27690100,113599729,87665224,48991380,130327668,93091893,141052363,114281093,134541844,76962930,113045735,136396693,95851008,131599104,109439644,49072489,75206202,113959291,115077305,120289520,113322063,143718114,85133223,136981369,89414220,90477539,113443235,120255728,81139702,94359172,126571887,96314787,83244159,89187968,139508889,90390075,104232191,86084399,114990369,94969889,62556400,113256247,82544060,131497860,113220284,99703915,81011195,119689515,125317359,114999625,93339534,95719795,95998005,110500920,95049815,105593215,125788781,128093374,82249742,122941596,136135870,96083028,133489881,93340783,103925676,72315402,73214186,93792764,129235528,69685815,59075347,108708033,110991117,95031870,93917362,137757402,109947627,94858072,107741051,130464801,60978746,71025973,107238250,95176417,90057148,76543324,97086864,88203501,104672501,75286957,137334994,105748155,88442157,87130877,93732702,110705712,71885708,113364415,87071894,112242564,79771070,127023313,100990392,115380769,80714349,71434979,120060187,58673581,65588448,128385946,114530859,78435955,137586316,99495619,122389640,88611404,122196131,125617849,112231282,105625871";
        auto obj = GJSearchObject::create(static_cast<SearchType>(10), chosenIDs);
        // auto searchObj = DemonBrowserLayer::create(obj);
        auto scene = LevelBrowserLayer::scene(obj);
        CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
        // m_fields->m_listener.spawn(
        //     fetchListData("0"),
        //     [](web::WebResponse res) {
        //         geode::log::info("{}", res.string().unwrapOr("Uh oh!"));
        //     }
        // );
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