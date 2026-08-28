#include <Geode/Geode.hpp>
#include <Geode/binding/InfoAlertButton.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <matjson.hpp>
#include <iostream>
#include <string>
#include <functional>
#include <Geode/modify/DemonFilterSelectLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/utils/SeedValue.hpp>

#include "list.hpp" 
using namespace geode::prelude;

static std::unordered_map<std::string, int> s_demonRanks;

class DemonListAPI {
    
    public:
    static void fetchListObject(int pageIndex, std::function<void(GJSearchObject*)> onComplete) {
        std::string pg = std::to_string(pageIndex);
        
        auto listener = new TaskHolder<web::WebResponse>();
        
        listener->spawn(
            fetchListData(pg),
            [pageIndex, onComplete, listener](web::WebResponse res) {
                matjson::Value data = res.json().unwrapOr(matjson::makeObject({}));
                std::string values = "";
                
                for (int i = 0; i < data.size(); i++) {
                    auto item = data[i];
                    auto id = item["id"].asString().unwrapOr("0");
                    s_demonRanks[id] = (pageIndex * 10) + 1 + i;
                    values = values + id + ",";
                }
                
                if (!values.empty()) values.resize(values.size() - 1);
                
                auto obj = GJSearchObject::create(SearchType::Type19, values);
                
                if (onComplete) {
                    onComplete(obj);
                }

                delete listener;
            }
        );
    }
};

class DemonBrowserLayer : public LevelBrowserLayer {
    private:
        int m_customPage;

    void loadPageByIndex(int page) {
        DemonListAPI::fetchListObject(page, [this](GJSearchObject* obj) {
            this->setSearchObject(obj);
            this->loadPage(obj);
        });
    }


    public:

    void setupPageInfo(gd::string info, const char* key) {
        if (auto label = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("level-count-label"))) {
            
            int total = 150;
            int start = (this->m_customPage * 10) + 1;
            int end = start + 9;

            if (end > total) {
                end = total;
            }

            std::string newText = std::to_string(start) + " to " + std::to_string(end) + " of " + std::to_string(total);
            label->setString(newText.c_str());
        }
    }

    // creates demonbrowserlayer object 
    static DemonBrowserLayer* create() {
        auto ret = new DemonBrowserLayer();
        
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() {
        auto placeholder = GJSearchObject::create(SearchType::Type19, "0");

        if (!LevelBrowserLayer::init(placeholder)) return false;

        this->loadPageByIndex(0);

        auto winSize = CCDirector::get()->getWinSize();

        auto arrowMenu = CCMenu::create();
        arrowMenu->setID("arrow-menu");
        arrowMenu->setPosition(0, 0);

        auto leftArrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        leftArrowSprite->setFlipX(true);
        auto nextBtn = CCMenuItemSpriteExtra::create(
            leftArrowSprite,
            this,
            menu_selector(DemonBrowserLayer::onNextPage)
        );
        arrowMenu->addChild(nextBtn);
        nextBtn->setPosition(winSize.width - 25.f, winSize.height / 2.f);
        nextBtn->setID("next-arrow");

        this->addChild(arrowMenu, 10);

        return true;
    }

    void onNextPage(cocos2d::CCObject* sender) {
        this->m_customPage++;
        this->loadPageByIndex(this->m_customPage);
        auto arrowMenu = this->getChildByID("arrow-menu");
        auto winSize = CCDirector::get()->getWinSize();
        arrowMenu->removeChildByID("next-arrow");
        arrowMenu->removeChildByID("prev-arrow");

        if (this->m_customPage < 14) {
            auto leftArrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            leftArrowSprite->setFlipX(true);
            auto nextBtn = CCMenuItemSpriteExtra::create(
                leftArrowSprite,
                this,
                menu_selector(DemonBrowserLayer::onNextPage)
            );
            arrowMenu->addChild(nextBtn);
            nextBtn->setPosition(winSize.width - 25.f, winSize.height / 2.f);
            nextBtn->setID("next-arrow");
        }
        if (this->m_customPage > 0) {
            auto prevBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
                this,
                menu_selector(DemonBrowserLayer::onPrevPage)
            );
            arrowMenu->addChild(prevBtn);
            prevBtn->setPosition(25.f, winSize.height / 2.f);
            prevBtn->setID("prev-arrow");
        }
    }

    void onPrevPage(cocos2d::CCObject* sender) {
        this->m_customPage--;
        this->loadPageByIndex(this->m_customPage);
        auto arrowMenu = this->getChildByID("arrow-menu");
        auto winSize = CCDirector::get()->getWinSize();
        arrowMenu->removeChildByID("next-arrow");
        arrowMenu->removeChildByID("prev-arrow");
        if (this->m_customPage < 14) {
            auto leftArrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            leftArrowSprite->setFlipX(true);
            auto nextBtn = CCMenuItemSpriteExtra::create(
                leftArrowSprite,
                this,
                menu_selector(DemonBrowserLayer::onNextPage)
            );
            arrowMenu->addChild(nextBtn);
            nextBtn->setPosition(winSize.width - 25.f, winSize.height / 2.f);
            nextBtn->setID("next-arrow");
        }
        if (this->m_customPage > 0) {
            auto prevBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
                this,
                menu_selector(DemonBrowserLayer::onPrevPage)
            );
            arrowMenu->addChild(prevBtn);
            prevBtn->setPosition(25.f, winSize.height / 2.f);
            prevBtn->setID("prev-arrow");
        }
    }
};

class $modify(RankedLevelCell, LevelCell) {
    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);
        int id = level->m_levelID;
        auto it = s_demonRanks.find(std::to_string(id));

        if (it == s_demonRanks.end()) return;

        int demonRank = it->second;

        std::string rankText = "#" + std::to_string(demonRank);
        auto rankLabel = cocos2d::CCLabelBMFont::create(rankText.c_str(), "bigFont.fnt");
        rankLabel->setScale(0.6f); 
        
        if (auto mainLayer = this->getChildByID("main-layer")) {
            rankLabel->setScale(0.5f);
            rankLabel->setPosition({ 325.f, 15.f }); 
            rankLabel->setID("pointercrate-rank-label");
            mainLayer->addChild(rankLabel);
        }
    }
};

class $modify(MyDemonFilterSelectLayer, DemonFilterSelectLayer) {

    private:
    void onButton(CCObject* sender) {
        geode::log::info("Button clicked!");
        
        auto newLayer = DemonBrowserLayer::create();
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