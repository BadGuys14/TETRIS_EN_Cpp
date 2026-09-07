#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>
#include "Palette.h"

// Etats du menu principal
enum class MenuState {
    EcranTitre,  // Ecran splash avec titre TETRIS multicolore
    Accueil      // Menu principal avec 5 boutons
};

class Menu
{
public:
    Menu();
    Menu(float larg, float haut, const sf::Font& police);

    void init(float larg, float haut, const sf::Font& police);

    // Gestion des evenements (clavier + souris)
    void gererTouche(sf::Keyboard::Key touche);
    void gererSouris(sf::Vector2i pos);
    void gererClic(sf::Vector2i pos);
    void gererMolette(float delta);

    // Rendu
    void afficher(sf::RenderWindow& fenetre);

    // Accesseurs
    MenuState getEtat()  const { return m_etat; }
    int       getIndex() const { return m_index; }
    bool      demandeLancementJeu() const { return m_lancerJeu; }
    void      resetLancementJeu() { m_lancerJeu = false; }

private:
    struct BoutonUI {
        sf::RectangleShape fond;
        sf::RectangleShape bordure;
        sf::Text           texte;

        explicit BoutonUI(const sf::Font& police) : texte(police) {}
    };

    void initEcranTitre(float larg, float haut);
    void initAccueil(float larg, float haut);
    void majStyle();
    void validerOption();

    MenuState m_etat { MenuState::EcranTitre };
    const sf::Font* m_police { nullptr };
    int m_index { 0 };
    bool m_lancerJeu { false };

    std::vector<sf::Text>    m_lettreTitre;
    std::optional<sf::Text>  m_textConsigne;
    std::optional<sf::Text>  m_titreMenu;
    std::vector<BoutonUI>    m_btnsAccueil;
};
