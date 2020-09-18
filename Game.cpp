// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <iostream>

const int thickness = 15;
const float paddleH = 100.0f;//tamanho da raquete
int golsEsq = 0;
int golsDir = 0;

Game::Game()
	:mWindow(nullptr)//para criar uma janela
	, mRenderer(nullptr)//para fins de renderização na tela
	, mTicksCount(0)//para guardar o tempo decorrido no jogo
	, mIsRunning(true)//verificar se o jogo ainda deve continuar sendo executado
	, mPaddleDir(0)
{

}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1000,	// Width of window
		500,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mPaddlePos.x = 10.0f;//posição inicial da raquete eixo x
	mPaddlePos.y = 490.0f / 2.0f;//posição inicial da raquee eixo y

	mPaddlePos2.x = 980.0f;//posição inicial da raquete eixo x
	mPaddlePos2.y = 490.0f / 2.0f;//posição inicial da raquee eixo y

	mBallPos.x = 998.0f / 2.0f;//posição da bola 1 eixo x
	mBallPos.y = 502.0f / 2.0f;//posição da bola 1 eixo y
	mBallVel.x = -200.0f;//velocidade de movimentação da bola 1no eixo x
	mBallVel.y = 235.0f;//velocidade de movimentação da bola 1 no eixo y

	mBallPos2.x = 1002.0f / 2.0f;//posição da bola 2 eixo x
	mBallPos2.y = 498.0f / 2.0f;//posição da bola 2 eixo y
	mBallVel2.x = -200.0f;//velocidade de movimentação da bola 1no eixo x
	mBallVel2.y = 150.0f;//velocidade de movimentação da bola 2 no eixo y
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;//evento, inputs do jogador são armazenados aqui
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard - podemos buscar por alguma tecla específica pressionada no teclado, nesse caso, Escape
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Update paddle direction based on W/S keys - atualize a direção da raquete com base na entrada do jogador
	// W -> move a raquete para cima, Y -> move a raquete para baixo
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

	mPaddleDir2 = 0;

	if (mBallPos.x > mBallPos2.x) {
		if (mBallPos.y > mPaddlePos2.y)
		{
			mPaddleDir2 += 1;
		}
		else if (mBallPos.y < mPaddlePos2.y)
		{
			mPaddleDir2 -= 1;
		}
	}
	else {
		if (mBallPos2.y > mPaddlePos2.y)
		{
			mPaddleDir2 += 1;
		}
		else if (mBallPos2.y < mPaddlePos2.y)
		{
			mPaddleDir2 -= 1;
		}
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame - limitando os frames
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds) - calcula o delta time para atualização do jogo
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value - valores máximos de delta time
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame) - atualize o tempo corrente do jogo
	mTicksCount = SDL_GetTicks();

	// Update paddle position based on direction - atualiza a posição da raquete
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (515.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 515.0f - paddleH / 2.0f - thickness;
		}
	}

	if (mPaddleDir2 != 0)
	{
		mPaddlePos2.y += mPaddleDir2 * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos2.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos2.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos2.y > (500.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos2.y = 500.0f - paddleH / 2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity - atualiza a posição da bola com base na sua velocidade
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	mBallPos2.x += mBallVel2.x * deltaTime;
	mBallPos2.y += mBallVel2.y * deltaTime;

	//Atuliza a posição da bola se ela colidiu com a raquete
	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddlePos.y - mBallPos.y;

	float diff2 = mPaddlePos.y - mBallPos2.y;
	// Take absolute value of difference
	//verifica se a bola 1 está na area da raquete e na mesma posição no eixo x
	diff = (diff > 0.0f) ? diff : -diff;
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
	if (
		// Our y-difference is small enough
		diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	if (
		// Our y-difference is small enough
		diff2 <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos2.x <= 25.0f && mBallPos2.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel2.x < 0.0f)
	{
		mBallVel2.x *= -1.0f;
	}

	float diff3 = mPaddlePos2.y - mBallPos.y;

	float diff4 = mPaddlePos2.y - mBallPos2.y;
	// Take absolute value of difference
	//verifica se a bola 1 está na area da raquete e na mesma posição no eixo x
	diff3 = (diff3 > 0.0f) ? diff3 : -diff3;
	diff4 = (diff4 > 0.0f) ? diff4 : -diff4;
	if (
		// Our y-difference is small enough
		diff3 <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x >= 980.0f && mBallPos.x <= 990.0f &&
		// The ball is moving to the left
		mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	if (
		// Our y-difference is small enough
		diff4 <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos2.x >= 980.0f && mBallPos2.x <= 990.0f &&
		// The ball is moving to the left
		mBallVel2.x > 0.0f)
	{
		mBallVel2.x *= -1.0f;
	}


	//Verifica se a bola saiu da tela (no lado esquerdo, onde é permitido)
	//Se sim, encerra o jogo
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f || mBallPos2.x <= 0.0f)
	{
		golsDir += 1;
		if (mBallPos.x <= 0.0f) {
			// Faz bola voltar pro meio
			mBallPos.x = 998.0f / 2.0f;//posição da bola 1 eixo x
			mBallPos.y = 502.0f / 2.0f;//posição da bola 1 eixo y
			mBallVel.x = -200.0f;//velocidade de movimentação da bola 1no eixo x
			mBallVel.y = 235.0f;//velocidade de movimentação da bola 1 no eixo y
			mBallVel.x *= -1.0f;
		}
		else if (mBallPos2.x <= 0.0f) {
			// Faz bola voltar pro meio
			mBallPos2.x = 1002.0f / 2.0f;//posição da bola 2 eixo x
			mBallPos2.y = 498.0f / 2.0f;//posição da bola 2 eixo y
			mBallVel2.x = -100.0f;//velocidade de movimentação da bola 2 no eixo x
			mBallVel2.y = 150.0f;//velocidade de movimentação da bola 2 no eixo y
			mBallVel2.x *= -1.0f;
		}
		//mIsRunning = false;
	}

	//Atulize a velocidade da bola se ela colidir com a parede do lado direito
	// Did the ball collide with the right wall?
	else if ((mBallPos.x >= (1000.0f - thickness) && mBallVel.x > 0.0f)  || (mBallPos2.x >= (1000.0f - thickness) && mBallPos2.x > 0.0f))
	{
		golsEsq += 1;
		if (mBallPos.x >= (1000.0f - thickness) && mBallVel.x > 0.0f) {
			// Faz bola voltar pro meio
			mBallPos.x = 998.0f / 2.0f;//posição da bola 1 eixo x
			mBallPos.y = 502.0f / 2.0f;//posição da bola 1 eixo y
			mBallVel.x = -200.0f;//velocidade de movimentação da bola 1no eixo x
			mBallVel.y = 235.0f;//velocidade de movimentação da bola 1 no eixo y
			mBallVel.x *= 1.0f;
		}
		else if (mBallPos2.x >= (1000.0f - thickness) && mBallPos2.x > 0.0f) {
			// Faz bola voltar pro meio
			mBallPos2.x = 1002.0f / 2.0f;//posição da bola 2 eixo x
			mBallPos2.y = 498.0f / 2.0f;//posição da bola 2 eixo y
			mBallVel2.x = -100.0f;//velocidade de movimentação da bola 2 no eixo x
			mBallVel2.y = 150.0f;//velocidade de movimentação da bola 2 no eixo y
			mBallVel2.x *= 1.0f;
		}
	}

	//Atualize as posições das bolas se ela colidir com a parede de cima
	// Did the ball collide with the top wall?
	if ((mBallPos.y <= thickness && mBallVel.y < 0.0f) || (mBallPos2.y <= thickness && mBallVel2.y < 0.0f))
	{
		if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
			mBallVel.y *= -1;
		}
		else if (mBallPos2.y <= thickness && mBallVel2.y < 0.0f) {
			mBallVel2.y *= -1;
		}
	}

	//Atualize as posições da bolas se ela colidiu com a parede de baixo
	// Did the ball collide with the bottom wall?
	else if ((mBallPos.y >= (500 - thickness) && mBallVel.y > 0.0f) || (mBallPos2.y >= (500 - thickness) && mBallVel2.y > 0.0f))
	{
		if ((mBallPos.y >= (500 - thickness) && mBallVel.y > 0.0f)) {
			mBallVel.y *= -1;
		}
		else if ((mBallPos2.y >= (500 - thickness) && mBallVel2.y > 0.0f)) {
			mBallVel2.y *= -1;
		}	
	}

	using namespace std;
	
	//Colisões bola

}

//Desenhando a tela do jogo
void Game::GenerateOutput()
{
	// Set draw color to blue - Setamos a cor original para o back buffer
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		112,		// G 
		0,	// B
		0		// A
	);

	// Clear back buffer - passamos a cor da tela para o back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls - mudamos a cor de mRenderer para o desenho dos retangulos que formaram as paredes
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 0);

	// Draw top wall - desenhando um retangulo no topo da tela, coordenada x e y = 0, 0 representa o topo esquerdo
	//primeiro definimos as coordenadas e tamanhos do triangulo
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1000,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);//finalmente, desenhamos um retangulo no topo

	//desenhamos as outras paredes apenas mudando as coordenadas de wall
	// Draw bottom wall
	wall.y = 500 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);



	//como as posições da raquete e da boal serão atualizadas a cada iteração do game loop, criamos "membros" na classe
	//Game.h para tal

	//desenhando a raquete - usando mPaddlePos que é uma struc de coordenada que foi definida em Game.h
	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),//static_cast converte de float para inteiros, pois SDL_Rect trabalha com inteiros
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_Rect paddle2{
		static_cast<int>(mPaddlePos2.x),//static_cast converte de float para inteiros, pois SDL_Rect trabalha com inteiros
		static_cast<int>(mPaddlePos2.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);

	//desenhando a bola - usando mBallPos que é uma struc de coordenadas definida como membro em Game.h
	// Draw ball
	SDL_Rect ball1{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball1);

	SDL_Rect ball2{
	static_cast<int>(mBallPos2.x - thickness / 2),
	static_cast<int>(mBallPos2.y - thickness / 2),
	thickness,
	thickness
	};

	// Atualização do Placar

	SDL_RenderFillRect(mRenderer, &ball2);
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

//Para encerrar o jogo
void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);//encerra o renderizador
	SDL_DestroyWindow(mWindow);//encerra a janela aberta
	SDL_Quit();//encerra o jogo
}
