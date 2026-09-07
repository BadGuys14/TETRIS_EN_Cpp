#include "jeu.hpp"
#include <iostream>
#include <algorithm>

Jeu::Jeu()
    : m_fenetre(
        sf::VideoMode({
            static_cast<unsigned int>(LARGEUR_FENETRE),
            static_cast<unsigned int>(LONGUEUR_FENETRE)
        }),
        "Tetris en C++",
        sf::Style::Default
    ),
    m_police(),
    m_menu(),
    m_grille(),
    m_etat(EtatJeu::Menu),
    m_generator(std::random_device{}()),
    m_pieceSuivante(FormePiece::I)
{
    // 1. Charger la police en premier
    if (!m_police.openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        if (!m_police.openFromFile("assets/fonts/PixelifySans-VariableFont_wght.ttf")) {
            std::cerr << "Erreur : Impossible de charger la police !" << std::endl;
        }
    }

    // 2. Initialiser le Menu APRES le chargement de la police
    m_menu.init(static_cast<float>(LARGEUR_FENETRE), static_cast<float>(LONGUEUR_FENETRE), m_police);
}

void Jeu::executer() {
    while (m_fenetre.isOpen()) {
        gesEvenements();
        miseAJour();
        affichage();
    }
}

// ---------------------------------------------------------------
//  Gestion des tirages aléatoires (Technique du 7-Bag)
// ---------------------------------------------------------------

// Remplissage du sac avec exactement une fois chacune des 7 formes Tetris (I, O, T, L, J, S, Z).
// Les formes sont ensuite mélangées de manière aléatoire grâce à std::shuffle et std::mt19937.
void Jeu::remplirSac() {
    m_sac = {
        FormePiece::I, FormePiece::O, FormePiece::T,
        FormePiece::L, FormePiece::J, FormePiece::S, FormePiece::Z
    };
    std::shuffle(m_sac.begin(), m_sac.end(), m_generator);
}

// Extrait et renvoie la forme au sommet du sac.
// Si le sac est vide, il est automatiquement re-rempli et re-mélangé avant d'extraire la forme.
FormePiece Jeu::piocherForme() {
    if (m_sac.empty()) {
        remplirSac();
    }
    FormePiece forme = m_sac.back();
    m_sac.pop_back();
    return forme;
}

// ---------------------------------------------------------------
//  Initialisation d'une nouvelle partie
// ---------------------------------------------------------------

// Prépare le plateau de jeu et la réserve de pièces pour un nouveau départ :
// réinitialise la grille, vide le sac, génère la pièce courante ainsi que la pièce suivante,
// puis réinitialise l'horloge de chute. Si la pièce de départ est déjà bloquée (game over instantané),
// le jeu retourne au menu principal.
void Jeu::demarrerNouvellePartie() {
    m_grille.reinitialiser();
    m_sac.clear();
    remplirSac();

    m_pieceCourante = Piece(piocherForme());
    m_pieceSuivante = Piece(piocherForme());
    m_horlogeChute.restart();

    // Vérification de sécurité lors de la première apparition
    if (!m_grille.positionValide(m_pieceCourante->positionAbs())) {
        m_etat = EtatJeu::Menu;
        m_pieceCourante.reset();
    }
}

// ---------------------------------------------------------------
//  Déplacements et Collisions de la pièce courante
// ---------------------------------------------------------------

// Tente de déplacer la pièce courante d'un décalage relatif (dl lignes, dc colonnes).
// Effectue une copie temporaire pour tester si la nouvelle position est valide dans la grille.
// Renvoie true si le déplacement est validé et appliqué, false sinon.
bool Jeu::essayerDeplacerPiece(int dl, int dc) {
    if (!m_pieceCourante.has_value()) return false;

    Piece testPiece = *m_pieceCourante;
    testPiece.deplacer(dl, dc);

    if (m_grille.positionValide(testPiece.positionAbs())) {
        m_pieceCourante->deplacer(dl, dc);
        return true;
    }
    return false;
}

// ---------------------------------------------------------------
//  Verrouillage de la pièce et passage à la suivante
// ---------------------------------------------------------------

// Appelée lorsqu'une pièce ne peut plus descendre.
// Fixe la pièce courante dans les tableaux de la grille, détruit les éventuelles lignes pleines,
// promeut la pièce suivante au statut de pièce courante, puis pioche une nouvelle pièce suivante.
// Si la nouvelle pièce ne peut pas être placée (grille pleine en haut), réinitialise le jeu et revient au menu.
void Jeu::verrouillerPiece() {
    if (!m_pieceCourante.has_value()) return;

    // 1. Inscrire les 4 blocs dans la grille avec leur couleur respectives
    m_grille.fixerPiece(*m_pieceCourante);

    // 2. Vérifier et supprimer les lignes pleines
    m_grille.supprimerLignesCompletes();

    // 3. Passer la pièce suivante en pièce courante et piocher la nouvelle pièce suivante
    m_pieceCourante = m_pieceSuivante;
    m_pieceSuivante = Piece(piocherForme());

    // 4. Test d'apparition de la nouvelle pièce courante
    if (!m_grille.positionValide(m_pieceCourante->positionAbs())) {
        // La grille est saturée en haut : fin de partie, retour au menu
        m_etat = EtatJeu::Menu;
        m_grille.reinitialiser();
        m_pieceCourante.reset();
    }

    // Réinitialisation du timer de chute pour la nouvelle pièce
    m_horlogeChute.restart();
}

// ---------------------------------------------------------------
//  Gestion des Événements Entrées (Clavier / Souris)
// ---------------------------------------------------------------

void Jeu::gesEvenements() {
    while (const std::optional event = m_fenetre.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_fenetre.close();
        }

        // Touche Echap : bascule entre EnJeu et Pause
        if (const auto* e = event->getIf<sf::Event::KeyPressed>()) {
            if (e->code == sf::Keyboard::Key::Escape) {
                if (m_etat == EtatJeu::EnJeu) {
                    m_etat = EtatJeu::Pause;
                } else if (m_etat == EtatJeu::Pause) {
                    m_etat = EtatJeu::EnJeu;
                    m_horlogeChute.restart(); // Évite une chute subite à la sortie de pause
                }
            }
        }

        if (m_etat == EtatJeu::Menu) {
            if (const auto* e = event->getIf<sf::Event::KeyPressed>())
                m_menu.gererTouche(e->code);

            if (const auto* e = event->getIf<sf::Event::MouseMoved>())
                m_menu.gererSouris({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseButtonPressed>())
                if (e->button == sf::Mouse::Button::Left)
                    m_menu.gererClic({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseWheelScrolled>())
                m_menu.gererMolette(e->delta);
        }
        else if (m_etat == EtatJeu::EnJeu) {
            // Controles de déplacement de la pièce en cours de jeu
            if (const auto* e = event->getIf<sf::Event::KeyPressed>()) {
                if (e->code == sf::Keyboard::Key::Left) {
                    essayerDeplacerPiece(0, -1); // Déplacement gauche
                }
                else if (e->code == sf::Keyboard::Key::Right) {
                    essayerDeplacerPiece(0, 1); // Déplacement droite
                }
                else if (e->code == sf::Keyboard::Key::Down) {
                    // Descente rapide manuelle : si impossible de descendre, la pièce est verrouillée
                    if (!essayerDeplacerPiece(1, 0)) {
                        verrouillerPiece();
                    } else {
                        m_horlogeChute.restart(); // Réinitialise l'horloge de chute auto
                    }
                }
            }
        }
        else if (m_etat == EtatJeu::Pause) {
            if (const auto* e = event->getIf<sf::Event::KeyPressed>())
                m_menu.gererTouchePause(e->code);

            if (const auto* e = event->getIf<sf::Event::MouseMoved>())
                m_menu.gererSourisPause({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseButtonPressed>())
                if (e->button == sf::Mouse::Button::Left)
                    m_menu.gererClicPause({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseWheelScrolled>())
                m_menu.gererMolettePause(e->delta);
        }
    }
}

// ---------------------------------------------------------------
//  Mise à Jour (Logique du Jeu et Chute Automatique)
// ---------------------------------------------------------------

void Jeu::miseAJour() {
    if (m_etat == EtatJeu::Menu) {
        if (m_menu.demandeLancementJeu()) {
            m_etat = EtatJeu::EnJeu;
            m_menu.resetLancementJeu();
            demarrerNouvellePartie();
        }
    }
    else if (m_etat == EtatJeu::EnJeu) {
        // Chute automatique de la pièce courante toutes les 0.5s
        if (m_pieceCourante.has_value()) {
            if (m_horlogeChute.getElapsedTime().asSeconds() >= m_delaiChute) {
                m_horlogeChute.restart();
                // Si la pièce ne peut plus descendre, on la fixe et on passe à la suivante
                if (!essayerDeplacerPiece(1, 0)) {
                    verrouillerPiece();
                }
            }
        }
    }
    else if (m_etat == EtatJeu::Pause) {
        // Actions demandées depuis le menu pause
        if (m_menu.demandeReprise()) {
            m_etat = EtatJeu::EnJeu;
            m_menu.resetReprise();
            m_horlogeChute.restart();
        }
        else if (m_menu.demandeRecommencer()) {
            m_etat = EtatJeu::EnJeu;
            m_menu.resetRecommencer();
            demarrerNouvellePartie();
        }
        else if (m_menu.demandeQuitter()) {
            m_etat = EtatJeu::Menu;
            m_menu.resetQuitter();
            m_grille.reinitialiser();
            m_pieceCourante.reset();
        }
    }
}

// ---------------------------------------------------------------
//  Rendu Graphique
// ---------------------------------------------------------------

void Jeu::affichage() {
    m_fenetre.clear(COULEUR_FENETRE);

    if (m_etat == EtatJeu::Menu) {
        m_menu.afficher(m_fenetre);
    }
    else if (m_etat == EtatJeu::EnJeu) {
        m_grille.dessiner(m_fenetre);
        if (m_pieceCourante.has_value()) {
            m_pieceCourante->dessiner(m_fenetre, m_grille.getTailleCase());
        }
    }
    else if (m_etat == EtatJeu::Pause) {
        m_grille.dessiner(m_fenetre);      // Grille en arrière-plan
        if (m_pieceCourante.has_value()) {
            m_pieceCourante->dessiner(m_fenetre, m_grille.getTailleCase());
        }
        m_menu.afficherPause(m_fenetre);   // Overlay pause par-dessus
    }

    m_fenetre.display();
}
