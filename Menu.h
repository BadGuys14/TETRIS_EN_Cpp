#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Palette.h"

enum class MenuState {
    EcranTitre,
    Accueil, // Ecran d'accueil
    SelectionNiveau //Ecran de choix de niveau
};

class Menu
{
    public:
        Menu(float larg, float haut, const sf::Font& police);

        void gererTouche(sf::Keyboard::Key touche);
        void gererSouris(sf::Vector2i pos);
        void gererClic(sf::Vector2i pos);
        void gererMolette(float delta);
        void afficher(sf::RenderWindow& fenetre);

        MenuState getEtat() const {return m_etat;}
        int getNiveau() const {return m_niveau;}

    private:
        //  Structure pour un bouton encadre
        struct BoutonUI{
            sf::RectangleShape fond;
            sf::RectangleShape bordure;
            sf::Text texte;

            //constructeur pour la police
            BoutonUI(const sf::Font& police) : texte(police){}
        };

        void initEcranTitre(float larg, float haut);
        void initAccueil(float larg, float haut);
        void initNiveau(float larg, float haut);
        void majStyle(); //Met à jour l'apparence
        void validerOption();

        MenuState m_etat{MenuState::EcranTitre};
        const sf::Font& m_police;
        int m_index{0};
        int m_niveau{1};

        //UI Ecran titre (titre multicolore)
        std::vector<sf::Text> m_lettreTitre;//Explication
        sf::Text m_textConsigne; //Explication

        //UI Accueil
        sf::Text m_titre;
        std::vector<BoutonUI> m_btnsAccueil; //explication


        //UI Niveau
        sf::Text m_titreNiveau;//explication
        std::vector<BoutonUI> m_btnsNiveau;//explication

};

