#include <Geode/Geode.hpp>
#include "CrashPickerLayer.hpp"

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	struct Fields {
		cocos2d::CCSprite* m_buttonSprite;
		float m_hue;
	};

	static cocos2d::ccColor3B colorFromHSV(float h, float s, float v) {
		float r, g, b;
		if (s == 0) {
			r = g = b = v;
		} else {
			h *= 6.f;
			int i = h;
			float f = h - i;
			float p = v * (1 - s);
			float q = v * (1 - s * f);
			float t = v * (1 - s * (1 - f));
			switch (i) {
				case 0: r = v, g = t, b = p; break;
				case 1: r = q, g = v, b = p; break;
				case 2: r = p, g = v, b = t; break;
				case 3: r = p, g = q, b = v; break;
				case 4: r = t, g = p, b = v; break;
				case 5: r = v, g = p, b = q; break;
			}
		}
		return ccc3(r * 255, g * 255, b * 255);
	}

	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto sprite = CCSprite::createWithSpriteFrameName("GJ_freeStuffBtn_001.png");
		auto crashButton = CCMenuItemSpriteExtra::create(
			sprite, this, menu_selector(MyMenuLayer::onCrashButton)
		);

		m_fields->m_buttonSprite = sprite;
		this->schedule(schedule_selector(MyMenuLayer::updateColor));
		updateColor(0);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(crashButton);

		crashButton->setID("crash-button"_spr);

		menu->updateLayout();
		return true;
	}

	void updateColor(float dt) {
		auto fields = m_fields.self();
		fields->m_hue += dt * 0.1f;
		if (fields->m_hue > 1) {
			fields->m_hue -= 1;
		}
		fields->m_buttonSprite->setColor(colorFromHSV(fields->m_hue, 1, 1));
	}

	void onCrashButton(CCObject*) {
		auto layer = CrashPickerLayer::create();
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
	}
};
