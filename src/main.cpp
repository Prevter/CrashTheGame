#include <Geode/Geode.hpp>
#include "CrashPickerLayer.hpp"

constexpr size_t vector_size = sizeof(std::vector);
constexpr size_t map_size = sizeof(std::map);
constexpr size_t umap_size = sizeof(std::unordered_map);
constexpr size_t set_size = sizeof(std::set);
constexpr size_t uset_size = sizeof(std::unordered_set);

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		static_assert(vector_size == 20);
		static_assert(map_size == 20);
		static_assert(umap_size == 20);
		static_assert(set_size == 20);
		static_assert(uset_size == 20);

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
