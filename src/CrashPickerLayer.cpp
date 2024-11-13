#include "CrashPickerLayer.hpp"
#include "utils.hpp"

const char *const FONTS[] = {
    "bigFont.fnt", "goldFont.fnt", "gjFont02.fnt",
    "gjFont03.fnt", "gjFont05.fnt", "gjFont06.fnt",
    "gjFont07.fnt", "gjFont09.fnt", "gjFont11.fnt",
    "gjFont12.fnt", "gjFont13.fnt", "gjFont14.fnt", "gjFont15.fnt",
    "gjFont17.fnt", "gjFont18.fnt", "gjFont19.fnt",
    "gjFont20.fnt", "gjFont21.fnt", "gjFont22.fnt",
    "gjFont26.fnt", "gjFont27.fnt", "gjFont29.fnt", "gjFont30.fnt",
    "gjFont32.fnt", "gjFont33.fnt", "gjFont34.fnt", "gjFont35.fnt",
    "gjFont36.fnt", "gjFont39.fnt", "gjFont41.fnt",
    "gjFont42.fnt", "gjFont43.fnt", "gjFont44.fnt", "gjFont45.fnt",
    "gjFont46.fnt", "gjFont47.fnt", "gjFont48.fnt", "gjFont50.fnt",
    "gjFont51.fnt", "gjFont53.fnt", "gjFont54.fnt",
    "gjFont55.fnt", "gjFont57.fnt", "gjFont58.fnt", "gjFont59.fnt",
}; // Some fonts were removed for being too long/high

const size_t FONT_COUNT = sizeof(FONTS) / sizeof(FONTS[0]);

const char* const SCARY_MESSAGES[] = {
    "BOO!", "OOPS!",
    "OH NO!", "CRASH!",
    "ERROR!", "RIP!",
    "DEAD!", "FATAL!",
    "NOPE!", "NO!",
    "HELLO!", "GOODBYE!",
};

const size_t SCARY_MESSAGES_SIZE = sizeof(SCARY_MESSAGES) / sizeof(SCARY_MESSAGES[0]);

static bool s_pickedTheFont = false;
static const char* getRandomFont() {
    static const char* font = nullptr;
    if (!s_pickedTheFont) {
        // Have a 1 in 256 change to load an invalid font
        if (util::randInt(0, 255) == 0) {
            font = "invalidFont.fnt";
            s_pickedTheFont = true;
            return font;
        }

        font = FONTS[util::randInt(0, FONT_COUNT - 1)];
        s_pickedTheFont = true;
    }
    return font;
}

CrashPickerLayer* CrashPickerLayer::create() {
    s_pickedTheFont = false;
    auto ret = new CrashPickerLayer();
    if (ret->initAnchored(360, 240, fmt::format("Mega Crash v{}", util::randInt(-1, 69)), "GJ_square01.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CrashPickerLayer::setup(const std::string& title) {
    auto menu = cocos2d::CCMenu::create();
    this->addChild(menu);

    geode::log::info("Creating crash picker with font: {}", getRandomFont());

    auto titleLabel = cocos2d::CCLabelBMFont::create(title.c_str(), getRandomFont());
    titleLabel->setPosition(0.f, 100.f);
    titleLabel->setScale(0.5f);
    titleLabel->setID("title"_spr);
    menu->addChild(titleLabel);

    const size_t columns = 2;
    const size_t size = CrashEngine::CrashCount;
    const float columnWidth = m_size.width / columns * 1.6f;

    for (size_t i = 0; i < size; i++) {
        auto btnSprite = ButtonSprite::create(CrashEngine::CrashNames[i], getRandomFont(), "GJ_button_04.png");
        btnSprite->setScale(0.5f);
        btnSprite->m_BGSprite->setContentWidth(columnWidth);

        auto button = CCMenuItemSpriteExtra::create(btnSprite, this, menu_selector(CrashPickerLayer::onCrashButton));
        button->setUserData(reinterpret_cast<void*>(i));

        size_t column = i % columns;
        size_t row = i / columns;
        float x = -75.f + column * 150.f;
        float y = 75.f - row * 25.f;
        button->setPosition(x, y);

        menu->addChild(button);
    }

    this->setID("crash-picker"_spr);
    return true;
}

void CrashPickerLayer::onCrashButton(CCObject* sender) {
    const char* message = SCARY_MESSAGES[util::randInt(0, SCARY_MESSAGES_SIZE - 1)];
    auto label = CCLabelBMFont::create(message, getRandomFont());
    auto winSize = CCDirector::get()->getWinSize();
    label->setPosition(winSize.width / 2, winSize.height / 2);
    label->setScale(2.5f);
    label->setAnchorPoint(ccp(0.5f, 0.5f));
    CCScene::get()->addChild(label);

    auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
    auto userData = reinterpret_cast<size_t>(button->getUserData());
    auto type = static_cast<CrashEngine::CrashType>(userData);
    m_crashType = type;

    geode::queueInMainThread([this]{
        CrashEngine::trigger(m_crashType);
    });
}