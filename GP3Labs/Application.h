#pragma once

#include "Entity.h"

#define WINDOW_W Application::GetInstance()->GetWindowWidth()
#define WINDOW_H Application::GetInstance()->GetWindowHeight()

enum AppState
{
	INITILISING, RUNNING, QUITTING
};

class Camera; //forward decleration

class Application
{
private:
	std::vector<Entity*> m_entities;
	Camera* m_mainCamera = nullptr;

	std::string f_path = ASSET_PATH + "Arial.ttf";

	const int js_Deadzone = 5000;
	SDL_Joystick* m_gameController = nullptr;
	SDL_Haptic* m_gameControllerHaptic = nullptr;

	//private variables
	static Application* m_application;
	SDL_Window* m_window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* fontSurface = nullptr;
	SDL_Texture* message = nullptr;
	SDL_Color fontColor;
	SDL_Rect fontRect;
	TTF_Font* font = nullptr;
	SDL_GLContext m_glContext = nullptr;
	int m_windowWidth = 1280;
	int m_windowHeight = 720;
	AppState m_appState = AppState::INITILISING;
	float m_worldDeltaTime = 0.f;

	//Used to check FPS
	time_t s_time, c_time;
	int frames = 0;
	
	//private functions
	Application();
	void Init();
	void OpenGlInit();
	void Loop();
	void Quit();
	void Update(float deltaTime);
	void Render();	void GameInit();	void Movement();

	//Joystick direction
	float xDir = 1.f;
	float yDir = 1.f;

public:
	//public functions
	~Application();
	static Application* GetInstance();
	void Run();
	inline int GetWindowHeight() { return m_windowHeight; }
	inline int GetWindowWidth() { return m_windowWidth; }

	inline Camera* GetCamera() { return m_mainCamera; }
	void SetCamera(Camera* camera);
};