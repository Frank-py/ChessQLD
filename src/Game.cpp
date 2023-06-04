#include <cmath>
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "renderWindow.hpp"
#include "util.hpp"

#include "pawn.hpp"
#include "rook.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "knight.hpp"
#include "piece.hpp"

#include "game.hpp"

Game::Game() : window("never gonna give you up"){
    run();
}
void Game::run() {
    while (gameRunning)
    {

        window.updateWindowSize();
        if (isPieceSelected)
        {
           DragPiece();
        }
        handleEvents();
        //std::vector<glm::vec2> temp = {{1000,1000}};
        window.fullRender(lastPositions, Pieces, whiteDown);
    }
}

Game::~Game() {
    window.cleanUp();
}

void Game::DragPiece() {

    glm::vec2 newPos = getMousePosition(whiteDown,window.squareSize);

    
    newPos -= 0.5;
    selectedPiece->setPos(newPos);
        
}

void Game::selectPiece() {
    
    glm::ivec2 MousePosition = getMousePosition(whiteDown,window.squareSize);
     
    selectedPiece = getMatchingPiece(MousePosition, Pieces);
    if (selectedPiece != nullptr) {
        selectedPiece->findMoves(Pieces);     
        lastPositions = {selectedPiece->getPos()};
        isPieceSelected = true;
       
    }
}

void Game::placePiece() {

    glm::ivec2 MousePosition = getMousePosition(whiteDown,window.squareSize);

    /* if (!lastPieces.empty()) {
        if (lastPieces[lastPieces.size()-1]->white) {
            whiteTurn = false;
        } else {
            whiteTurn = true;
        }
    } */
    if (selectedPiece->move(MousePosition, lastPositions[0], Pieces, whiteTurn)) {
        if (rotate_board) {
        whiteDown=!whiteDown;
        }
        whiteTurn = !whiteTurn;
        handleCheckmate();
        lastPieces.push_back(selectedPiece);
        lastPositions.push_back(selectedPiece->getPos());
    } 
    //lastPositions.push_back(selectedPiece->getPos());
    isPieceSelected = false;
}
void Game::handleCheckmate() {
        bool checkmate_white = true;
        bool checkmate_black = true;
        for (auto i : Pieces) {
            if (i->white && !i->findMoves(Pieces)) {
                checkmate_white = false;
            }
            if (!i->white && !i->findMoves(Pieces)) {
                checkmate_black = false;
            }
        }
        if (checkmate_black || checkmate_white) {
            std::cout << "checkmate either of them" << std::endl;
            gameRunning = false;
        }
}
//prolly hashmaps of all pieces' moves im too stupid for this
void Game::handleEvents() {
    if (counter < 0) {
        counter = 0;
    }
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT){
                    selectPiece();
                    break;
                }
                break;
            case SDL_QUIT:
                gameRunning = false;
                return;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && selectedPiece != nullptr) {
                    placePiece();
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q: gameRunning = false; break;
                    case SDLK_LEFT: if ( (lastPieces.size()-1) >= counter && (lastPositions.size()-1) >= 2*counter){
                        lastPieces[(lastPieces.size() - 1)  - counter]->setPos(lastPositions[(lastPositions.size() - 1)-(2*counter)-1]); 
                        whiteTurn = !whiteTurn;
                        counter++;
                    }
                        break;
                    case SDLK_RIGHT: if ( (lastPieces.size()-1) >= 1*counter-1 && lastPositions.size() >= 1*counter-1){
                        lastPieces[(lastPieces.size() - 1) - (counter-1)]->setPos(lastPositions[(lastPositions.size() - 1) - (2*(counter-1)) ]);
                        whiteTurn = !whiteTurn; 
                        counter--;
                    } 
                        break;
                }
        }
    }
}


