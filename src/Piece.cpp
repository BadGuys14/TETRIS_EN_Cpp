#include "Piece.hpp"

Piece::Piece(FormePiece forme, int colonneDepart, int ligneDepart)
    : m_forme(forme), 
      m_couleur(CouleurselonForme(forme)),
      m_ligne(ligneDepart),
      m_colonne(colonneDepart)
      {
        //Pk c'est vide ? tu remplaces l'expliquation 
      }

      //Chaque forme est fixee avec une couleur correspond a un element de genshin impact
      sf::Color Piece::CouleurselonForme(FormePiece forme){
        switch (forme) {
            case FormePiece::I: return Palette::Anemo;
            case FormePiece::O: return Palette::Geo;
            case FormePiece::T: return Palette::Electro;
            case FormePiece::L: return Palette::Pyro;
            case FormePiece::J: return Palette::Hydro;
            case FormePiece::S: return Palette::Dendro;
            case FormePiece::Z: return Palette::Cryo;
        }
        return sf::Color::White; //securite mais ne devrais pas arriver
        
      }

      //Definit la forme de chaque piece via ses 4 blocs dans un repere local 4*4
      FormeCoords Piece::calculerCoords() const {
        switch(m_forme){
            case FormePiece::I:
                return{sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{3,1}};
            case FormePiece::O:
                return{sf::Vector2i{1,0}, sf::Vector2i{2,0}, sf::Vector2i{1,1}, sf::Vector2i{2,1}};
            case FormePiece::T:
                return{sf::Vector2i{0,1},sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{1,0}};
            case FormePiece::L: 
                return{sf::Vector2i{0,1},sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{2,0}};
            case FormePiece::J: 
                return{sf::Vector2i{0,1},sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{0,0}};
            case FormePiece::S:
                return{sf::Vector2i{1,0},sf::Vector2i{2,0}, sf::Vector2i{0,1}, sf::Vector2i{1,1}};
            case FormePiece::Z:
                return{sf::Vector2i{0,0},sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{2,1}};
      }
      return {sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{3,1}}; // securite
    }

    void Piece::deplacer(int dl, int dc){
        m_ligne += dl; //Expliquation
        m_colonne += dc;//Expliquation
    }

    std::array<sf::Vector2i,4> Piece::positionAbs() const {
        FormeCoords coords = calculerCoords();
        std::array<sf::Vector2i, 4> positions; 

        for (int i = 0; i < 4; i++){
            positions[i] = sf::Vector2i(m_colonne + coords[i].x, m_ligne + coords[i].y); // position absolue dans la grille
        }
        return positions;
    }

    void Piece::dessiner(sf::RenderWindow& fenetre, int tailleCase) const {
        sf::RectangleShape bloc(sf::Vector2f({
            static_cast <float>(tailleCase - 1),//Expliquation
            static_cast <float>(tailleCase - 1)//Expliquation
        }));
        bloc.setFillColor(m_couleur); //Expliquation

        for (const auto& pos : positionAbs()){
            bloc.setPosition({
                static_cast <float>(pos.x * tailleCase),
                static_cast <float>(pos.y * tailleCase)
            });
            fenetre.draw(bloc);
        }
    }
