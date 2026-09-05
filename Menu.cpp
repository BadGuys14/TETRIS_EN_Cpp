#include "Menu.h"
#include <cstdlib>

using namespace std;

Menu::Menu(float larg, float haut, const sf::Font& police)
    : m_police(police),
      m_textConsigne(police),
      m_titre(police),
      m_titreNiveau(police)
{
    initEcranTitre(larg, haut);
    initAccueil(larg, haut);
    initNiveau(larg, haut);
    majStyle();
}

void Menu::initEcranTitre(float larg, float haut) {
    string titre = "TETRIS";
    vector<sf::Color> couleurs = {
        Palette::PyroPastel,
        Palette::HydroPastel,
        Palette::PyroPastel,
        Palette::DendroPastel,
        Palette::AnemoPastel,
        Palette::GeoPastel
    };

    float posX = larg / 2.f - 180.f;
    for (size_t i = 0; i < titre.size(); i++) {
        sf::Text lettre(m_police);
        lettre.setString(string(1, titre[i]));
        lettre.setCharacterSize(60);
        lettre.setFillColor(couleurs[i]);
        lettre.setPosition({posX + (i * 60.f), haut / 3.f});
        m_lettreTitre.push_back(lettre);
    }

    m_textConsigne.setFont(m_police);
    m_textConsigne.setString("APPUYEZ SUR ENTREE");
    m_textConsigne.setCharacterSize(20);
    m_textConsigne.setFillColor(Palette::GeoPastel);

    sf::FloatRect bounds = m_textConsigne.getLocalBounds();
    m_textConsigne.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    m_textConsigne.setPosition({larg / 2.f, haut * 0.7f});
}

void Menu::initAccueil(float larg, float haut) {
    vector<string> libelles = {"JOUER", "NIVEAUX", "QUITTER"};
    for (size_t i = 0; i < libelles.size(); ++i) {
        BoutonUI btn(m_police);

        float x = larg / 2.f - 150.f;
        float y = haut / 2.f - 50.f + (i * 70.f);

        btn.fond.setSize({300.f, 50.f});
        btn.fond.setPosition({x, y});

        btn.bordure.setSize({300.f, 50.f});
        btn.bordure.setPosition({x, y});
        btn.bordure.setFillColor(sf::Color::Transparent);
        btn.bordure.setOutlineThickness(4.f);

        btn.texte.setFont(m_police);
        btn.texte.setString(libelles[i]);
        btn.texte.setCharacterSize(20);

        sf::FloatRect bounds = btn.texte.getLocalBounds();
        btn.texte.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
        btn.texte.setPosition({x + 150.f, y + 25.f});

        m_btnsAccueil.push_back(btn);
    }
}

void Menu::initNiveau(float larg, float haut) {
    m_titreNiveau.setFont(m_police);
    m_titreNiveau.setString("CHOISIR LE NIVEAU");
    m_titreNiveau.setCharacterSize(36);
    m_titreNiveau.setFillColor(Palette::GeoPastel);
    m_titreNiveau.setStyle(sf::Text::Bold);

    sf::FloatRect bTitre = m_titreNiveau.getLocalBounds();
    m_titreNiveau.setOrigin({bTitre.position.x + bTitre.size.x / 2.f, bTitre.position.y + bTitre.size.y / 2.f});
    m_titreNiveau.setPosition({larg / 2.f, 110.f});

    vector<string> options = {"NIVEAU 1", "NIVEAU 2", "NIVEAU 3", "RETOUR"};
    for (size_t i = 0; i < options.size(); ++i) {
        BoutonUI btn(m_police);

        float x = larg / 2.f - 100.f;
        float y = 190.f + (i * 65.f);

        btn.fond.setSize({200.f, 50.f});
        btn.fond.setPosition({x, y});

        btn.bordure.setSize({200.f, 50.f});
        btn.bordure.setPosition({x, y});
        btn.bordure.setFillColor(sf::Color::Transparent);
        btn.bordure.setOutlineThickness(3.f);

        btn.texte.setFont(m_police);
        btn.texte.setString(options[i]);
        btn.texte.setCharacterSize(20);

        sf::FloatRect bounds = btn.texte.getLocalBounds();
        btn.texte.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
        btn.texte.setPosition({x + 100.f, y + 25.f});

        m_btnsNiveau.push_back(btn);
    }
}

void Menu::validerOption() {
    if (m_etat == MenuState::EcranTitre) {
        m_etat = MenuState::Accueil;
        m_index = 0;
    }
    else if (m_etat == MenuState::Accueil) {
        if (m_index == 0) {
            // Option JOUER
        }
        else if (m_index == 1) {
            m_etat = MenuState::SelectionNiveau;
            m_index = 0;
        }
        else if (m_index == 2) {
            exit(0);
        }
    }
    else if (m_etat == MenuState::SelectionNiveau) {
        if (m_index < 3) {
            m_niveau = m_index + 1;
        }
        else {
            m_etat = MenuState::Accueil;
            m_index = 0;
        }
    }
}

void Menu::gererTouche(sf::Keyboard::Key touche) {
    if (m_etat == MenuState::EcranTitre) {
        if (touche == sf::Keyboard::Key::Enter || touche == sf::Keyboard::Key::Space) {
            validerOption();
        }
        return;
    }

    int maxOptions = (m_etat == MenuState::Accueil) ? static_cast<int>(m_btnsAccueil.size())
                                                    : static_cast<int>(m_btnsNiveau.size());

    if (touche == sf::Keyboard::Key::Up) {
        m_index = (m_index - 1 + maxOptions) % maxOptions;
    }
    else if (touche == sf::Keyboard::Key::Down) {
        m_index = (m_index + 1) % maxOptions;
    }
    else if (touche == sf::Keyboard::Key::Enter) {
        validerOption();
    }

    majStyle();
}

void Menu::gererMolette(float delta) {
    if (m_etat == MenuState::EcranTitre) return;

    int maxOptions = (m_etat == MenuState::Accueil) ? static_cast<int>(m_btnsAccueil.size())
                                                    : static_cast<int>(m_btnsNiveau.size());

    if (delta > 0) {
        m_index = (m_index - 1 + maxOptions) % maxOptions;
    }
    else if (delta < 0) {
        m_index = (m_index + 1) % maxOptions;
    }

    majStyle();
}

void Menu::gererSouris(sf::Vector2i pos) {
    sf::Vector2f p(static_cast<float>(pos.x), static_cast<float>(pos.y));

    if (m_etat == MenuState::Accueil) {
        for (size_t i = 0; i < m_btnsAccueil.size(); i++) {
            if (m_btnsAccueil[i].fond.getGlobalBounds().contains(p)) {
                m_index = static_cast<int>(i);
                majStyle();
                break;
            }
        }
    }
    else if (m_etat == MenuState::SelectionNiveau) {
        for (size_t i = 0; i < m_btnsNiveau.size(); i++) {
            if (m_btnsNiveau[i].fond.getGlobalBounds().contains(p)) {
                m_index = static_cast<int>(i);
                majStyle();
                break;
            }
        }
    }
}

void Menu::gererClic(sf::Vector2i pos) {
    if (m_etat == MenuState::EcranTitre) {
        validerOption();
        return;
    }

    sf::Vector2f p(static_cast<float>(pos.x), static_cast<float>(pos.y));

    if (m_etat == MenuState::Accueil) {
        for (size_t i = 0; i < m_btnsAccueil.size(); i++) {
            if (m_btnsAccueil[i].fond.getGlobalBounds().contains(p)) {
                m_index = static_cast<int>(i);
                validerOption();
                break;
            }
        }
    }
    else if (m_etat == MenuState::SelectionNiveau) {
        for (size_t i = 0; i < m_btnsNiveau.size(); ++i) {
            if (m_btnsNiveau[i].fond.getGlobalBounds().contains(p)) {
                m_index = static_cast<int>(i);
                validerOption();
                break;
            }
        }
    }

    majStyle();
}

void Menu::majStyle() {
    auto toutAppliquer = [this](vector<BoutonUI>& listeBoutons) {
        for (size_t i = 0; i < listeBoutons.size(); ++i) {
            if (static_cast<int>(i) == m_index) {
                listeBoutons[i].fond.setFillColor(Palette::HydroPastel);
                listeBoutons[i].bordure.setOutlineColor(Palette::BordureOr);
                listeBoutons[i].texte.setFillColor(sf::Color::White);
            } else {
                listeBoutons[i].fond.setFillColor(Palette::FondBoutonGris);
                listeBoutons[i].bordure.setOutlineColor(sf::Color(140, 140, 145));
                listeBoutons[i].texte.setFillColor(sf::Color(60, 60, 60));
            }
        }
    };

    if (m_etat == MenuState::Accueil) toutAppliquer(m_btnsAccueil);
    else if (m_etat == MenuState::SelectionNiveau) toutAppliquer(m_btnsNiveau);
}

void Menu::afficher(sf::RenderWindow& fenetre) {
    if (m_etat == MenuState::EcranTitre) {
        for (const auto& lettre : m_lettreTitre) {
            fenetre.draw(lettre);
        }
        fenetre.draw(m_textConsigne);
    }
    else if (m_etat == MenuState::Accueil) {
        for (const auto& btn : m_btnsAccueil) {
            fenetre.draw(btn.fond);
            fenetre.draw(btn.bordure);
            fenetre.draw(btn.texte);
        }
    }
    else if (m_etat == MenuState::SelectionNiveau) {
        fenetre.draw(m_titreNiveau);
        for (const auto& btn : m_btnsNiveau) {
            fenetre.draw(btn.fond);
            fenetre.draw(btn.bordure);
            fenetre.draw(btn.texte);
        }
    }
}
