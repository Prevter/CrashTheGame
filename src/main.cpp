#include <Geode/Geode.hpp>
#include "CrashPickerLayer.hpp"

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		static_assert(sizeof(std::vector<int>) == 24);
		static_assert(sizeof(std::map<int, int>) == 24);
		static_assert(sizeof(std::unordered_map<int, int>) == 40);
		static_assert(sizeof(std::set<int>) == 24);
		static_assert(sizeof(std::unordered_set<int>) == 40);

		static_assert(sizeof(std::vector<bool>) == 24);
		static_assert(sizeof(std::map<int, bool>) == 24);
		static_assert(sizeof(std::unordered_map<int, bool>) == 40);
		static_assert(sizeof(std::set<bool>) == 24);
		static_assert(sizeof(std::unordered_set<bool>) == 40);

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
