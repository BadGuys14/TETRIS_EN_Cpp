#include "Piece.hpp"

// Constructeur de la pièce
Piece::Piece(FormePiece forme, int colonneDepart, int ligneDepart)
    : m_forme(forme),
      m_couleur(CouleurselonForme(forme)),
      m_ligne(ligneDepart),
      m_colonne(colonneDepart),
      m_orientation(0)
{
}

// Chaque forme est associée à une couleur d'élément (thème Genshin Impact)
sf::Color Piece::CouleurselonForme(FormePiece forme) {
    switch (forme) {
        case FormePiece::I: return Palette::Anemo;   // Turquoise
        case FormePiece::O: return Palette::Geo;     // Or / Jaune
        case FormePiece::T: return Palette::Electro; // Violet
        case FormePiece::L: return Palette::Pyro;    // Rouge / Orange
        case FormePiece::J: return Palette::Hydro;   // Bleu
        case FormePiece::S: return Palette::Dendro;  // Vert
        case FormePiece::Z: return Palette::Cryo;    // Bleu Givre
    }
    return sf::Color::White;
}

// -----------------------------------------------------------------------------
// EXPLICATION POUR LE PROFESSEUR :
// Renvoie les 4 coordonnées relatives (x, y) des blocs selon la forme ET l'orientation.
// L'orientation est une valeur de 0 à 3 (0°, 90°, 180°, 270°).
// Chaque forme pivote autour de son centre local dans une grille 4x4.
// -----------------------------------------------------------------------------
FormeCoords Piece::calculerCoords() const {
    switch (m_forme) {
        case FormePiece::O:
            // Le carré O ne change pas selon l'orientation
            return { sf::Vector2i{1,0}, sf::Vector2i{2,0}, sf::Vector2i{1,1}, sf::Vector2i{2,1} };

        case FormePiece::I:
            switch (m_orientation) {
                case 0: return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{3,1} };
                case 1: return { sf::Vector2i{2,0}, sf::Vector2i{2,1}, sf::Vector2i{2,2}, sf::Vector2i{2,3} };
                case 2: return { sf::Vector2i{0,2}, sf::Vector2i{1,2}, sf::Vector2i{2,2}, sf::Vector2i{3,2} };
                case 3: return { sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{1,2}, sf::Vector2i{1,3} };
            }
            break;

        case FormePiece::T:
            switch (m_orientation) {
                case 0: return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{1,0} }; // Haut
                case 1: return { sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{1,2}, sf::Vector2i{2,1} }; // Droite
                case 2: return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{1,2} }; // Bas
                case 3: return { sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{1,2}, sf::Vector2i{0,1} }; // Gauche
            }
            break;

        case FormePiece::L:
            switch (m_orientation) {
                case 0: return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{2,0} };
                case 1: return { sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{1,2}, sf::Vector2i{2,2} };
                case 2: return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{0,2} };
                case 3: return { sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{1,2}, sf::Vector2i{0,0} };
            }
            break;

        case FormePiece::J:
            switch (m_orientation) {
                case 0: return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{0,0} };
                case 1: return { sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{1,2}, sf::Vector2i{2,0} };
                case 2: return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{2,2} };
                case 3: return { sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{1,2}, sf::Vector2i{0,2} };
            }
            break;

        case FormePiece::S:
            switch (m_orientation) {
                case 0: return { sf::Vector2i{1,0}, sf::Vector2i{2,0}, sf::Vector2i{0,1}, sf::Vector2i{1,1} };
                case 1: return { sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{2,2} };
                case 2: return { sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{0,2}, sf::Vector2i{1,2} };
                case 3: return { sf::Vector2i{0,0}, sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{1,2} };
            }
            break;

        case FormePiece::Z:
            switch (m_orientation) {
                case 0: return { sf::Vector2i{0,0}, sf::Vector2i{1,0}, sf::Vector2i{1,1}, sf::Vector2i{2,1} };
                case 1: return { sf::Vector2i{2,0}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{1,2} };
                case 2: return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{1,2}, sf::Vector2i{2,2} };
                case 3: return { sf::Vector2i{1,0}, sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{0,2} };
            }
            break;
    }
    return { sf::Vector2i{0,1}, sf::Vector2i{1,1}, sf::Vector2i{2,1}, sf::Vector2i{3,1} };
}

// Effectue une rotation horaire (+90°)
void Piece::tournerHoraire() {
    m_orientation = (m_orientation + 1) % 4;
}

// Effectue une rotation anti-horaire (-90°)
void Piece::tournerAntiHoraire() {
    m_orientation = (m_orientation + 3) % 4;
}

// Déplace la pièce dans le repère global de la grille
void Piece::deplacer(int dl, int dc) {
    m_ligne   += dl;
    m_colonne += dc;
}

// Calcule les positions absolues des 4 blocs dans la grille
std::array<sf::Vector2i, 4> Piece::positionAbs() const {
    FormeCoords coords = calculerCoords();
    std::array<sf::Vector2i, 4> positions;
    for (int i = 0; i < 4; i++) {
        positions[i] = sf::Vector2i(m_colonne + coords[i].x, m_ligne + coords[i].y);
    }
    return positions;
}

// Dessine la pièce réelle dans la fenêtre de jeu
void Piece::dessiner(sf::RenderWindow& fenetre, int tailleCase) const {
    sf::RectangleShape bloc(sf::Vector2f({
        static_cast<float>(tailleCase - 1),
        static_cast<float>(tailleCase - 1)
    }));
    bloc.setFillColor(m_couleur);

    for (const auto& pos : positionAbs()) {
        bloc.setPosition({
            static_cast<float>(pos.x * tailleCase),
            static_cast<float>(pos.y * tailleCase)
        });
        fenetre.draw(bloc);
    }
}

// -----------------------------------------------------------------------------
// EXPLICATION POUR LE PROFESSEUR :
// Dessine la "Ghost Piece" (pièce fantôme) qui projette l'ombre d'atterrissage.
// On utilise une couleur semi-transparente (alpha = 50) avec une bordure fine
// de la couleur officielle de la pièce pour un effet visuel moderne.
// -----------------------------------------------------------------------------
void Piece::dessinerFantome(sf::RenderWindow& fenetre, int tailleCase) const {
    sf::RectangleShape bloc(sf::Vector2f({
        static_cast<float>(tailleCase - 1),
        static_cast<float>(tailleCase - 1)
    }));

    sf::Color couleurFond = m_couleur;
    couleurFond.a = 50; // Semi-transparent (opacité 50/255)

    sf::Color couleurBordure = m_couleur;
    couleurBordure.a = 160;

    bloc.setFillColor(couleurFond);
    bloc.setOutlineThickness(-1.f);
    bloc.setOutlineColor(couleurBordure);

    for (const auto& pos : positionAbs()) {
        bloc.setPosition({
            static_cast<float>(pos.x * tailleCase),
            static_cast<float>(pos.y * tailleCase)
        });
        fenetre.draw(bloc);
    }
}

// -----------------------------------------------------------------------------
// Dessine la pièce dans une zone d'aperçu fixe (ex: encadré NEXT PIECE).
// -----------------------------------------------------------------------------
void Piece::dessinerAPosition(sf::RenderWindow& fenetre, float xOrigin, float yOrigin, int tailleCase) const {
    sf::RectangleShape bloc(sf::Vector2f({
        static_cast<float>(tailleCase - 1),
        static_cast<float>(tailleCase - 1)
    }));
    bloc.setFillColor(m_couleur);

    FormeCoords coords = calculerCoords();
    for (const auto& c : coords) {
        bloc.setPosition({
            xOrigin + static_cast<float>(c.x * tailleCase),
            yOrigin + static_cast<float>(c.y * tailleCase)
        });
        fenetre.draw(bloc);
    }
}
