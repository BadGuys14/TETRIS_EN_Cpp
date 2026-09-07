#include "grille.hpp"

Grille::Grille() {
    reinitialiser();
}

// Remet toutes les cases a 0 (vide) et leur couleur a Transparent.
// Appele au demarrage et quand on recommence une partie.
void Grille::reinitialiser() {
    for (int l = 0; l < LIGNES; ++l) {
        for (int c = 0; c < COLONNES; ++c) {
            m_grille  [l][c] = 0;
            m_couleurs[l][c] = sf::Color::Transparent;
        }
    }
}

// ---------------------------------------------------------------
//  Requetes de collision
// ---------------------------------------------------------------

// Vrai si (ligne, colonne) est dans les bornes et que la case est vide.
// Les valeurs hors bornes (ex. colonne < 0 ou ligne >= LIGNES) retournent
// false car elles representent un mur ou le sol.
bool Grille::caseLibre(int ligne, int colonne) const {
    if (ligne < 0 || ligne >= LIGNES || colonne < 0 || colonne >= COLONNES)
        return false;
    return m_grille[ligne][colonne] == 0;
}

// Verifie que chacun des 4 blocs (x=colonne, y=ligne) est sur une case libre.
// Renvoie false des le premier bloc invalide (court-circuit).
bool Grille::positionValide(const std::array<sf::Vector2i, 4>& positions) const {
    for (const auto& pos : positions) {
        if (!caseLibre(pos.y, pos.x))
            return false;
    }
    return true;
}

// ---------------------------------------------------------------
//  Modification de la grille
// ---------------------------------------------------------------

// Ecrit les 4 blocs de la piece dans les tableaux m_grille et m_couleurs.
// Les blocs hors limites (peut arriver si la piece depasse en haut) sont
// ignores avec la garde if, mais ne devraient pas survenir en jeu normal.
void Grille::fixerPiece(const Piece& piece) {
    for (const auto& pos : piece.positionAbs()) {
        if (pos.y >= 0 && pos.y < LIGNES && pos.x >= 0 && pos.x < COLONNES) {
            m_grille  [pos.y][pos.x] = 1;
            m_couleurs[pos.y][pos.x] = piece.getCouleur();
        }
    }
}

// Parcourt la grille de bas en haut.
// Pour chaque ligne pleine (toutes colonnes == 1) :
//   - on recopie chaque ligne precedente sur celle du dessous (decalage vers le bas)
//   - on vide la ligne 0
//   - on re-examine l'indice courant (ne pas decrementer) car la ligne qui vient
//     de descendre peut elle-meme etre pleine
// Cette boucle est plus simple et robuste qu'une suppression par splice.
int Grille::supprimerLignesCompletes() {
    int nbSupprimees = 0;

    for (int l = LIGNES - 1; l >= 0; ) {
        // Verifie si la ligne l est entierement pleine
        bool pleine = true;
        for (int c = 0; c < COLONNES; ++c) {
            if (m_grille[l][c] == 0) { pleine = false; break; }
        }

        if (pleine) {
            // Decale toutes les lignes au-dessus vers le bas d'un cran
            for (int r = l; r > 0; --r) {
                for (int c = 0; c < COLONNES; ++c) {
                    m_grille  [r][c] = m_grille  [r - 1][c];
                    m_couleurs[r][c] = m_couleurs[r - 1][c];
                }
            }
            // Vide la toute premiere ligne (qui n'avait pas de ligne au-dessus)
            for (int c = 0; c < COLONNES; ++c) {
                m_grille  [0][c] = 0;
                m_couleurs[0][c] = sf::Color::Transparent;
            }
            ++nbSupprimees;
            // Ne pas decrementer l : la ligne qui vient de descendre doit etre re-verifiee
        } else {
            --l;
        }
    }

    return nbSupprimees;
}

// ---------------------------------------------------------------
//  Affichage
// ---------------------------------------------------------------

// Dessine le plateau case par case.
// Cases vides : couleur COULEUR_GRILLE (fond discret).
// Cases occupees : couleur stockee dans m_couleurs (couleur elementaire de la piece fixee).
void Grille::dessiner(sf::RenderWindow& fenetre) {
    sf::RectangleShape caseGrille(sf::Vector2f({
        static_cast<float>(TAILLE_CASE - 1),
        static_cast<float>(TAILLE_CASE - 1)
    }));

    for (int l = 0; l < LIGNES; ++l) {
        for (int c = 0; c < COLONNES; ++c) {
            caseGrille.setPosition({
                static_cast<float>(c * TAILLE_CASE),
                static_cast<float>(l * TAILLE_CASE)
            });

            if (m_grille[l][c] == 0) {
                caseGrille.setFillColor(COULEUR_GRILLE); // fond de grille vide
            } else {
                caseGrille.setFillColor(m_couleurs[l][c]); // couleur elementaire de la piece fixee
            }

            fenetre.draw(caseGrille);
        }
    }
}