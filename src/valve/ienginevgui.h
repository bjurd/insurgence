#pragma once

enum VGuiPanel_t
{
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS
};

enum PaintMode_t
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
	PAINT_CURSOR = (1 << 2), // software cursor, if appropriate
};

class IEngineVGui
{
public:
	virtual ~IEngineVGui() = 0;

	virtual void* GetPanel(VGuiPanel_t type) = 0; // TODO: vgui::VPANEL

	virtual bool IsGameUIVisible() = 0;
};

class IEngineVGuiInternal : public IEngineVGui
{
public:
	virtual void ActivateGameUI() = 0;
	virtual void Init() = 0;
	virtual void Connect() = 0;
	virtual void Shutdown() = 0;
	virtual bool SetVGUIDirectories() = 0;
	virtual bool IsInitialized() const = 0;
	virtual void* GetGameUIFactory() = 0; // CreateInterfaceFn
	virtual bool Key_Event(const void* event) = 0; // TODO: InputEvent_t&
	virtual void BackwardCompatibility_Paint() = 0;
	virtual void UpdateButtonState(const void* event) = 0; // TODO: InputEvent_t&
	virtual void PostInit() = 0;

	virtual void Paint(PaintMode_t mode) = 0;
};
