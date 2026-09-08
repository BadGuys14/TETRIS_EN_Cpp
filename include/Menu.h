#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>
#include "Palette.h"

// -----------------------------------------------------------------------------
// EXPLICATION POUR LE PROFESSEUR :
// Énumération des différents états du menu de l'application.
// - EcranTitre : Écran d'accueil décoré (splash screen) avec logo multicolore.
// - Accueil    : Menu principal avec les options de jeu (PLAY, OPTIONS, EXIT...).
// - Pause      : Overlay de pause affiché par-dessus la partie en cours.
// - GameOver   : Overlay de fin de partie avec question et boutons de reprise.
// -----------------------------------------------------------------------------
enum class MenuState {
    EcranTitre,  
    Accueil,     
    Pause,       
    GameOver     
};

class Menu
{
public:
    Menu();
    Menu(float larg, float haut, const sf::Font& police);

    void init(float larg, float haut, const sf::Font& police);

    // --- Événements du Menu Principal ---
    void gererTouche(sf::Keyboard::Key touche);
    void gererSouris(sf::Vector2i pos);
    void gererClic(sf::Vector2i pos);
    void gererMolette(float delta);

    // --- Rendu du Menu Principal et Splash Screen ---
    void afficher(sf::RenderWindow& fenetre);

    // Accesseurs
    MenuState getEtat()  const { return m_etat; }
    int       getIndex() const { return m_index; }
    bool      demandeLancementJeu() const { return m_lancerJeu; }
    void      resetLancementJeu() { m_lancerJeu = false; }

    // --- Menu Pause ---
    void ouvrirPause();
    void fermerPause();
    void gererTouchePause(sf::Keyboard::Key touche);
    void gererSourisPause(sf::Vector2i pos);
    void gererClicPause(sf::Vector2i pos);
    void gererMolettePause(float delta);
    void afficherPause(sf::RenderWindow& fenetre);

    bool demandeReprise()     const { return m_reprise; }
    bool demandeRecommencer() const { return m_recommencer; }
    bool demandeQuitter()     const { return m_quitterVersMenu; }
    void resetReprise()       { m_reprise     = false; }
    void resetRecommencer()   { m_recommencer = false; }
    void resetQuitter()       { m_quitterVersMenu = false; }

    // --- Écran Game Over ---
    void ouvrirGameOver();
    void gererToucheGameOver(sf::Keyboard::Key touche);
    void gererSourisGameOver(sf::Vector2i pos);
    void gererClicGameOver(sf::Vector2i pos);
    void gererMoletteGameOver(float delta);
    void afficherGameOver(sf::RenderWindow& fenetre);

    bool demandeRecommencerGameOver() const { return m_recommencerGameOver; }
    bool demandeMenuGameOver()        const { return m_quitterVersMenuGameOver; }
    void resetRecommencerGameOver()   { m_recommencerGameOver = false; }
    void resetMenuGameOver()          { m_quitterVersMenuGameOver = false; }

private:
    // Structure regroupant les 3 composants graphiques SFML d'un bouton UI
    struct BoutonUI {
        sf::RectangleShape fond;
        sf::RectangleShape bordure;
        sf::Text           texte;

        explicit BoutonUI(const sf::Font& police) : texte(police) {}
    };

    // Méthodes d'initialisation privées
    void initEcranTitre(float larg, float haut);
    void initAccueil(float larg, float haut);
    void initPause(float larg, float haut);
    void initGameOver(float larg, float haut);

    void majStyle();
    void majStylePause();
    void majStyleGameOver();

    void validerOption();
    void validerOptionPause();
    void validerOptionGameOver();

    MenuState       m_etat { MenuState::EcranTitre };
    const sf::Font* m_police { nullptr };
    int             m_index { 0 };
    int             m_indexPause { 0 };
    int             m_indexGameOver { 0 };
    bool            m_lancerJeu { false };

    // Flags du menu pause
    bool m_reprise          { false };
    bool m_recommencer      { false };
    bool m_quitterVersMenu  { false };

    // Flags de l'écran Game Over
    bool m_recommencerGameOver       { false };
    bool m_quitterVersMenuGameOver   { false };

    float m_larg { 0.f };
    float m_haut { 0.f };

    // --- Éléments du Splash Screen et du Menu ---
    std::vector<sf::Text>    m_lettreTitre;
    std::optional<sf::Text>  m_textConsigne;
    std::optional<sf::Text>  m_titreMenu;

    // --- Éléments Décoratifs du Titre ---
    std::optional<sf::Text>          m_sousTitreAuteurs; // Noms des auteurs "Emmanuel & Martinaud"
    std::vector<sf::Text>            m_notesDeco;        // Symboles de notes de musique décoratives (♪ ♫)
    std::vector<sf::RectangleShape>  m_lignesDeco;       // Portées / lignes de décoration
    std::vector<sf::RectangleShape>  m_blocsDeco;        // Mini tetraminos de décoration sous le titre

    // Boutons
    std::vector<BoutonUI>    m_btnsAccueil;
    std::vector<BoutonUI>    m_btnsPause;
    std::optional<sf::Text>  m_titrePause;
    sf::RectangleShape       m_overlayPause;

    // Écran Game Over
    std::vector<BoutonUI>    m_btnsGameOver;
    std::optional<sf::Text>  m_titreGameOver;
    std::optional<sf::Text>  m_sousTitreGameOver;
    sf::RectangleShape       m_overlayGameOver;
};
