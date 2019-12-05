#include "pch.h"
#include "Application.h"
#include "Common.h" // Didn't exist, but Log.h was missing for functions LOG_DEBUG / CHECK_GL_ERROR
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "ConeShape.h"
#include "SphereShape.h"
#include "CylinderShape.h"
#include "CapsuleShape.h"
#include "RigidBody.h"

Application* Application::m_application = nullptr;

Application::Application()
{
}

void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), Log::Error);
		exit(-1);
	}
	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating window
	m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight,
		SDL_WINDOW_OPENGL);
	SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	OpenGlInit();
	GameInit();
}

void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls
	//will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	// Using m_  to refer it as it being a member 
	CHECK_GL_ERROR();
	SDL_GL_SetSwapInterval(1);
	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) <<
			std::endl;
		exit(-1);
	}
	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));
	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));
	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));
	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//turn on back face culling
	//GL_ATTEMPT(glEnable(GL_CULL_FACE));
	
	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::GameInit()
{
	//Loading all resources
	Resources::GetInstance()->AddModel("Cube.obj");
	Resources::GetInstance()->AddModel("stickman.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "simple_Vert.glsl", ASSET_PATH + "simple_Frag.glsl"), "simple");

	Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer( Resources::GetInstance()->GetModel("Cube.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("Wood.jpg")));
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f, 100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));

	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	cc->Start();
	
	Resources::GetInstance()->AddModel("Skull.obj");
	Resources::GetInstance()->AddTexture("Bone-Texture.jpg");
	
	for (int i = 0; i < 100; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		if (i == 2)
		{
			a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("stickman.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("Wood.jpg")));
		}
		else
		{
			a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Skull.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("Bone-Texture.jpg")));
		}
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init
		(
			new BoxShape(glm::vec3(1.f, 1.f, 1.f))
			//new CapsuleShape(1.f, 0.25f)
			//new ConeShape(1.f, 1.f)
			//new CylinderShape(glm::vec3(1.f, 1.f, 1.f))
			//new SphereShape(1.f)
			
		);
		a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
	}
}

void Application::Loop()
{
	m_appState = AppState::RUNNING;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;
	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:
				INPUT->SetKey(event.key.keysym.sym, true);
				
				switch (event.key.keysym.sym)
				{

				case SDLK_UP:
					for (int i = 2; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyForce(btVector3(0,30,0));
					}
					break;
				case SDLK_LEFT:
					for (int i = 2; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyTorque(btVector3(0, 30, 0));
					}
					break;
				case SDLK_ESCAPE:
					m_appState = AppState::QUITTING;
					break;
				}
				break;
				//record when the user releases a key
			case SDL_KEYUP:
				INPUT->SetKey(event.key.keysym.sym, false);
				break;
			case SDL_MOUSEMOTION: 
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel,event.motion.yrel));
				glm::ivec2 movementPos = INPUT->GetMouseDelta();
				m_entities.at(1)->GetTransform()->RotateEulerAxis(movementPos.x, -glm::vec3(0,1,0));
				m_entities.at(1)->GetTransform()->RotateEulerAxis(movementPos.y, -m_entities.at(1)->GetTransform()->GetRight());
				break;
			}
		}
		Movement();
		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;
		Physics::GetInstance()->Update(deltaTime);

		Update(deltaTime);
		Render();

		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Movement()
{	
	if(INPUT->GetKey(SDLK_w))
	{ 
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetForward());
	}
	else if (INPUT->GetKey(SDLK_s))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetForward());
	}

	if (INPUT->GetKey(SDLK_a))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetRight());
	}
	else if (INPUT->GetKey(SDLK_d))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetRight());
	}

	if (INPUT->GetKey(SDLK_SPACE))
	{
		m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0, 1, 0));
	}
	else if (INPUT->GetKey(SDLK_LSHIFT))
	{
		m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0, -1, 0));
	}
}

void Application::ShaderUpdate()
{
	
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
	m_appState = AppState::QUITTING;
}

Application::~Application()
{
}

Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}



void Application::Run()
{
	Init();
	Loop();
	Quit();
}

void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

void Application::Render()
{
	/* Clear context */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}