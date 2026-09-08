#pragma once
#include <SFML/Graphics.hpp>

// Palette de couleurs - Genshin Impact (7 Archons / Éléments - Couleurs Vives et Saturated)
namespace Palette {

    //-- Fond & Interface --
    const sf::Color FondSombre(30, 32, 40);             // Fond fenêtre style arcade
    const sf::Color BordureOr(255, 195, 50);            // Bordure dorée Geo éclatante
    const sf::Color FondBoutonInactif(190, 195, 205);    // Bouton inactif gris métallique
    const sf::Color FondBoutonGris = FondBoutonInactif;  // Alias compatibilité

    //-- Couleurs Élémentaires GENSHIN IMPACT (Hyper-Saturées et Éclatantes) --
    const sf::Color Anemo(0, 230, 180);     // Turquoise / Vert Anemo éclatant (Venti)
    const sf::Color Geo(255, 180, 0);       // Or / Ambre Geo intense (Zhongli)
    const sf::Color Electro(170, 70, 255);  // Violet Électrique vibrant (Raiden Shogun)
    const sf::Color Dendro(130, 230, 30);   // Vert Prairie Dendro pur (Nahida)
    const sf::Color Hydro(0, 165, 255);     // Bleu Océan Hydro vif (Furina)
    const sf::Color Pyro(255, 60, 20);      // Rouge / Orange Pyro incandescent (Murata)
    const sf::Color Cryo(120, 235, 255);    // Bleu Givre Cryo lumineux (Tsaritsa)

    //-- Texte des boutons --
    const sf::Color TexteBoutonActif(255, 220, 80);   // Doré Geo pour bouton sélectionné
    const sf::Color TexteBoutonInactif(45, 40, 30);   // Brun sombre pour bouton inactif

    //-- Lettres du logo TETRIS --
    const sf::Color Lettre_T1 = Pyro;    // T  - Pyro
    const sf::Color Lettre_E  = Hydro;   // E  - Hydro
    const sf::Color Lettre_T2 = Pyro;    // T  - Pyro
    const sf::Color Lettre_R  = Dendro;  // R  - Dendro
    const sf::Color Lettre_I  = Anemo;   // I  - Anemo
    const sf::Color Lettre_S  = Geo;     // S  - Geo
}
