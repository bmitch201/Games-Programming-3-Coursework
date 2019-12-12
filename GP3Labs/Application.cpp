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

	//Initalise the joystick
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0)
	{
		LOG_DEBUG(SDL_GetError(), Log::Error);
	}

	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating window
	m_window = SDL_CreateWindow("GP3-COURSEWORK", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);
	SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	//Check for amount of joysticks
	if (SDL_NumJoysticks > 0)
	{
		m_gameController = SDL_JoystickOpen(0);
		
		if (m_gameController == nullptr)
		{
			LOG_DEBUG("Game controller not found ", Log::Error);
		}
		else
		{
			m_gameControllerHaptic = SDL_HapticOpenFromJoystick(m_gameController);
			
			if (m_gameControllerHaptic == nullptr)
			{
				LOG_DEBUG("Game controller haptic not found", Log::Error);
			}
			else
			{
				if (SDL_HapticRumbleInit(m_gameControllerHaptic) < 0)
				{
					LOG_DEBUG("Unable to initalise rumble", Log::Warning);
				}
			}
		}
	}
	else
	{
		LOG_DEBUG("No controllers connected", Log::Warning);
	}

	//For the FPS counter
	renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	TTF_Init();
	font = TTF_OpenFont(f_path.c_str(), 15);
	if (font == nullptr)
	{
		LOG_DEBUG("No font loaded", Log::Error);
	}
	else
	{
		LOG_DEBUG("Font loaded from " + f_path, Log::Trace);
	}
	fontColor = { 255, 255, 0, 0 };

	fontRect.x = 10;
	fontRect.y = 0;
	fontRect.w = 50;
	fontRect.h = 50;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	OpenGlInit();
	GameInit();
}

void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls will refer to)
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
	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	
	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::GameInit()
{
	//Loading all resources
	//Objects
	Resources::GetInstance()->AddModel("Crate1.obj");
	Resources::GetInstance()->AddModel("Skull.obj");
	Resources::GetInstance()->AddModel("Golf Ball.obj");
	Resources::GetInstance()->AddModel("45.obj");
	//Shaders
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "simple_Vert.glsl", ASSET_PATH + "simple_Frag.glsl"), "simple");
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "blinn-phong_Vert.glsl", ASSET_PATH + "blinn-phong_Frag.glsl"), "blinn-phong");
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "normal_Vert.glsl", ASSET_PATH + "normal_Frag.glsl"), "normal");
	//Textures
	Resources::GetInstance()->AddTexture("crate.jpg");
	Resources::GetInstance()->AddTexture("metal_scratches.jpg");
	Resources::GetInstance()->AddTexture("Metal.jpg");
	Resources::GetInstance()->AddTexture("Rubber.jpg");
	Resources::GetInstance()->AddTexture("Bone-Texture.jpg");

	Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer( Resources::GetInstance()->GetModel("Crate1.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("crate.jpg")));
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(50.f, 1.f, 50.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(50.f, 1.f, 50.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Crate1.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("crate.jpg")));
	m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -5.f, -70.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(50.f, 5.f, 1.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(50.f, 5.f, 1.f));
	
	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Crate1.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("crate.jpg")));
	m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -5.f, 30.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(50.f, 5.f, 1.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(50.f, 5.f, 1.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Crate1.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("crate.jpg")));
	m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(-50.f, -5.f, -20.f));
	a->GetTransform()->SetRotation(glm::quat(90.f, 0, 90.f, 0));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(50.f, 5.f, 1.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(50.f, 5.f, 1.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Crate1.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("crate.jpg")));
	m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(50.f, -5.f, -20.f));
	a->GetTransform()->SetRotation(glm::quat(90.f, 0, 90.f, 0));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(50.f, 5.f, 1.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(50.f, 5.f, 1.f));

	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	cc->Start();
	
	for (int i = 0; i < 5; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		if (i == 0)
		{
			a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Skull.obj"), Resources::GetInstance()->GetShader("blinn-phong"), Resources::GetInstance()->GetTexture("Bone-Texture.jpg")));
			a->GetTransform()->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));
			a->GetTransform()->SetPosition(glm::vec3(5.f * i, 0, -20.f));
			a->AddComponent<RigidBody>();
			a->GetComponent<RigidBody>()->Init
			(
				new BoxShape(glm::vec3(1.f, .05f, 1.f))
			);
		}
		else if (i == 1)
		{
			a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Golf Ball.obj"), Resources::GetInstance()->GetShader("blinn-phong"), Resources::GetInstance()->GetTexture("Rubber.jpg")));
			a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
			a->GetTransform()->SetPosition(glm::vec3(5.f * i, 0, -20.f));
			a->AddComponent<RigidBody>();
			a->GetComponent<RigidBody>()->Init
			(
				new SphereShape(2.1f)

			);
		}
		else if (i == 2)
		{
			a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("45.obj"), Resources::GetInstance()->GetShader("blinn-phong"), Resources::GetInstance()->GetTexture("metal_scratches.jpg")));
			a->GetTransform()->SetScale(glm::vec3(3.f, 3.f, 3.f));
			a->GetTransform()->SetPosition(glm::vec3(5.f * i, 0, -20.f));
			a->AddComponent<RigidBody>();
			a->GetComponent<RigidBody>()->Init
			(
				new CylinderShape(glm::vec3(.6f, 1.25f, .6f))
			);
		}
		else if (i == 3)
		{
			a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Golf Ball.obj"), Resources::GetInstance()->GetShader("blinn-phong"), Resources::GetInstance()->GetTexture("Metal.jpg")));
			a->GetTransform()->SetScale(glm::vec3(.8f, .8f, .8f));
			a->GetTransform()->SetPosition(glm::vec3(5.f * i, 0, -20.f));
			a->AddComponent<RigidBody>();
			a->GetComponent<RigidBody>()->Init
			(
				//new BoxShape(glm::vec3(1.f, .05f, 1.f))
				//new CapsuleShape(1.f, 0.25f)
				//new ConeShape(1.f, 1.f)
				//new CylinderShape(glm::vec3(1.f, 1.f, 1.f))
				new SphereShape(2.1f)
			);
		}
		else
		{
			a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Skull.obj"), Resources::GetInstance()->GetShader("blinn-phong"), Resources::GetInstance()->GetTexture("Bone-Texture.jpg")));
			a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));		
			a->GetTransform()->SetPosition(glm::vec3(5.f * i, 0, -20.f));
			a->AddComponent<RigidBody>();
			a->GetComponent<RigidBody>()->Init
			(
			new BoxShape(glm::vec3(1.f, .5f, 1.f))
			//new CapsuleShape(1.f, 0.25f)
			//new ConeShape(1.f, 1.f)
			//new CylinderShape(glm::vec3(1.f, 1.f, 1.f))
			//new SphereShape(1.f)
			);
		}	
	}


	time(&s_time);
}

void Application::Loop()
{	
	m_appState = AppState::RUNNING;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	INPUT->SetAxis(0, false);
	INPUT->SetAxis(1, false);
	INPUT->SetAxis(2, false);
	INPUT->SetAxis(3, false);
	INPUT->SetAxis(4, false);
	INPUT->SetAxis(5, false);
	INPUT->SetAxis(6, false);
	INPUT->SetAxis(7, false);

	INPUT->SetButton(0, false);
	INPUT->SetButton(1, false);
	INPUT->SetButton(4, false);
	INPUT->SetButton(5, false);
	
	while (m_appState != AppState::QUITTING)
	{
		frames++;
		time(&c_time);
		SDL_RenderClear(renderer);

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
					for (int i = 6; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyForce(btVector3(0, 20, 0));
					}
					break;				
				case SDLK_DOWN:
					for (int i = 6; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyForce(btVector3(0, -20, 0));
					}
					break;
				case SDLK_LEFT:
					for (int i = 6; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyTorque(btVector3(0, 30, 0));
					}
					break;
				case SDLK_RIGHT:
					for (int i = 6; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyTorque(btVector3(0, -30, 0));
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
				m_entities.at(5)->GetTransform()->RotateEulerAxis(movementPos.x, glm::vec3(0,1,0));
				m_entities.at(5)->GetTransform()->RotateEulerAxis(movementPos.y, -m_entities.at(5)->GetTransform()->GetRight());
				break;
			case SDL_JOYAXISMOTION:	
				if (event.jaxis.which == 0)
				{				
					if (event.jaxis.axis == 0)
					{
						if (event.jaxis.value < -js_Deadzone)
						{
							INPUT->SetAxis(0, true);
						}
						else if (event.jaxis.value > js_Deadzone)
						{
							INPUT->SetAxis(1, true);
						}
						else
						{
							INPUT->SetAxis(0, false);
							INPUT->SetAxis(1, false);
						}
					}
					else if (event.jaxis.axis == 1)
					{
						if (event.jaxis.value < -js_Deadzone)
						{
							INPUT->SetAxis(2, true);
						}
						else if (event.jaxis.value > js_Deadzone)
						{
							INPUT->SetAxis(3, true);
						}
						else
						{
							INPUT->SetAxis(2, false);
							INPUT->SetAxis(3, false);
						}
					}	
					
					if (event.jaxis.axis == 3)
					{
						if (event.jaxis.value < -js_Deadzone)
						{
							INPUT->SetAxis(4, true);
						}
						else if (event.jaxis.value > js_Deadzone)
						{
							INPUT->SetAxis(5, true);
						}
						else
						{
							INPUT->SetAxis(4, false);
							INPUT->SetAxis(5, false);
						}
					}
					else if (event.jaxis.axis == 4)
					{
						if (event.jaxis.value < -js_Deadzone)
						{
							INPUT->SetAxis(6, true);
							
						}
						else if (event.jaxis.value > js_Deadzone)
						{
							INPUT->SetAxis(7, true);
						}
						else
						{
							INPUT->SetAxis(6, false);
							INPUT->SetAxis(7, false);
						}
					}
				}
				break;
			case SDL_JOYBUTTONDOWN:
				
				if (event.jbutton.button == 0)
				{
					INPUT->SetButton(0, true);
				}

				if (event.jbutton.button == 1)
				{
					INPUT->SetButton(1, true);
				}
				
				if (event.jbutton.button == 4)
				{
					INPUT->SetButton(4, true);
				}
				
				if (event.jbutton.button == 5)
				{
					INPUT->SetButton(5, true);
				}

				if (event.jbutton.button == 7)
				{
					m_appState = AppState::QUITTING;
					break;
				}
				
				break;
			case SDL_JOYBUTTONUP:

				if (event.jbutton.button == 0)
				{
					INPUT->SetButton(0, false);
				}

				if (event.jbutton.button == 1)
				{
					INPUT->SetButton(1, false);
				}
				
				if (event.jbutton.button == 4)
				{
					INPUT->SetButton(4, false);
				}

				if (event.jbutton.button == 5)
				{
					INPUT->SetButton(5, false);
				}

				break;
			}
		}

		Movement();
		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;
		Physics::GetInstance()->Update(deltaTime);
		
		//Set Uniforms to allow for multiple shaders!
		Resources::GetInstance()->GetShader("simple")->Use();
		Resources::GetInstance()->GetShader("simple")->setMat4("view", Application::GetInstance()->GetCamera()->GetView());
		Resources::GetInstance()->GetShader("simple")->setMat4("projection", Application::GetInstance()->GetCamera()->GetProj());
		Resources::GetInstance()->GetShader("simple")->setVec3("objectColor", glm::vec3(1.f, 1.f, 1.f));
		
		Resources::GetInstance()->GetShader("blinn-phong")->Use();
		Resources::GetInstance()->GetShader("blinn-phong")->setMat4("view", Application::GetInstance()->GetCamera()->GetView());
		Resources::GetInstance()->GetShader("blinn-phong")->setMat4("projection", Application::GetInstance()->GetCamera()->GetProj());
		Resources::GetInstance()->GetShader("blinn-phong")->setVec3("viewPos", Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition());
		Resources::GetInstance()->GetShader("blinn-phong")->setVec4("lightColor", glm::vec4(1.f, 1.f, 1.f, 0.5f));
		Resources::GetInstance()->GetShader("blinn-phong")->setVec3("lightDir", glm::vec3(-5.f, 6.f, 7.f));	

		Update(deltaTime);

		if (difftime(c_time, s_time) >= 1.0)
		{
			LOG_DEBUG("Frames: " + std::to_string(frames), Log::Trace);		

			SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);

			std::string text = std::to_string(frames);			
			
			SDL_FreeSurface(fontSurface);
			fontSurface = TTF_RenderText_Solid(font, text.c_str(), fontColor);
			message = SDL_CreateTextureFromSurface(renderer, fontSurface);

			frames = 0;
			time(&s_time);
		}
		
		SDL_RenderCopy(renderer, message, NULL, &fontRect);
		
		Render();

		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Movement()
{	
	if (INPUT->GetAxis(0))
	{
		m_entities.at(5)->GetTransform()->AddPosition(-m_entities.at(5)->GetTransform()->GetRight());
	}
	else if (INPUT->GetAxis(1))
	{
		
		m_entities.at(5)->GetTransform()->AddPosition(m_entities.at(5)->GetTransform()->GetRight());
	}	
	
	if (INPUT->GetAxis(2))
	{
		m_entities.at(5)->GetTransform()->AddPosition(m_entities.at(5)->GetTransform()->GetForward());
	}
	else if (INPUT->GetAxis(3))
	{
		m_entities.at(5)->GetTransform()->AddPosition(-m_entities.at(5)->GetTransform()->GetForward());
	}	
	
	if(INPUT->GetKey(SDLK_w))
	{ 
		m_entities.at(5)->GetTransform()->AddPosition(m_entities.at(5)->GetTransform()->GetForward());
	}
	else if (INPUT->GetKey(SDLK_s))
	{
		m_entities.at(5)->GetTransform()->AddPosition(-m_entities.at(5)->GetTransform()->GetForward());
	}

	if (INPUT->GetKey(SDLK_a))
	{
		m_entities.at(5)->GetTransform()->AddPosition(-m_entities.at(5)->GetTransform()->GetRight());
	}
	else if (INPUT->GetKey(SDLK_d))
	{
		m_entities.at(5)->GetTransform()->AddPosition(m_entities.at(5)->GetTransform()->GetRight());
	}

	if (INPUT->GetAxis(4))
	{
		m_entities.at(5)->GetTransform()->RotateEulerAxis(-xDir, glm::vec3(0, 1, 0));
	}
	else if (INPUT->GetAxis(5))
	{
		m_entities.at(5)->GetTransform()->RotateEulerAxis(xDir, glm::vec3(0, 1, 0));
	}

	if (INPUT->GetAxis(6))
	{
		m_entities.at(5)->GetTransform()->RotateEulerAxis(yDir, m_entities.at(5)->GetTransform()->GetRight());
	}
	else if (INPUT->GetAxis(7))
	{
		m_entities.at(5)->GetTransform()->RotateEulerAxis(-yDir, m_entities.at(5)->GetTransform()->GetRight());
	}

	if (INPUT->GetKey(SDLK_SPACE))
	{
		m_entities.at(5)->GetTransform()->AddPosition(glm::vec3(0, 1, 0));
	}
	else if (INPUT->GetKey(SDLK_LSHIFT))
	{
		m_entities.at(5)->GetTransform()->AddPosition(glm::vec3(0, -1, 0));
	}

	if (INPUT->GetButton(0))
	{
		m_entities.at(5)->GetTransform()->AddPosition(glm::vec3(0, 1, 0));
	}
	else if (INPUT->GetButton(1))
	{
		m_entities.at(5)->GetTransform()->AddPosition(glm::vec3(0, -1, 0));
	}

	if (INPUT->GetButton(4))
	{
		SDL_HapticRumblePlay(m_gameControllerHaptic, 0.5, 500);
		for (int i = 6; i < m_entities.size(); i++)
		{
			m_entities.at(i)->GetComponent<RigidBody>()->ApplyTorque(btVector3(0, -30, 0));
		}
	}
	
	if (INPUT->GetButton(5))
	{
		SDL_HapticRumblePlay(m_gameControllerHaptic, 0.5, 500);
		for (int i = 6; i < m_entities.size(); i++)
		{
			m_entities.at(i)->GetComponent<RigidBody>()->ApplyForce(btVector3(0, -20, 0));
		}
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_JoystickClose(m_gameController);
	SDL_HapticClose(m_gameControllerHaptic);
	m_gameController = nullptr;
	m_gameControllerHaptic = nullptr;
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyTexture(message);
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
	//SDL_RenderPresent(renderer);
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