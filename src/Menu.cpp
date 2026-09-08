#include "Menu.h"
#include <cstdlib>

using namespace std;

Menu::Menu()
    : m_police(nullptr)
{}

Menu::Menu(float larg, float haut, const sf::Font& police)
{
    init(larg, haut, police);
}

void Menu::init(float larg, float haut, const sf::Font& police) {
    m_police = &police;
    m_larg   = larg;
    m_haut   = haut;
    m_lettreTitre.clear();
    m_btnsAccueil.clear();
    m_btnsPause.clear();
    m_btnsGameOver.clear();
    m_notesDeco.clear();
    m_lignesDeco.clear();
    m_blocsDeco.clear();

    initEcranTitre(larg, haut);
    initAccueil(larg, haut);
    initPause(larg, haut);
    initGameOver(larg, haut);
    majStyle();
}

// -----------------------------------------------------------------------------
// EXPLICATION POUR LE PROFESSEUR :
// Initialise l'écran titre (Splash Screen) inspiré de la maquette du projet :
// 1. Logo "TETRIS" avec chaque lettre colorée selon les 7 éléments Genshin Impact.
// 2. Sous-titre des auteurs "Emmanuel & Martinaud".
// 3. Éléments décoratifs : notes de musique (♪ ♫), portées de 5 lignes et mini-pièces Tetris.
// 4. Utilisation de getLocalBounds() et setOrigin() pour centrer précisément chaque texte.
// -----------------------------------------------------------------------------
void Menu::initEcranTitre(float larg, float haut) {
    if (!m_police) return;

    // --- 1. Titre multicolore TETRIS ---
    vector<sf::Color> couleurs = {
        Palette::Pyro,    // T - Rouge/Orange
        Palette::Hydro,   // E - Bleu Océan
        Palette::Pyro,    // T - Rouge/Orange
        Palette::Dendro,  // R - Vert Prairie
        Palette::Anemo,   // I - Turquoise
        Palette::Geo      // S - Or / Ambre
    };

    const float tailleLettre = 48.f;
    const string titre = "TETRIS";
    const float largeurTotale = titre.size() * tailleLettre;
    float posX = larg / 2.f - largeurTotale / 2.f;

    for (size_t i = 0; i < titre.size(); i++) {
        sf::Text lettre(*m_police);
        lettre.setString(string(1, titre[i]));
        lettre.setCharacterSize(44);
        lettre.setFillColor(couleurs[i]);
        lettre.setStyle(sf::Text::Bold);

        sf::FloatRect bL = lettre.getLocalBounds();
        lettre.setOrigin({bL.position.x + bL.size.x / 2.f, bL.position.y + bL.size.y / 2.f});
        lettre.setPosition({posX + (i * tailleLettre) + (tailleLettre / 2.f), haut * 0.22f});
        m_lettreTitre.push_back(lettre);
    }

    // --- 2. Sous-titre des Auteurs "Emmanuel & Martinaud" ---
    m_sousTitreAuteurs.emplace(*m_police);
    m_sousTitreAuteurs->setString("Emmanuel & Martinaud");
    m_sousTitreAuteurs->setCharacterSize(16);
    m_sousTitreAuteurs->setFillColor(sf::Color(60, 50, 45));
    m_sousTitreAuteurs->setStyle(sf::Text::Bold);

    sf::FloatRect bAut = m_sousTitreAuteurs->getLocalBounds();
    m_sousTitreAuteurs->setOrigin({bAut.position.x + bAut.size.x / 2.f, bAut.position.y + bAut.size.y / 2.f});
    m_sousTitreAuteurs->setPosition({larg / 2.f, haut * 0.36f});

    // --- 3. Décoration : Mini Pièces Tetris au centre (comme sur la maquette) ---
    // Barres 4x1 (Vert Anemo)
    const float tailleBlocDeco = 26.f;
    float centreX = larg / 2.f;
    float centreY = haut * 0.54f;

    // Pièce I (4 blocs horizontaux)
    for (int c = -2; c < 2; ++c) {
        sf::RectangleShape bloc(sf::Vector2f({tailleBlocDeco - 1.f, tailleBlocDeco - 1.f}));
        bloc.setFillColor(Palette::Anemo);
        bloc.setPosition({centreX + c * tailleBlocDeco, centreY - tailleBlocDeco});
        m_blocsDeco.push_back(bloc);
    }

    // Pièce O (Carré 2x2 Or/Geo en dessous)
    for (int l = 0; l < 2; ++l) {
        for (int c = -1; c < 1; ++c) {
            sf::RectangleShape bloc(sf::Vector2f({tailleBlocDeco - 1.f, tailleBlocDeco - 1.f}));
            bloc.setFillColor(Palette::Geo);
            bloc.setPosition({centreX + c * tailleBlocDeco, centreY + l * tailleBlocDeco});
            m_blocsDeco.push_back(bloc);
        }
    }

    // --- 4. Décoration : Symboles Musicales (Notes ♪ ♫) ---
    struct NoteData { std::string sym; float x; float y; unsigned int sz; sf::Color col; };
    std::vector<NoteData> notes = {
        {"#",  larg * 0.12f, haut * 0.18f, 22, sf::Color(120, 110, 100)},
        {"*",  larg * 0.24f, haut * 0.14f, 18, sf::Color(130, 120, 110)},
        {"#",  larg * 0.52f, haut * 0.08f, 20, sf::Color(140, 130, 120)},
        {"*",  larg * 0.76f, haut * 0.12f, 18, sf::Color(130, 120, 110)},
        {"#",  larg * 0.86f, haut * 0.22f, 22, sf::Color(120, 110, 100)},
        {"*",  larg * 0.10f, haut * 0.44f, 20, sf::Color(140, 130, 110)},
        {"#",  larg * 0.90f, haut * 0.58f, 20, sf::Color(140, 130, 110)},
        {"*",  larg * 0.18f, haut * 0.70f, 18, sf::Color(130, 120, 110)},
        {"#",  larg * 0.80f, haut * 0.74f, 20, sf::Color(130, 120, 110)}
    };

    for (const auto& nd : notes) {
        sf::Text n(*m_police);
        n.setString(nd.sym);
        n.setCharacterSize(nd.sz);
        n.setFillColor(nd.col);
        n.setPosition({nd.x, nd.y});
        m_notesDeco.push_back(n);
    }

    // --- 5. Décoration : Lignes de Portée Musicales ---
    // Groupe de 5 lignes fines à droite et en bas à gauche
    auto ajouterPortee = [&](float posX, float posY, float largeur) {
        for (int i = 0; i < 5; ++i) {
            sf::RectangleShape ligne(sf::Vector2f({largeur, 1.f}));
            ligne.setFillColor(sf::Color(180, 175, 170));
            ligne.setPosition({posX, posY + i * 5.f});
            m_lignesDeco.push_back(ligne);
        }
    };

    ajouterPortee(larg * 0.72f, haut * 0.36f, 180.f); // Portée droite
    ajouterPortee(larg * 0.06f, haut * 0.82f, 180.f); // Portée bas gauche

    // --- 6. Texte d'instruction "APPUYEZ SUR ENTREE POUR COMMENCER" ---
    m_textConsigne.emplace(*m_police);
    m_textConsigne->setString("APPUYEZ SUR ENTREE POUR COMMENCER");
    m_textConsigne->setCharacterSize(14);
    m_textConsigne->setFillColor(Palette::TexteBoutonInactif);

    sf::FloatRect b = m_textConsigne->getLocalBounds();
    m_textConsigne->setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    m_textConsigne->setPosition({larg / 2.f, haut * 0.90f});
}

// Initialisation du Menu Principal (Accueil)
void Menu::initAccueil(float larg, float haut) {
    if (!m_police) return;

    m_titreMenu.emplace(*m_police);
    m_titreMenu->setString("MENU");
    m_titreMenu->setCharacterSize(36);
    m_titreMenu->setFillColor(Palette::BordureOr);
    m_titreMenu->setStyle(sf::Text::Bold);

    sf::FloatRect bTitre = m_titreMenu->getLocalBounds();
    m_titreMenu->setOrigin({bTitre.position.x + bTitre.size.x / 2.f,
                            bTitre.position.y + bTitre.size.y / 2.f});

    const float btnLarg    = 320.f;
    const float btnHaut    = 50.f;
    const float espacement = 14.f;
    const int   nbBoutons  = 5;

    float blocHaut  = nbBoutons * btnHaut + (nbBoutons - 1) * espacement;
    float titreHaut = 36.f + 16.f;
    float totalHaut = titreHaut + blocHaut;
    float offsetY   = (haut - totalHaut) / 2.f;

    m_titreMenu->setPosition({larg / 2.f, offsetY + 18.f});
    float premierBtnY = offsetY + titreHaut;

    vector<string> libelles = {"PLAY", "CONTINUE", "OPTIONS", "CREDITS", "EXIT"};

    for (size_t i = 0; i < libelles.size(); ++i) {
        BoutonUI btn(*m_police);

        float x = larg / 2.f - btnLarg / 2.f;
        float y = premierBtnY + (i * (btnHaut + espacement));

        btn.fond.setSize({btnLarg, btnHaut});
        btn.fond.setPosition({x, y});

        btn.bordure.setSize({btnLarg, btnHaut});
        btn.bordure.setPosition({x, y});
        btn.bordure.setFillColor(sf::Color::Transparent);
        btn.bordure.setOutlineThickness(4.f);

        btn.texte.setFont(*m_police);
        btn.texte.setString(libelles[i]);
        btn.texte.setCharacterSize(16);
        btn.texte.setStyle(sf::Text::Bold);

        sf::FloatRect bounds = btn.texte.getLocalBounds();
        btn.texte.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                              bounds.position.y + bounds.size.y / 2.f});
        btn.texte.setPosition({x + btnLarg / 2.f, y + btnHaut / 2.f});

        m_btnsAccueil.push_back(btn);
    }
}

void Menu::validerOption() {
    if (m_etat == MenuState::EcranTitre) {
        m_etat  = MenuState::Accueil;
        m_index = 0;
        majStyle();
        return;
    }

    if (m_etat == MenuState::Accueil) {
        switch (m_index) {
            case 0: /* PLAY */
                m_lancerJeu = true;
                break;
            case 1: /* CONTINUE */ break;
            case 2: /* OPTIONS */  break;
            case 3: /* CREDITS */  break;
            case 4: /* EXIT */
                exit(0);
            default: break;
        }
    }
}

void Menu::gererTouche(sf::Keyboard::Key touche) {
    if (m_etat == MenuState::EcranTitre) {
        if (touche == sf::Keyboard::Key::Enter ||
            touche == sf::Keyboard::Key::Space) {
            validerOption();
        }
        return;
    }

    int maxOptions = static_cast<int>(m_btnsAccueil.size());

    if (touche == sf::Keyboard::Key::Up) {
        m_index = (m_index - 1 + maxOptions) % maxOptions;
        majStyle();
    }
    else if (touche == sf::Keyboard::Key::Down) {
        m_index = (m_index + 1) % maxOptions;
        majStyle();
    }
    else if (touche == sf::Keyboard::Key::Enter) {
        validerOption();
    }
}

void Menu::gererMolette(float delta) {
    if (m_etat != MenuState::Accueil) return;

    int maxOptions = static_cast<int>(m_btnsAccueil.size());

    if (delta > 0.f)
        m_index = (m_index - 1 + maxOptions) % maxOptions;
    else if (delta < 0.f)
        m_index = (m_index + 1) % maxOptions;

    majStyle();
}

void Menu::gererSouris(sf::Vector2i pos) {
    if (m_etat != MenuState::Accueil) return;

    sf::Vector2f p(static_cast<float>(pos.x), static_cast<float>(pos.y));

    for (size_t i = 0; i < m_btnsAccueil.size(); ++i) {
        if (m_btnsAccueil[i].fond.getGlobalBounds().contains(p)) {
            m_index = static_cast<int>(i);
            majStyle();
            break;
        }
    }
}

void Menu::gererClic(sf::Vector2i pos) {
    if (m_etat == MenuState::EcranTitre) {
        validerOption();
        return;
    }

    sf::Vector2f p(static_cast<float>(pos.x), static_cast<float>(pos.y));

    for (size_t i = 0; i < m_btnsAccueil.size(); ++i) {
        if (m_btnsAccueil[i].fond.getGlobalBounds().contains(p)) {
            m_index = static_cast<int>(i);
            validerOption();
            break;
        }
    }
}

void Menu::majStyle() {
    if (m_etat != MenuState::Accueil) return;

    for (size_t i = 0; i < m_btnsAccueil.size(); ++i) {
        if (static_cast<int>(i) == m_index) {
            m_btnsAccueil[i].fond.setFillColor(Palette::Hydro);
            m_btnsAccueil[i].bordure.setOutlineColor(Palette::BordureOr);
            m_btnsAccueil[i].texte.setFillColor(Palette::TexteBoutonActif);
        }
        else {
            m_btnsAccueil[i].fond.setFillColor(Palette::FondBoutonInactif);
            m_btnsAccueil[i].bordure.setOutlineColor(sf::Color(150, 135, 90));
            m_btnsAccueil[i].texte.setFillColor(Palette::TexteBoutonInactif);
        }
    }
}

// ---------------------------------------------------------------
//  Menu Pause : init et événements
// ---------------------------------------------------------------
void Menu::initPause(float larg, float haut) {
    if (!m_police) return;

    m_overlayPause.setSize({larg, haut});
    m_overlayPause.setPosition({0.f, 0.f});
    m_overlayPause.setFillColor(sf::Color(0, 0, 0, 180));

    m_titrePause.emplace(*m_police);
    m_titrePause->setString("PAUSE");
    m_titrePause->setCharacterSize(36);
    m_titrePause->setFillColor(Palette::Anemo);
    m_titrePause->setStyle(sf::Text::Bold);

    sf::FloatRect bTitre = m_titrePause->getLocalBounds();
    m_titrePause->setOrigin({bTitre.position.x + bTitre.size.x / 2.f,
                             bTitre.position.y + bTitre.size.y / 2.f});

    const float btnLarg    = 320.f;
    const float btnHaut    = 50.f;
    const float espacement = 14.f;

    vector<string> libelles = {"REPRENDRE", "RECOMMENCER", "OPTIONS", "MENU PRINCIPAL"};
    const int      nbBoutons = static_cast<int>(libelles.size());

    float blocHaut  = nbBoutons * btnHaut + (nbBoutons - 1) * espacement;
    float titreHaut = 36.f + 20.f;
    float totalHaut = titreHaut + blocHaut;
    float offsetY   = (haut - totalHaut) / 2.f;

    m_titrePause->setPosition({larg / 2.f, offsetY + 18.f});
    float premierBtnY = offsetY + titreHaut;

    for (size_t i = 0; i < libelles.size(); ++i) {
        BoutonUI btn(*m_police);

        float x = larg / 2.f - btnLarg / 2.f;
        float y = premierBtnY + (i * (btnHaut + espacement));

        btn.fond.setSize({btnLarg, btnHaut});
        btn.fond.setPosition({x, y});

        btn.bordure.setSize({btnLarg, btnHaut});
        btn.bordure.setPosition({x, y});
        btn.bordure.setFillColor(sf::Color::Transparent);
        btn.bordure.setOutlineThickness(4.f);

        btn.texte.setFont(*m_police);
        btn.texte.setString(libelles[i]);
        btn.texte.setCharacterSize(14);
        btn.texte.setStyle(sf::Text::Bold);

        sf::FloatRect bounds = btn.texte.getLocalBounds();
        btn.texte.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                              bounds.position.y + bounds.size.y / 2.f});
        btn.texte.setPosition({x + btnLarg / 2.f, y + btnHaut / 2.f});

        m_btnsPause.push_back(btn);
    }

    majStylePause();
}

void Menu::majStylePause() {
    for (size_t i = 0; i < m_btnsPause.size(); ++i) {
        if (static_cast<int>(i) == m_indexPause) {
            m_btnsPause[i].fond.setFillColor(Palette::Anemo);
            m_btnsPause[i].bordure.setOutlineColor(Palette::BordureOr);
            m_btnsPause[i].texte.setFillColor(Palette::TexteBoutonActif);
        }
        else {
            m_btnsPause[i].fond.setFillColor(Palette::FondBoutonInactif);
            m_btnsPause[i].bordure.setOutlineColor(sf::Color(150, 135, 90));
            m_btnsPause[i].texte.setFillColor(Palette::TexteBoutonInactif);
        }
    }
}

void Menu::validerOptionPause() {
    switch (m_indexPause) {
        case 0: /* REPRENDRE */       m_reprise         = true; break;
        case 1: /* RECOMMENCER */     m_recommencer     = true; break;
        case 2: /* OPTIONS */         /* TODO */          break;
        case 3: /* MENU PRINCIPAL */  m_quitterVersMenu = true; break;
        default: break;
    }
}

void Menu::ouvrirPause() {
    m_indexPause = 0;
    majStylePause();
}

void Menu::fermerPause() {}

void Menu::gererTouchePause(sf::Keyboard::Key touche) {
    int maxOptions = static_cast<int>(m_btnsPause.size());

    if (touche == sf::Keyboard::Key::Up) {
        m_indexPause = (m_indexPause - 1 + maxOptions) % maxOptions;
        majStylePause();
    }
    else if (touche == sf::Keyboard::Key::Down) {
        m_indexPause = (m_indexPause + 1) % maxOptions;
        majStylePause();
    }
    else if (touche == sf::Keyboard::Key::Enter) {
        validerOptionPause();
    }
}

void Menu::gererSourisPause(sf::Vector2i pos) {
    sf::Vector2f p(static_cast<float>(pos.x), static_cast<float>(pos.y));

    for (size_t i = 0; i < m_btnsPause.size(); ++i) {
        if (m_btnsPause[i].fond.getGlobalBounds().contains(p)) {
            m_indexPause = static_cast<int>(i);
            majStylePause();
            break;
        }
    }
}

void Menu::gererClicPause(sf::Vector2i pos) {
    sf::Vector2f p(static_cast<float>(pos.x), static_cast<float>(pos.y));

    for (size_t i = 0; i < m_btnsPause.size(); ++i) {
        if (m_btnsPause[i].fond.getGlobalBounds().contains(p)) {
            m_indexPause = static_cast<int>(i);
            validerOptionPause();
            break;
        }
    }
}

void Menu::gererMolettePause(float delta) {
    int maxOptions = static_cast<int>(m_btnsPause.size());

    if (delta > 0.f)
        m_indexPause = (m_indexPause - 1 + maxOptions) % maxOptions;
    else if (delta < 0.f)
        m_indexPause = (m_indexPause + 1) % maxOptions;

    majStylePause();
}

void Menu::afficherPause(sf::RenderWindow& fenetre) {
    fenetre.draw(m_overlayPause);
    if (m_titrePause.has_value())
        fenetre.draw(*m_titrePause);

    for (const auto& btn : m_btnsPause) {
        fenetre.draw(btn.fond);
        fenetre.draw(btn.bordure);
        fenetre.draw(btn.texte);
    }
}

// -----------------------------------------------------------------------------
// EXPLICATION POUR LE PROFESSEUR :
// Initialise l'overlay Game Over affiché lors d'une défaite :
// 1. m_overlayGameOver : Rectangle semi-transparent (noir alpha=200) recouvrant la fenêtre.
// 2. m_titreGameOver : Titre "GAME OVER" rouge Pyro imposant.
// 3. m_sousTitreGameOver : Question "Voulez-vous recommencer ?" pour interroger le joueur.
// 4. m_btnsGameOver : Liste de 2 boutons interactifs ("RECOMMENCER" et "MENU PRINCIPAL").
// -----------------------------------------------------------------------------
void Menu::initGameOver(float larg, float haut) {
    if (!m_police) return;

    m_overlayGameOver.setSize({larg, haut});
    m_overlayGameOver.setPosition({0.f, 0.f});
    m_overlayGameOver.setFillColor(sf::Color(0, 0, 0, 200));

    // Titre "GAME OVER"
    m_titreGameOver.emplace(*m_police);
    m_titreGameOver->setString("GAME OVER");
    m_titreGameOver->setCharacterSize(36);
    m_titreGameOver->setFillColor(Palette::Pyro);
    m_titreGameOver->setStyle(sf::Text::Bold);

    sf::FloatRect bTitre = m_titreGameOver->getLocalBounds();
    m_titreGameOver->setOrigin({bTitre.position.x + bTitre.size.x / 2.f,
                               bTitre.position.y + bTitre.size.y / 2.f});

    // Sous-titre "Voulez-vous recommencer ?"
    m_sousTitreGameOver.emplace(*m_police);
    m_sousTitreGameOver->setString("Voulez-vous recommencer ?");
    m_sousTitreGameOver->setCharacterSize(14);
    m_sousTitreGameOver->setFillColor(sf::Color(230, 235, 245));

    sf::FloatRect bSousTitre = m_sousTitreGameOver->getLocalBounds();
    m_sousTitreGameOver->setOrigin({bSousTitre.position.x + bSousTitre.size.x / 2.f,
                                    bSousTitre.position.y + bSousTitre.size.y / 2.f});

    const float btnLarg    = 320.f;
    const float btnHaut    = 50.f;
    const float espacement = 16.f;

    vector<string> libelles = {"RECOMMENCER", "MENU PRINCIPAL"};
    const int nbBoutons = static_cast<int>(libelles.size());

    float blocHaut  = nbBoutons * btnHaut + (nbBoutons - 1) * espacement;
    float titreHaut = 36.f + 14.f + 30.f;
    float totalHaut = titreHaut + blocHaut;
    float offsetY   = (haut - totalHaut) / 2.f;

    m_titreGameOver->setPosition({larg / 2.f, offsetY + 18.f});
    m_sousTitreGameOver->setPosition({larg / 2.f, offsetY + 56.f});

    float premierBtnY = offsetY + titreHaut;

    for (size_t i = 0; i < libelles.size(); ++i) {
        BoutonUI btn(*m_police);

        float x = larg / 2.f - btnLarg / 2.f;
        float y = premierBtnY + (i * (btnHaut + espacement));

        btn.fond.setSize({btnLarg, btnHaut});
        btn.fond.setPosition({x, y});

        btn.bordure.setSize({btnLarg, btnHaut});
        btn.bordure.setPosition({x, y});
        btn.bordure.setFillColor(sf::Color::Transparent);
        btn.bordure.setOutlineThickness(4.f);

        btn.texte.setFont(*m_police);
        btn.texte.setString(libelles[i]);
        btn.texte.setCharacterSize(14);
        btn.texte.setStyle(sf::Text::Bold);

        sf::FloatRect bounds = btn.texte.getLocalBounds();
        btn.texte.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                              bounds.position.y + bounds.size.y / 2.f});
        btn.texte.setPosition({x + btnLarg / 2.f, y + btnHaut / 2.f});

        m_btnsGameOver.push_back(btn);
    }

    majStyleGameOver();
}

void Menu::majStyleGameOver() {
    for (size_t i = 0; i < m_btnsGameOver.size(); ++i) {
        if (static_cast<int>(i) == m_indexGameOver) {
            m_btnsGameOver[i].fond.setFillColor(Palette::Pyro);
            m_btnsGameOver[i].bordure.setOutlineColor(Palette::BordureOr);
            m_btnsGameOver[i].texte.setFillColor(Palette::TexteBoutonActif);
        }
        else {
            m_btnsGameOver[i].fond.setFillColor(Palette::FondBoutonInactif);
            m_btnsGameOver[i].bordure.setOutlineColor(sf::Color(150, 135, 90));
            m_btnsGameOver[i].texte.setFillColor(Palette::TexteBoutonInactif);
        }
    }
}

void Menu::validerOptionGameOver() {
    switch (m_indexGameOver) {
        case 0: /* RECOMMENCER */     m_recommencerGameOver     = true; break;
        case 1: /* MENU PRINCIPAL */  m_quitterVersMenuGameOver = true; break;
        default: break;
    }
}

void Menu::ouvrirGameOver() {
    m_indexGameOver = 0;
    majStyleGameOver();
}

void Menu::gererToucheGameOver(sf::Keyboard::Key touche) {
    int maxOptions = static_cast<int>(m_btnsGameOver.size());

    if (touche == sf::Keyboard::Key::Up) {
        m_indexGameOver = (m_indexGameOver - 1 + maxOptions) % maxOptions;
        majStyleGameOver();
    }
    else if (touche == sf::Keyboard::Key::Down) {
        m_indexGameOver = (m_indexGameOver + 1) % maxOptions;
        majStyleGameOver();
    }
    else if (touche == sf::Keyboard::Key::Enter) {
        validerOptionGameOver();
    }
}

void Menu::gererSourisGameOver(sf::Vector2i pos) {
    sf::Vector2f p(static_cast<float>(pos.x), static_cast<float>(pos.y));

    for (size_t i = 0; i < m_btnsGameOver.size(); ++i) {
        if (m_btnsGameOver[i].fond.getGlobalBounds().contains(p)) {
            m_indexGameOver = static_cast<int>(i);
            majStyleGameOver();
            break;
        }
    }
}

void Menu::gererClicGameOver(sf::Vector2i pos) {
    sf::Vector2f p(static_cast<float>(pos.x), static_cast<float>(pos.y));

    for (size_t i = 0; i < m_btnsGameOver.size(); ++i) {
        if (m_btnsGameOver[i].fond.getGlobalBounds().contains(p)) {
            m_indexGameOver = static_cast<int>(i);
            validerOptionGameOver();
            break;
        }
    }
}

void Menu::gererMoletteGameOver(float delta) {
    int maxOptions = static_cast<int>(m_btnsGameOver.size());

    if (delta > 0.f)
        m_indexGameOver = (m_indexGameOver - 1 + maxOptions) % maxOptions;
    else if (delta < 0.f)
        m_indexGameOver = (m_indexGameOver + 1) % maxOptions;

    majStyleGameOver();
}

void Menu::afficherGameOver(sf::RenderWindow& fenetre) {
    fenetre.draw(m_overlayGameOver);

    if (m_titreGameOver.has_value())
        fenetre.draw(*m_titreGameOver);

    if (m_sousTitreGameOver.has_value())
        fenetre.draw(*m_sousTitreGameOver);

    for (const auto& btn : m_btnsGameOver) {
        fenetre.draw(btn.fond);
        fenetre.draw(btn.bordure);
        fenetre.draw(btn.texte);
    }
}

// ---------------------------------------------------------------
//  Rendu de l'écran Titre décoré et de l'Accueil
// ---------------------------------------------------------------
void Menu::afficher(sf::RenderWindow& fenetre) {
    if (m_etat == MenuState::EcranTitre) {
        // 1. Dessiner les lignes de portée décoratives
        for (const auto& ligne : m_lignesDeco) {
            fenetre.draw(ligne);
        }

        // 2. Dessiner les notes de musique décoratives
        for (const auto& note : m_notesDeco) {
            fenetre.draw(note);
        }

        // 3. Dessiner les mini tetraminos au centre
        for (const auto& bloc : m_blocsDeco) {
            fenetre.draw(bloc);
        }

        // 4. Dessiner le logo TETRIS
        for (const auto& lettre : m_lettreTitre) {
            fenetre.draw(lettre);
        }

        // 5. Dessiner le sous-titre des auteurs "Emmanuel & Martinaud"
        if (m_sousTitreAuteurs.has_value()) {
            fenetre.draw(*m_sousTitreAuteurs);
        }

        // 6. Dessiner la consigne "APPUYEZ SUR ENTREE"
        if (m_textConsigne.has_value()) {
            fenetre.draw(*m_textConsigne);
        }
    }
    else if (m_etat == MenuState::Accueil) {
        if (m_titreMenu.has_value()) {
            fenetre.draw(*m_titreMenu);
        }
        for (const auto& btn : m_btnsAccueil) {
            fenetre.draw(btn.fond);
            fenetre.draw(btn.bordure);
            fenetre.draw(btn.texte);
        }
    }
}
