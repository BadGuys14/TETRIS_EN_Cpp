#pragma once
#include <SFML/Graphics.hpp>

// Palette de couleurs - Genshin Impact (7 Archons / Elements)
namespace Palette {

    //-- Fond & Interface --
    const sf::Color FondSombre(30, 32, 40);             // Fond fenetre style arcade
    const sf::Color BordureOr(235, 195, 110);            // Bordure doree Geo
    const sf::Color FondBoutonInactif(190, 195, 205);    // Bouton inactif gris metallique
    const sf::Color FondBoutonGris = FondBoutonInactif;  // Alias compatibilite

    //-- Couleurs Elementaires GENSHIN IMPACT (Couleurs saturees officielles) --
    const sf::Color Anemo(116, 226, 196);   // Turquoise / Vert Anemo (Venti)
    const sf::Color Geo(248, 186, 56);      // Jaune Ambre / Or Geo (Zhongli)
    const sf::Color Electro(175, 142, 237); // Violet Electro (Raiden Shogun)
    const sf::Color Dendro(165, 213, 79);   // Vert Prairie Dendro (Nahida)
    const sf::Color Hydro(76, 196, 253);    // Bleu Ocean Hydro (Furina)
    const sf::Color Pyro(241, 100, 47);     // Rouge / Orange Pyro (Murata)
    const sf::Color Cryo(153, 236, 255);    // Bleu Givre Cryo (Tsaritsa)

    //-- Texte des boutons --
    const sf::Color TexteBoutonActif(248, 186, 56);   // Dore Geo pour bouton selectionne
    const sf::Color TexteBoutonInactif(45, 40, 30);   // Brun sombre pour bouton inactif

    //-- Lettres du logo TETRIS --
    const sf::Color Lettre_T1 = Pyro;    // T  - Pyro (Rouge/Orange)
    const sf::Color Lettre_E  = Hydro;   // E  - Hydro (Bleu)
    const sf::Color Lettre_T2 = Pyro;    // T  - Pyro (Rouge/Orange)
    const sf::Color Lettre_R  = Dendro;  // R  - Dendro (Vert)
    const sf::Color Lettre_I  = Anemo;   // I  - Anemo (Turquoise)
    const sf::Color Lettre_S  = Geo;     // S  - Geo (Or)
}
