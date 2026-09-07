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

    initEcranTitre(larg, haut);
    initAccueil(larg, haut);
    initPause(larg, haut);
    majStyle();
}

void Menu::initEcranTitre(float larg, float haut) {
    if (!m_police) return;

    // Couleurs saturees officielles des Archons Genshin Impact (image 4)
    vector<sf::Color> couleurs = {
        Palette::Pyro,    // T - Rouge/Orange (Murata)
        Palette::Hydro,   // E - Bleu Ocean (Furina)
        Palette::Pyro,    // T - Rouge/Orange
        Palette::Dendro,  // R - Vert Prairie (Nahida)
        Palette::Anemo,   // I - Turquoise (Venti)
        Palette::Geo      // S - Jaune Ambre / Or (Zhongli)
    };

    const float tailleLettre = 50.f;
    const string titre = "TETRIS";
    const float largeurTotale = titre.size() * tailleLettre;
    float posX = larg / 2.f - largeurTotale / 2.f;

    for (size_t i = 0; i < titre.size(); i++) {
        sf::Text lettre(*m_police);
        lettre.setString(string(1, titre[i]));
        lettre.setCharacterSize(48);
        lettre.setFillColor(couleurs[i]);
        lettre.setStyle(sf::Text::Bold);

        sf::FloatRect bL = lettre.getLocalBounds();
        lettre.setOrigin({bL.position.x + bL.size.x / 2.f, bL.position.y + bL.size.y / 2.f});
        lettre.setPosition({posX + (i * tailleLettre) + (tailleLettre / 2.f), haut / 3.f});
        m_lettreTitre.push_back(lettre);
    }

    // Texte "APPUYEZ SUR ENTREE" centre
    m_textConsigne.emplace(*m_police);
    m_textConsigne->setString("APPUYEZ SUR ENTREE");
    m_textConsigne->setCharacterSize(16);
    m_textConsigne->setFillColor(Palette::Geo);

    sf::FloatRect b = m_textConsigne->getLocalBounds();
    m_textConsigne->setOrigin({b.position.x + b.size.x / 2.f,
                                b.position.y + b.size.y / 2.f});
    m_textConsigne->setPosition({larg / 2.f, haut * 0.72f});
}

void Menu::initAccueil(float larg, float haut) {
    if (!m_police) return;

    // Titre "MENU"
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
            // Bouton selectionne : Hydro (Bleu Ocean Genshin), bordure Geo (Or)
            m_btnsAccueil[i].fond.setFillColor(Palette::Hydro);
            m_btnsAccueil[i].bordure.setOutlineColor(Palette::BordureOr);
            m_btnsAccueil[i].texte.setFillColor(Palette::TexteBoutonActif);
        }
        else {
            // Bouton inactif : fond gris metallique, bordure doree attenee
            m_btnsAccueil[i].fond.setFillColor(Palette::FondBoutonInactif);
            m_btnsAccueil[i].bordure.setOutlineColor(sf::Color(150, 135, 90));
            m_btnsAccueil[i].texte.setFillColor(Palette::TexteBoutonInactif);
        }
    }
}

// ---------------------------------------------------------------
//  Menu Pause : init
// ---------------------------------------------------------------
void Menu::initPause(float larg, float haut) {
    if (!m_police) return;

    // Fond semi-transparent couvrant toute la fenetre
    m_overlayPause.setSize({larg, haut});
    m_overlayPause.setPosition({0.f, 0.f});
    m_overlayPause.setFillColor(sf::Color(0, 0, 0, 160));

    // Titre "PAUSE"
    m_titrePause.emplace(*m_police);
    m_titrePause->setString("PAUSE");
    m_titrePause->setCharacterSize(36);
    m_titrePause->setFillColor(Palette::Anemo);
    m_titrePause->setStyle(sf::Text::Bold);

    sf::FloatRect bTitre = m_titrePause->getLocalBounds();
    m_titrePause->setOrigin({bTitre.position.x + bTitre.size.x / 2.f,
                             bTitre.position.y + bTitre.size.y / 2.f});

    const float btnLarg    = 300.f;
    const float btnHaut    = 50.f;
    const float espacement = 14.f;

    vector<string> libelles = {"REPRENDRE", "RECOMMENCER", "OPTIONS", "QUITTER"};
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
        case 0: /* REPRENDRE */    m_reprise       = true; break;
        case 1: /* RECOMMENCER */  m_recommencer   = true; break;
        case 2: /* OPTIONS */      /* TODO */        break;
        case 3: /* QUITTER */      m_quitterVersMenu = true; break;
        default: break;
    }
}

void Menu::ouvrirPause() {
    m_indexPause = 0;
    majStylePause();
}

void Menu::fermerPause() {
    // Rien a faire pour l'instant (l'etat est gere par Jeu)
}

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
    // Fond semi-transparent par-dessus la grille
    fenetre.draw(m_overlayPause);

    // Titre "PAUSE"
    if (m_titrePause.has_value())
        fenetre.draw(*m_titrePause);

    // Boutons
    for (const auto& btn : m_btnsPause) {
        fenetre.draw(btn.fond);
        fenetre.draw(btn.bordure);
        fenetre.draw(btn.texte);
    }
}

// ---------------------------------------------------------------

void Menu::afficher(sf::RenderWindow& fenetre) {
    if (m_etat == MenuState::EcranTitre) {
        for (const auto& lettre : m_lettreTitre) {
            fenetre.draw(lettre);
        }
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
