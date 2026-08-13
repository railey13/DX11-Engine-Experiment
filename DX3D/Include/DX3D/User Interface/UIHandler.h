#pragma once
#include <unordered_map>
#include <memory>

#include <DX3D/Prerequisites.h>

#include <IMGUI/imgui.h>
#include <IMGUI/backends/imgui_impl_dx11.h>
#include <IMGUI/backends/imgui_impl_win32.h>

#include <Windows.h>
#include <functional>

class UIHandler {
public:
	UIHandler(Game* game, HWND hwnd);
	~UIHandler();
private:
	void IMGUISetUp(HWND hwnd);
private:
	template<typename T>
	void registerUI() {
		static_assert(std::is_base_of <UI, T>::value, "T must be derive from UI Class");
		auto e = getUI<T>();
		if (!e) {
			auto id = typeid(T).hash_code();
			auto c = new T(this);
			registerUIInternal(c, id);
		}
	}

	void registerUIInternal(UI* ui, size_t id);
	UI* getUIInternal(size_t id) const;
public:
	template <typename T>
	T* getUI() const {
		static_assert(std::is_base_of<UI, T>::value, "T must be derive from UI Class");
		auto id = typeid(T).hash_code();
		return static_cast<T*>(getUIInternal(id));
	}
public:
	void draw();
	GraphicsEngine* getGraphicsEngine() { return graphEngine; }
	Game* getGame() { return m_game; }
	void setAllActive(bool flag);
	void toggleAllActive();

	void openFileDialog(std::string path, const char* title, const char* filters, std::function<void(const std::string&)> onFileSelected);
	void drawFileDialog();
private:
	std::unordered_map<size_t, std::unique_ptr<UI>> m_ui_table;
	std::function<void(const std::string&)> m_onFileSelected = nullptr;
	Game* m_game = nullptr;
	GraphicsEngine* graphEngine = nullptr;
private:
	friend class UI;
};

