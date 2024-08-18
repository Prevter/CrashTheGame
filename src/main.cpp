#include <Geode/Geode.hpp>
#include "CrashPickerLayer.hpp"

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		static_assert(sizeof(std::vector) == 24);
		static_assert(sizeof(std::map) == 24);
		static_assert(sizeof(std::unordered_map) == 24);
		static_assert(sizeof(std::set) == 24);
		static_assert(sizeof(std::unordered_set) == 24);

		auto crashButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_freeStuffBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onCrashButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(crashButton);

		crashButton->setID("crash-button"_spr);

		menu->updateLayout();
		return true;
	}

	void onCrashButton(CCObject*) {
		auto layer = CrashPickerLayer::create();
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
	}
};
