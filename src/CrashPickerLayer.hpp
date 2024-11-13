#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "CrashEngine.hpp"

using namespace geode::prelude;

/// @brief A layer that allows the user to pick a crash type.
class CrashPickerLayer : public geode::Popup<const std::string&> {
public:
    /// @brief Creates a new CrashPickerLayer.
	static CrashPickerLayer* create();

    /// @brief Adds all nodes to the layer.
	bool setup(const std::string& title);

    /// @brief Called when a crash button is pressed.
	void onCrashButton(cocos2d::CCObject* sender);

private:
    /// @brief Selected crash type.
    CrashEngine::CrashType m_crashType;
};