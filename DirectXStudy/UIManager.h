#pragma once

class DirectXMain;
class Hierarchy;
class ModelBrowserUI;
class Insfector;

class UIManager {
public:
	UIManager(DirectXMain* dxd);
	void UpdateUI();
private:
	Hierarchy* hierarchy;
	DirectXMain* dxdmain;
	ModelBrowserUI* modelBrowserUI;
	Insfector* insfector;
};