# Tetris en C++ — SFML 3.1.0

Projet de Tetris en C++ utilisant la bibliotheque SFML 3.

---

## Structure du projet

```
TETRIS_EN_Cpp/
├── src/
│   ├── main.cpp          Point d entree du programme
│   ├── jeu.cpp           Boucle principale, fenetre, evenements
│   ├── grille.cpp        Plateau 10x20, dessin des cases
│   ├── Menu.cpp          Menus : titre, accueil, pause
│   └── Piece.cpp         (en cours)
├── include/
│   ├── jeu.hpp           Classe Jeu + enum EtatJeu (Menu / EnJeu / Pause)
│   ├── grille.hpp        Classe Grille
│   ├── Menu.h            Classe Menu + enum MenuState
│   ├── Palette.h         Constantes de couleurs (theme Genshin Impact)
│   └── Piece.hpp         (en cours)
├── assets/
│   └── fonts/            Polices TTF utilisees
└── README.md             Ce fichier
```

---

## Modifications apportees lors de la session du 07/09/2026

### Palette.h — Ajouts

| Ligne | Constante | Valeur RGB | Utilisation |
|-------|-----------|------------|-------------|
| 8  | `FondSombre`          | (30, 32, 40)    | Fond de la fenetre (style arcade sombre) |
| 9  | `BordureOr`           | (235, 195, 110) | Bordure doree des boutons (Geo / Zhongli) |
| 10 | `FondBoutonInactif`   | (200, 205, 215) | Fond gris metallique bouton non survole |
| 11 | `FondBoutonGris`      | alias ci-dessus | Conserve pour compatibilite ancienne version |
| 22 | `TexteBoutonActif`    | (245, 210, 120) | Texte dore sur bouton selectionne |
| 23 | `TexteBoutonInactif`  | (60, 50, 30)    | Texte brun sombre sur bouton inactif |

Les couleurs elementaires (AnemoPastel, GeoPastel, ElectroPastel, DendroPastel,
HydroPastel, PyroPastel, CryoPastel) correspondent aux 7 Archons de Genshin Impact
(image 4 du projet) et servent aussi aux couleurs des pieces Tetris (image 3).

---

### Menu.h — Modifications

**MenuState simplifie :**
```cpp
// AVANT (3 etats)
enum class MenuState { EcranTitre, Accueil, SelectionNiveau };

// APRES (2 etats — SelectionNiveau supprime car non requis)
enum class MenuState { EcranTitre, Accueil };
```

**Nouveau membre :**
- `sf::Text m_titreMenu` : label "MENU" affiche au-dessus des boutons

**Nouveau getter :**
- `int getIndex() const` : permet a Jeu de savoir quel bouton est selectionne

**Membres supprimes :**
- `m_titreNiveau`, `m_btnsNiveau` : appartenaient a l ecran de selection de niveau

---

### Menu.cpp — Modifications detaillees

#### initEcranTitre() — lignes 22 a 58

Les lettres T-E-T-R-I-S sont colorees avec les pastels Genshin (vecteur `couleurs`).

**Centrage du texte "APPUYEZ SUR ENTREE" (lignes 54-57) :**
```cpp
sf::FloatRect b = m_textConsigne.getLocalBounds();
m_textConsigne.setOrigin({b.position.x + b.size.x / 2.f,
                           b.position.y + b.size.y / 2.f});
m_textConsigne.setPosition({larg / 2.f, haut * 0.72f});
```

**Explication sf::FloatRect :**
`getLocalBounds()` retourne un rectangle (`sf::FloatRect`) decrivant la boite
englobante du texte dans son propre espace local. En SFML 3, ce rectangle a :
- `.position` : coin haut-gauche de la boite  (sf::Vector2f)
- `.size`     : dimensions de la boite (sf::Vector2f)

> SFML 2 utilisait `.left`, `.top`, `.width`, `.height`.
> SFML 3 utilise `.position.x`, `.position.y`, `.size.x`, `.size.y`.

La boite locale ne commence pas forcement en (0,0) car les polices ont un
"ascender" (espace au-dessus des majuscules). C est pourquoi on ajoute
`b.position.x` et `b.position.y` pour obtenir le vrai centre visuel.

**Explication setOrigin() :**
L origine est le point d ancrage du sprite/texte. Par defaut c est (0,0)
= coin haut-gauche, ce qui fait que `setPosition()` positionne le coin.
En placant l origine au centre geometrique, `setPosition()` positionne
le centre visuel directement. Formule :

```
origine.x = b.position.x + b.size.x / 2
origine.y = b.position.y + b.size.y / 2
```

---

#### initAccueil() — lignes 70 a 128

**Calcul du centrage vertical (lignes 83-94) :**
```cpp
float blocHaut  = 5 * 54.f + 4 * 16.f;  // = 334 px
float titreHaut = 42.f + 18.f;           // = 60  px
float totalHaut = titreHaut + blocHaut;   // = 394 px
float offsetY   = (haut - totalHaut) / 2.f; // = (600-394)/2 = 103 px
```
Tous les boutons sont espaces de 16 px et empiles a partir de `premierBtnY`.

**Explication setOutlineThickness() (ligne 113) :**
```cpp
btn.bordure.setOutlineThickness(4.f);
```
Dessine un trait de 4 pixels sur le pourtour du `RectangleShape`.
- Valeur **positive** : le contour depasse VERS L EXTERIEUR du rectangle.
- Valeur **negative** : le contour rentre VERS L INTERIEUR.
La couleur du contour est definie separement avec `setOutlineColor()`.
Le fond du shape `bordure` est mis a `sf::Color::Transparent` pour ne pas
cacher le fond `btn.fond` dessine juste en dessous.

**Centrage du texte dans chaque bouton (lignes 121-124) :**
```cpp
sf::FloatRect bounds = btn.texte.getLocalBounds();
btn.texte.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                      bounds.position.y + bounds.size.y / 2.f});
btn.texte.setPosition({x + btnLarg / 2.f,   // milieu horizontal du bouton
                        y + btnHaut  / 2.f}); // milieu vertical du bouton
```
Meme technique que pour "APPUYEZ SUR ENTREE" : on centre l origine puis
on place l origine au milieu geometrique du rectangle de bouton.

---

#### majStyle() — lignes 240-257

Applique les couleurs selon l etat de chaque bouton :

| Etat | Fond | Contour | Texte |
|------|------|---------|-------|
| Selectionne | `HydroPastel` (135,195,240) | `BordureOr` (235,195,110) | `TexteBoutonActif` (245,210,120) |
| Inactif     | `FondBoutonInactif` (200,205,215) | dore attenue (160,145,100) | `TexteBoutonInactif` (60,50,30) |

---

#### Double gestion des controles

| Methode | Declencheur | Role |
|---------|-------------|------|
| `gererTouche()` | `sf::Event::KeyPressed` | Fleches Haut/Bas + Entree/Espace |
| `gererSouris()` | `sf::Event::MouseMoved` | Hover : selectionne le bouton survole |
| `gererClic()`   | `sf::Event::MouseButtonPressed` | Clic : selectionne et valide |
| `gererMolette()`| `sf::Event::MouseWheelScrolled` | Molette : defilement |

---

## Ce qui manque pour que grille / jeu / main fonctionnent avec Menu

### 1. jeu.hpp — Ajouter Menu comme membre

```cpp
// A ajouter dans les includes de jeu.hpp :
#include "Menu.h"
#include "Palette.h"

// A ajouter dans la section private de class Jeu :
Menu        m_menu;      // instance du menu
sf::Font    m_police;    // police partagee entre Menu et le reste du jeu
bool        m_enJeu;     // true = partie en cours, false = on est au menu
```

`Menu` a besoin d une `sf::Font` passee par reference au constructeur.
Cette police doit etre chargee une fois dans `Jeu` et rester en vie
aussi longtemps que le `Menu`.

### 2. jeu.cpp — Charger la police et initialiser Menu

**Dans le constructeur `Jeu::Jeu()` :**
```cpp
// Charger la police (remplacer par le chemin de ta police .ttf)
if (!m_police.openFromFile("assets/police.ttf")) {
    // Gerer l erreur : fichier introuvable
}

// Construire le menu avec les dimensions de la fenetre
// (attention : LARGEUR = 600, LONGUEUR = 800 dans ton code actuel
//  => la fenetre fait 600 de large et 800 de haut.
//  Si tu veux 800x600 (paysage), il faut inverser dans jeu.hpp)
m_menu = Menu(static_cast<float>(LARGEUR_FENETRE),
               static_cast<float>(LONGUEUR_FENETRE),
               m_police);

// Changer la couleur de fond de FondSombre
COULEUR_FENETRE = Palette::FondSombre; // (30, 32, 40)
```

**Dans `gesEvenements()` — transmettre les evenements au menu :**
```cpp
void Jeu::gesEvenements() {
    while (const std::optional event = m_fenetre.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            m_fenetre.close();

        if (!m_enJeu) {
            // --- Transmettre les evenements au menu ---
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
    }
}
```

**Dans `affichage()` — afficher menu OU jeu selon l etat :**
```cpp
void Jeu::affichage() {
    m_fenetre.clear(Palette::FondSombre);

    if (!m_enJeu) {
        m_menu.afficher(m_fenetre); // affiche l ecran titre ou les boutons
    } else {
        m_grille.dessiner(m_fenetre); // affiche la grille de jeu
    }

    m_fenetre.display();
}
```

**Dans `miseAJour()` — detecter si PLAY a ete clique :**
```cpp
void Jeu::miseAJour() {
    if (!m_enJeu && m_menu.getEtat() == MenuState::Accueil) {
        if (m_menu.getIndex() == 0) {
            // Le joueur a valide PLAY => demarrer la partie
            // (pour l instant getIndex() ne retourne pas l etat "vient d etre valide"
            //  il faudra ajouter un systeme de signal/callback ou un bool m_actionDeclenchee)
        }
    }
}
```

### 3. main.cpp — Nettoyage

`main.cpp` inclut deja `Menu.h` et `Palette.h` mais ne les utilise pas directement.
Ces includes peuvent rester (aucun impact) ou etre supprimes puisque `jeu.hpp`
les incluera.

### 4. Probleme de dimensions de la fenetre dans jeu.hpp

```cpp
// ACTUELLEMENT dans jeu.hpp :
static const int LONGUEUR_FENETRE = 800;  // => hauteur
static const int LARGEUR_FENETRE  = 600;  // => largeur

// La fenetre creee dans jeu.cpp :
m_fenetre(sf::VideoMode({LARGEUR_FENETRE, LONGUEUR_FENETRE}), ...)
// => VideoMode({600, 800}) = fenetre de 600 px de large et 800 px de haut (portrait)
```

Le `Menu` a ete concu pour **800 px de large x 600 px de haut** (paysage).
Pour corriger, inverser les valeurs dans `jeu.hpp` :
```cpp
static const int LARGEUR_FENETRE  = 800; // largeur (X)
static const int LONGUEUR_FENETRE = 600; // hauteur (Y)
```

### 5. Systeme de signal Menu -> Jeu (a concevoir)

Actuellement `validerOption()` fait `exit(0)` pour EXIT mais ne peut pas
signaler "PLAY clique" a `Jeu` proprement. Solutions possibles :

**Option A (simple) :** ajouter un getter `bool demandeJeu() const` dans Menu
qui retourne `true` quand PLAY est selectionne et valide.

**Option B (propre) :** utiliser un `enum class Action { Aucune, Jouer, Quitter }`
retourne par `validerOption()`.

---

## Resume des fichiers a modifier pour integration complete

| Fichier | Action requise |
|---------|---------------|
| `jeu.hpp` | Ajouter `#include "Menu.h"`, membres `m_menu`, `m_police`, `m_enJeu`; corriger dimensions |
| `jeu.cpp` | Charger la police, construire `m_menu`, router les evenements, choisir affichage menu/jeu |
| `main.cpp` | Optionnel : nettoyer les includes inutilises |
| `Menu.cpp` | Implementer les actions PLAY/CONTINUE/OPTIONS/CREDITS dans `validerOption()` |

---

## Modifications du 07/09/2026 — Option A (paysage 800x600) + fenetre redimensionnable

### Probleme corrige : fenetre en portrait au lieu de paysage

Les constantes dans `jeu.hpp` etaient inversees :

```cpp
// AVANT (portrait 600 x 800 — mauvais pour le menu)
static const int LONGUEUR_FENETRE = 800;
static const int LARGEUR_FENETRE  = 600;

// APRES (paysage 800 x 600 — correct)
static const int LARGEUR_FENETRE  = 800; // axe X (horizontal)
static const int LONGUEUR_FENETRE = 600; // axe Y (vertical)
```

### grille.hpp — TAILLE_CASE reduit de 40 a 28

Avec une fenetre de 600 px de haut et 20 lignes :
- `20 x 40 = 800 px` depasse la fenetre
- `20 x 28 = 560 px` tient avec 20 px de marge haut/bas

```
Fenetre 800 x 600 (Option A — paysage)
+--------------------------+------------------------+
|  Plateau Tetris          |  Zone laterale         |
|  280 x 560 px            |  520 x 600 px          |
|  (10 col x 20 lig)       |  Score / Piece suivante|
|  TAILLE_CASE = 28 px     |  Niveau (a implementer)|
+--------------------------+------------------------+
```

### jeu.cpp — Fenetre redimensionnable

```cpp
Jeu::Jeu()
    : m_fenetre(
        sf::VideoMode({800u, 600u}),
        "Tetris en C++",
        sf::Style::Default   // flag de redimensionnement
    )
{}
```

**Explication sf::Style (SFML 3) :**

`sf::Style` est un ensemble de drapeaux (flags) combinables avec `|` :

| Flag | Effet |
|------|-------|
| `sf::Style::None`       | Fenetre sans decoration |
| `sf::Style::Titlebar`   | Barre de titre |
| `sf::Style::Resize`     | Bords glissables : rend la fenetre redimensionnable |
| `sf::Style::Close`      | Bouton de fermeture (X) |
| `sf::Style::Default`    | = Titlebar OR Resize OR Close (les trois) |
| `sf::Style::Fullscreen` | Plein ecran |

`sf::Style::Default` active donc le redimensionnement via `sf::Style::Resize`.
Le flag est pose, mais la LOGIQUE de redimensionnement (recalcul de vue,
repositionnement des elements) sera implementee plus tard dans `gesEvenements()`
via `sf::Event::Resized` quand ce sera demande.

### jeu.hpp — Couleur de fond mise a jour

```cpp
// AVANT
static inline sf::Color COULEUR_FENETRE = sf::Color::White;

// APRES (style arcade sombre, identique a Palette::FondSombre)
static inline sf::Color COULEUR_FENETRE = sf::Color(30, 32, 40);
```

---

## Modifications du 07/09/2026 (soir) — EtatJeu a 3 valeurs + Menu Pause

### Contexte

Jusqu a present, `Jeu` utilisait un simple `bool m_enJeu` pour distinguer
"afficher le menu" de "afficher la grille". Ce booleen devient insuffisant
des que l on introduit un troisieme etat : la pause. Il a ete remplace par
un `enum class` a 3 valeurs.

---

### jeu.hpp — bool m_enJeu → enum class EtatJeu

```cpp
// AVANT
bool m_enJeu { false };

// APRES
enum class EtatJeu {
    Menu,   // Menus principaux (titre + accueil)
    EnJeu,  // Partie en cours
    Pause   // Menu pause
};

// Dans la classe Jeu :
EtatJeu m_etat { EtatJeu::Menu };
```

**Pourquoi un enum class plutot qu un entier ?**
- Lisibilite : `m_etat == EtatJeu::Pause` est explicite
- Securite : pas de confusion avec un int arbitraire
- Extensibilite : ajouter `GameOver`, `Cinematique`, etc. sans changer le type

---

### Menu.h — Ajout de MenuState::Pause + membres du menu pause

**MenuState etendu :**
```cpp
// AVANT
enum class MenuState { EcranTitre, Accueil };

// APRES
enum class MenuState { EcranTitre, Accueil, Pause };
```

**Nouvelles methodes publiques :**

| Methode | Role |
|---------|------|
| `gererTouchePause(key)` | Fleches Haut/Bas + Entree dans le menu pause |
| `gererSourisPause(pos)` | Hover : met en surbrillance le bouton survole |
| `gererClicPause(pos)` | Clic gauche : selectionne et valide |
| `gererMolettePause(delta)` | Molette : defilement dans la liste |
| `afficherPause(fenetre)` | Rendu de l overlay pause par-dessus la grille |
| `ouvrirPause()` | Reinitialise l index et le style du menu pause |
| `fermerPause()` | (reserve — l etat est gere par Jeu) |

**Flags de resultat (pattern signal simple) :**
```cpp
bool demandeReprise()     const;   // REPRENDRE clique
bool demandeRecommencer() const;   // RECOMMENCER clique
bool demandeQuitter()     const;   // QUITTER clique
void resetReprise();
void resetRecommencer();
void resetQuitter();
```
Ce pattern evite tout couplage direct Menu → Jeu : `Jeu` interroge les flags
a chaque `miseAJour()` puis les reinitialise apres traitement.

**Nouveaux membres prives :**

| Membre | Type | Role |
|--------|------|------|
| `m_btnsPause` | `vector<BoutonUI>` | 4 boutons du menu pause |
| `m_titrePause` | `optional<sf::Text>` | Titre "PAUSE" en turquoise Anemo |
| `m_overlayPause` | `sf::RectangleShape` | Fond noir semi-transparent |
| `m_indexPause` | `int` | Index du bouton selectionne dans le menu pause |
| `m_reprise` | `bool` | Flag : reprendre demande |
| `m_recommencer` | `bool` | Flag : recommencer demande |
| `m_quitterVersMenu` | `bool` | Flag : retour au menu principal demande |
| `m_larg`, `m_haut` | `float` | Dimensions stockees pour `initPause()` |

---

### Menu.cpp — Nouvelles methodes implementees

#### initPause()

Construit les 4 boutons du menu pause centres verticalement :

```
Boutons du menu pause :
  REPRENDRE    → m_reprise       = true
  RECOMMENCER  → m_recommencer   = true
  OPTIONS      → (TODO)
  QUITTER      → m_quitterVersMenu = true
```

Style du titre : couleur `Palette::Anemo` (turquoise), taille 36, Bold.
Fond overlay : `sf::Color(0, 0, 0, 160)` — noir a 63 % d opacite.

#### majStylePause()

Identique a `majStyle()` mais pour `m_btnsPause` :

| Etat | Fond | Contour | Texte |
|------|------|---------|-------|
| Selectionne | `Palette::Anemo` | `BordureOr` | `TexteBoutonActif` |
| Inactif | `FondBoutonInactif` | dore attenue | `TexteBoutonInactif` |

#### afficherPause()

Ordre de dessin (important pour le z-order SFML) :
1. `m_overlayPause` — fond semi-transparent couvrant toute la fenetre
2. `m_titrePause`   — titre "PAUSE"
3. `m_btnsPause`    — fond + bordure + texte de chaque bouton

---

### jeu.cpp — Adaptation a EtatJeu

#### gesEvenements()

La touche **Echap** fait basculer entre `EnJeu` et `Pause` :
```cpp
if (e->code == sf::Keyboard::Key::Escape) {
    if (m_etat == EtatJeu::EnJeu)  m_etat = EtatJeu::Pause;
    else if (m_etat == EtatJeu::Pause) m_etat = EtatJeu::EnJeu;
}
```

Le routing des evenements suit ensuite l etat courant :
- `EtatJeu::Menu`  → evenements transmis a `m_menu` (methodes normales)
- `EtatJeu::EnJeu` → evenements reserves au gameplay (a implementer)
- `EtatJeu::Pause` → evenements transmis a `m_menu` (methodes `*Pause`)

#### miseAJour()

```cpp
if (m_etat == EtatJeu::Pause) {
    if (m_menu.demandeReprise())     { m_etat = EtatJeu::EnJeu; m_menu.resetReprise(); }
    if (m_menu.demandeRecommencer()) { m_grille = Grille(); m_etat = EtatJeu::EnJeu; m_menu.resetRecommencer(); }
    if (m_menu.demandeQuitter())     { m_etat = EtatJeu::Menu; m_grille = Grille(); m_menu.resetQuitter(); }
}
```

`m_grille = Grille()` reinitialise la grille en utilisant l operateur
d affectation par defaut (la grille est copiable/assignable).

#### affichage()

```cpp
if      (m_etat == EtatJeu::Menu)  m_menu.afficher(m_fenetre);
else if (m_etat == EtatJeu::EnJeu) m_grille.dessiner(m_fenetre);
else if (m_etat == EtatJeu::Pause) {
    m_grille.dessiner(m_fenetre);    // arriere-plan
    m_menu.afficherPause(m_fenetre); // overlay par-dessus
}
```

---

### Resume des etats et transitions

```
         [Menu]
           |
      PLAY clique
           |
           v
        [EnJeu] <-------- REPRENDRE (menu pause)
           |                  ^
         Echap                |
           |                  |
           v                  |
        [Pause] --------------+
           |
        RECOMMENCER → reinitialise grille → [EnJeu]
           |
        QUITTER     → reinitialise grille → [Menu]
```

---

## A implementer (prochaines etapes)

| Fonctionnalite | Fichiers concernes |
|----------------|--------------------|
| Logique de jeu (chute des pieces, rotation, lignes) | `Piece.cpp`, `grille.cpp` |
| Ecran OPTIONS (volume, difficulte...) | `Menu.h`, `Menu.cpp` |
| Ecran GAME OVER | `jeu.hpp`, `jeu.cpp`, `Menu.h` |
| Ecran CREDITS | `Menu.h`, `Menu.cpp` |
| Gestion du redimensionnement de fenetre | `jeu.cpp` (sf::Event::Resized) |
| Score et niveau | `jeu.hpp`, `jeu.cpp`, `grille.hpp` |
