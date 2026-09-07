#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>
#include "Palette.h"

// Etats du menu principal
enum class MenuState {
    EcranTitre,  // Ecran splash avec titre TETRIS multicolore
    Accueil,     // Menu principal avec 5 boutons
    Pause        // Menu pause overlay
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

    // --- Menu Pause ---
    void ouvrirPause();   // bascule l'etat interne vers Pause
    void fermerPause();   // retourne a Accueil (si besoin)

    // Gestion evenements du menu pause
    void gererTouchePause(sf::Keyboard::Key touche);
    void gererSourisPause(sf::Vector2i pos);
    void gererClicPause(sf::Vector2i pos);
    void gererMolettePause(float delta);

    // Rendu du menu pause (overlay)
    void afficherPause(sf::RenderWindow& fenetre);

    // Flags de resultat du menu pause
    bool demandeReprise()     const { return m_reprise; }
    bool demandeRecommencer() const { return m_recommencer; }
    bool demandeQuitter()     const { return m_quitterVersMenu; }
    void resetReprise()       { m_reprise     = false; }
    void resetRecommencer()   { m_recommencer = false; }
    void resetQuitter()       { m_quitterVersMenu = false; }

private:
    struct BoutonUI {
        sf::RectangleShape fond;
        sf::RectangleShape bordure;
        sf::Text           texte;

        explicit BoutonUI(const sf::Font& police) : texte(police) {}
    };

    void initEcranTitre(float larg, float haut);
    void initAccueil(float larg, float haut);
    void initPause(float larg, float haut);
    void majStyle();
    void majStylePause();
    void validerOption();
    void validerOptionPause();

    MenuState m_etat { MenuState::EcranTitre };
    const sf::Font* m_police { nullptr };
    int m_index { 0 };
    int m_indexPause { 0 };
    bool m_lancerJeu { false };

    // Flags pause
    bool m_reprise       { false };
    bool m_recommencer   { false };
    bool m_quitterVersMenu { false };

    float m_larg { 0.f };
    float m_haut { 0.f };

    std::vector<sf::Text>    m_lettreTitre;
    std::optional<sf::Text>  m_textConsigne;
    std::optional<sf::Text>  m_titreMenu;
    std::vector<BoutonUI>    m_btnsAccueil;
    std::vector<BoutonUI>    m_btnsPause;     // Boutons du menu pause
    std::optional<sf::Text>  m_titrePause;    // Titre "PAUSE"
    sf::RectangleShape       m_overlayPause;  // Fond semi-transparent
};
