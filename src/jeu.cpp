#include "jeu.hpp"
#include <iostream>
#include <fstream>
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

    // 3. Charger le meilleur score sauvegardé
    chargerMeilleurScore();
}

void Jeu::executer() {
    while (m_fenetre.isOpen()) {
        gesEvenements();
        miseAJour();
        affichage();
    }
}

// ---------------------------------------------------------------
//  Sauvegarde et Chargement du Meilleur Score
// ---------------------------------------------------------------
void Jeu::chargerMeilleurScore() {
    std::ifstream fichier("assets/bestscore.txt");
    if (fichier.is_open()) {
        fichier >> m_meilleurScore;
        fichier.close();
    } else {
        m_meilleurScore = 0;
    }
}

void Jeu::sauvegarderMeilleurScore() {
    std::ofstream fichier("assets/bestscore.txt");
    if (fichier.is_open()) {
        fichier << m_meilleurScore;
        fichier.close();
    }
}

// ---------------------------------------------------------------
//  Gestion des tirages aléatoires (Technique du 7-Bag)
// ---------------------------------------------------------------
void Jeu::remplirSac() {
    m_sac = {
        FormePiece::I, FormePiece::O, FormePiece::T,
        FormePiece::L, FormePiece::J, FormePiece::S, FormePiece::Z
    };
    std::shuffle(m_sac.begin(), m_sac.end(), m_generator);
}

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
void Jeu::demarrerNouvellePartie() {
    m_grille.reinitialiser();
    m_sac.clear();
    remplirSac();

    m_score = 0;
    m_combo = 0;
    m_lignesTotales = 0;

    m_pieceCourante = Piece(piocherForme());
    m_pieceSuivante = Piece(piocherForme());
    m_horlogeChute.restart();

    if (!m_grille.positionValide(m_pieceCourante->positionAbs())) {
        m_etat = EtatJeu::GameOver;
        m_menu.ouvrirGameOver();
    }
}

// ---------------------------------------------------------------
//  Déplacements, Rotation et Hard Drop
// ---------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// EXPLICATION POUR LE PROFESSEUR :
// Tente de faire pivoter la pièce courante.
// Si la rotation directe frappe un obstacle ou un mur, on effectue des "wall kicks"
// (tests de décalage de -1, +1, -2, +2 cases ou +1 ligne vers le haut) pour ajuster la position.
// -----------------------------------------------------------------------------
void Jeu::tournerPieceCourante() {
    if (!m_pieceCourante.has_value()) return;

    Piece testPiece = *m_pieceCourante;
    testPiece.tournerHoraire();

    if (m_grille.positionValide(testPiece.positionAbs())) {
        *m_pieceCourante = testPiece;
        return;
    }

    Piece testLeft = testPiece;
    testLeft.deplacer(0, -1);
    if (m_grille.positionValide(testLeft.positionAbs())) {
        *m_pieceCourante = testLeft;
        return;
    }

    Piece testRight = testPiece;
    testRight.deplacer(0, 1);
    if (m_grille.positionValide(testRight.positionAbs())) {
        *m_pieceCourante = testRight;
        return;
    }

    Piece testLeft2 = testPiece;
    testLeft2.deplacer(0, -2);
    if (m_grille.positionValide(testLeft2.positionAbs())) {
        *m_pieceCourante = testLeft2;
        return;
    }

    Piece testRight2 = testPiece;
    testRight2.deplacer(0, 2);
    if (m_grille.positionValide(testRight2.positionAbs())) {
        *m_pieceCourante = testRight2;
        return;
    }

    Piece testUp = testPiece;
    testUp.deplacer(-1, 0);
    if (m_grille.positionValide(testUp.positionAbs())) {
        *m_pieceCourante = testUp;
        return;
    }
}

// Projection vers le bas pour calculer la Ghost Piece (ombre de la pièce)
Piece Jeu::calculerGhostPiece() const {
    Piece ghost = *m_pieceCourante;
    while (m_grille.positionValide(ghost.positionAbs())) {
        ghost.deplacer(1, 0);
    }
    ghost.deplacer(-1, 0);
    return ghost;
}

// Chute instantanée (Hard Drop avec la touche Espace)
void Jeu::hardDropPiece() {
    if (!m_pieceCourante.has_value()) return;

    int bonusChute = 0;
    while (essayerDeplacerPiece(1, 0)) {
        bonusChute += 2;
    }
    m_score += bonusChute;
    if (m_score > m_meilleurScore) {
        m_meilleurScore = m_score;
        sauvegarderMeilleurScore();
    }
    verrouillerPiece();
}

// ---------------------------------------------------------------
//  Verrouillage et Calcul du Score / Combos
// ---------------------------------------------------------------
void Jeu::verrouillerPiece() {
    if (!m_pieceCourante.has_value()) return;

    // 1. Inscrire les 4 blocs dans la grille
    m_grille.fixerPiece(*m_pieceCourante);

    // 2. Supprimer les lignes pleines et calculer les points
    int nbLignes = m_grille.supprimerLignesCompletes();
    if (nbLignes > 0) {
        m_combo++;
        int pointsDeBase = 0;
        if (nbLignes == 1) pointsDeBase = 100;
        else if (nbLignes == 2) pointsDeBase = 300;
        else if (nbLignes == 3) pointsDeBase = 500;
        else if (nbLignes >= 4) pointsDeBase = 800; // Tetris !

        int bonusCombo = (m_combo > 1) ? (m_combo - 1) * 50 : 0;
        m_score += pointsDeBase + bonusCombo;
        m_lignesTotales += nbLignes;

        if (m_score > m_meilleurScore) {
            m_meilleurScore = m_score;
            sauvegarderMeilleurScore();
        }
    } else {
        m_combo = 0;
    }

    // 3. Passer à la pièce suivante
    m_pieceCourante = m_pieceSuivante;
    m_pieceSuivante = Piece(piocherForme());

    // 4. Test d'apparition (Défaite / Game Over)
    if (!m_grille.positionValide(m_pieceCourante->positionAbs())) {
        sauvegarderMeilleurScore();
        m_etat = EtatJeu::GameOver;
        m_menu.ouvrirGameOver();
    }

    m_horlogeChute.restart();
}

// ---------------------------------------------------------------
//  Gestion des Événements
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
                    m_menu.ouvrirPause();
                } else if (m_etat == EtatJeu::Pause) {
                    m_etat = EtatJeu::EnJeu;
                    m_horlogeChute.restart();
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
            if (const auto* e = event->getIf<sf::Event::KeyPressed>()) {
                if (e->code == sf::Keyboard::Key::Left) {
                    essayerDeplacerPiece(0, -1);
                }
                else if (e->code == sf::Keyboard::Key::Right) {
                    essayerDeplacerPiece(0, 1);
                }
                else if (e->code == sf::Keyboard::Key::Up || e->code == sf::Keyboard::Key::Z) {
                    tournerPieceCourante();
                }
                else if (e->code == sf::Keyboard::Key::Down) {
                    if (!essayerDeplacerPiece(1, 0)) {
                        verrouillerPiece();
                    } else {
                        m_horlogeChute.restart();
                    }
                }
                else if (e->code == sf::Keyboard::Key::Space) {
                    hardDropPiece();
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
        else if (m_etat == EtatJeu::GameOver) {
            if (const auto* e = event->getIf<sf::Event::KeyPressed>())
                m_menu.gererToucheGameOver(e->code);

            if (const auto* e = event->getIf<sf::Event::MouseMoved>())
                m_menu.gererSourisGameOver({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseButtonPressed>())
                if (e->button == sf::Mouse::Button::Left)
                    m_menu.gererClicGameOver({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseWheelScrolled>())
                m_menu.gererMoletteGameOver(e->delta);
        }
    }
}

// ---------------------------------------------------------------
//  Mise à Jour (Logique du Jeu)
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
        if (m_pieceCourante.has_value()) {
            if (m_horlogeChute.getElapsedTime().asSeconds() >= m_delaiChute) {
                m_horlogeChute.restart();
                if (!essayerDeplacerPiece(1, 0)) {
                    verrouillerPiece();
                }
            }
        }
    }
    else if (m_etat == EtatJeu::Pause) {
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
    else if (m_etat == EtatJeu::GameOver) {
        if (m_menu.demandeRecommencerGameOver()) {
            m_menu.resetRecommencerGameOver();
            m_etat = EtatJeu::EnJeu;
            demarrerNouvellePartie();
        }
        else if (m_menu.demandeMenuGameOver()) {
            m_menu.resetMenuGameOver();
            m_etat = EtatJeu::Menu;
            m_grille.reinitialiser();
            m_pieceCourante.reset();
        }
    }
}

// -----------------------------------------------------------------------------
// EXPLICATION POUR LE PROFESSEUR :
// Dessine le panneau latéral droit (x: 280 à 800).
// -----------------------------------------------------------------------------
void Jeu::dessinerPanneauLateral() {
    sf::RectangleShape fondSidebar(sf::Vector2f({520.f, 600.f}));
    fondSidebar.setPosition({280.f, 0.f});
    fondSidebar.setFillColor(sf::Color(245, 246, 250));
    m_fenetre.draw(fondSidebar);

    sf::RectangleShape ligneSeparation(sf::Vector2f({2.f, 600.f}));
    ligneSeparation.setPosition({280.f, 0.f});
    ligneSeparation.setFillColor(sf::Color(210, 215, 225));
    m_fenetre.draw(ligneSeparation);

    sf::Text txt(m_police);

    // --- 1. BEST SCORE ---
    txt.setString("BEST SCORE");
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color(120, 130, 145));
    sf::FloatRect b1 = txt.getLocalBounds();
    txt.setOrigin({b1.position.x + b1.size.x / 2.f, b1.position.y + b1.size.y / 2.f});
    txt.setPosition({540.f, 35.f});
    m_fenetre.draw(txt);

    txt.setString(std::to_string(m_meilleurScore));
    txt.setCharacterSize(22);
    txt.setFillColor(Palette::Dendro);
    sf::FloatRect b2 = txt.getLocalBounds();
    txt.setOrigin({b2.position.x + b2.size.x / 2.f, b2.position.y + b2.size.y / 2.f});
    txt.setPosition({540.f, 65.f});
    m_fenetre.draw(txt);

    sf::RectangleShape div1(sf::Vector2f({240.f, 1.f}));
    div1.setPosition({420.f, 95.f});
    div1.setFillColor(sf::Color(220, 225, 235));
    m_fenetre.draw(div1);

    // --- 2. CURRENT SCORE ---
    txt.setString("CURRENT SCORE");
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color(120, 130, 145));
    sf::FloatRect b3 = txt.getLocalBounds();
    txt.setOrigin({b3.position.x + b3.size.x / 2.f, b3.position.y + b3.size.y / 2.f});
    txt.setPosition({540.f, 120.f});
    m_fenetre.draw(txt);

    txt.setString(std::to_string(m_score));
    txt.setCharacterSize(24);
    txt.setFillColor(sf::Color(40, 45, 55));
    sf::FloatRect b4 = txt.getLocalBounds();
    txt.setOrigin({b4.position.x + b4.size.x / 2.f, b4.position.y + b4.size.y / 2.f});
    txt.setPosition({540.f, 150.f});
    m_fenetre.draw(txt);

    if (m_combo > 1) {
        txt.setString("COMBO x" + std::to_string(m_combo) + "!");
        txt.setCharacterSize(14);
        txt.setFillColor(Palette::Geo);
        sf::FloatRect bc = txt.getLocalBounds();
        txt.setOrigin({bc.position.x + bc.size.x / 2.f, bc.position.y + bc.size.y / 2.f});
        txt.setPosition({540.f, 180.f});
        m_fenetre.draw(txt);
    }

    // --- 3. NEXT PIECE ---
    txt.setString("NEXT PIECE");
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color(120, 130, 145));
    sf::FloatRect b5 = txt.getLocalBounds();
    txt.setOrigin({b5.position.x + b5.size.x / 2.f, b5.position.y + b5.size.y / 2.f});
    txt.setPosition({540.f, 215.f});
    m_fenetre.draw(txt);

    sf::RectangleShape boxNext(sf::Vector2f({160.f, 130.f}));
    boxNext.setPosition({460.f, 235.f});
    boxNext.setFillColor(sf::Color::White);
    boxNext.setOutlineThickness(2.f);
    boxNext.setOutlineColor(sf::Color(220, 224, 232));
    m_fenetre.draw(boxNext);

    float previewX = 490.f;
    float previewY = 275.f;
    if (m_pieceSuivante.getForme() == FormePiece::I) {
        previewX = 468.f;
        previewY = 275.f;
    } else if (m_pieceSuivante.getForme() == FormePiece::O) {
        previewX = 502.f;
        previewY = 270.f;
    }
    m_pieceSuivante.dessinerAPosition(m_fenetre, previewX, previewY, 24);

    // --- 4. CONTROLS ---
    txt.setString("CONTROLS");
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color(120, 130, 145));
    sf::FloatRect b6 = txt.getLocalBounds();
    txt.setOrigin({b6.position.x + b6.size.x / 2.f, b6.position.y + b6.size.y / 2.f});
    txt.setPosition({540.f, 405.f});
    m_fenetre.draw(txt);

    const std::vector<std::string> commandes = {
        "< >  Move",
        "^ / Z Rotate",
        "v    Soft Drop",
        "Spc  Hard Drop",
        "Esc  Pause"
    };

    float ctrlY = 430.f;
    for (const auto& cmd : commandes) {
        txt.setString(cmd);
        txt.setCharacterSize(11);
        txt.setFillColor(sf::Color(130, 140, 155));
        sf::FloatRect bc = txt.getLocalBounds();
        txt.setOrigin({bc.position.x + bc.size.x / 2.f, bc.position.y + bc.size.y / 2.f});
        txt.setPosition({540.f, ctrlY});
        m_fenetre.draw(txt);
        ctrlY += 20.f;
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
            Piece ghost = calculerGhostPiece();
            ghost.dessinerFantome(m_fenetre, m_grille.getTailleCase());
            m_pieceCourante->dessiner(m_fenetre, m_grille.getTailleCase());
        }

        dessinerPanneauLateral();
    }
    else if (m_etat == EtatJeu::Pause) {
        m_grille.dessiner(m_fenetre);

        if (m_pieceCourante.has_value()) {
            Piece ghost = calculerGhostPiece();
            ghost.dessinerFantome(m_fenetre, m_grille.getTailleCase());
            m_pieceCourante->dessiner(m_fenetre, m_grille.getTailleCase());
        }

        dessinerPanneauLateral();
        m_menu.afficherPause(m_fenetre);
    }
    else if (m_etat == EtatJeu::GameOver) {
        m_grille.dessiner(m_fenetre);
        dessinerPanneauLateral();
        m_menu.afficherGameOver(m_fenetre);
    }

    m_fenetre.display();
}
