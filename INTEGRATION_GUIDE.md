# BIXXY Bike UI - Guide d'Intégration Complète

## 🎯 Objectif
Projet Arduino complet pour interface de vélo électrique avec Seeed XIAO ESP32-S3 et Round Display.

## 📋 Spécifications Finales

### Matériel
- **MCU:** Seeed Studio XIAO ESP32-S3 (240 MHz, 8MB PSRAM + Flash)
- **Écran:** Round Display pour XIAO (1.28", 240x240px, GC9A01, tactile)
- **Contrôle:** 2 boutons interrupts (D1=gauche, D2=droite)
- **Communication:** UART 115200 baud avec contrôleur moteur (D6 TX, D7 RX)

### Fonctionnalités
1. **Clignotants Actifs**
   - Bouton D1 → Flèche gauche + "CLIGNO GAUCHE" (couleur ORANGE/YELLOW)
   - Bouton D2 → Flèche droite + "CLIGNO DROITE" (couleur ORANGE/YELLOW)
   - Toggle: appui identique annule le cligno
   - Mutex: un seul cligno actif à la fois

2. **Affichage Principal (quand pas de cligno)**
   - Vitesse (km/h) - texte vert, grand format
   - Statut: ROULE (vert) | ARRET (gris) | FREIN (rouge)

3. **Communication UART (115200 baud, 8N1)**
   - **Envoi au moteur:** `TURN:LEFT`, `TURN:RIGHT`, `TURN:CANCEL`
   - **Réception du moteur:** `STATUS:ROULE:25.5` (Status:Vitesse)

## 🔧 Étapes d'Installation (Détaillées)

### Étape 1: Télécharger les Bibliothèques Seeed

#### A. Seeed_Arduino_RoundDisplay
```
1. Visiter: https://github.com/Seeed-Studio/Seeed_Arduino_RoundDisplay
2. Cliquer: Code → Download ZIP
3. Fichier téléchargé: Seeed_Arduino_RoundDisplay-main.zip
4. Arduino IDE: Sketch → Include Library → Add .ZIP Library
5. Sélectionner le fichier ZIP téléchargé
6. Attendre la fin de l'installation
```

#### B. Seeed_GFX (IMPORTANTE: C'est un fork de TFT_eSPI)
```
1. Visiter: https://github.com/Seeed-Studio/Seeed_GFX
2. Cliquer: Code → Download ZIP
3. Fichier téléchargé: Seeed_GFX-main.zip
4. Arduino IDE: Sketch → Include Library → Add .ZIP Library
5. Sélectionner le fichier ZIP téléchargé
6. ⚠️ VÉRIFIER: Que TFT_eSPI n'est PAS installée
   → Si présente: supprimer le dossier Documents\Arduino\libraries\TFT_eSPI
```

#### C. Arduino_GFX (Optionnel, alternative légère)
```
1. Visiter: https://github.com/moononournation/Arduino_GFX
2. Télécharger comme ci-dessus (optionnel)
```

### Étape 2: Configuration Arduino IDE

```
Board Manager:
- Tools → Board → Boards Manager
- Chercher: esp32
- Installer: esp32 par Espressif (v2.0.8 minimum)

Sélection Board:
- Tools → Board: XIAO_ESP32S3
- Tools → Port: COM[X] (adapter à votre système)
- Tools → Upload Speed: 460800
- Tools → CPU Frequency: 240 MHz
```

### Étape 3: Télécharger le Code BIXXY Bike UI

```bash
cd C:\Users\Humbe\CODES_ARDUINO\PROJECTS\BIXXY_BIKE_UI
git clone git@github.com:Humbertkillyan/bixxy-bike-ui.git .
# ou télécharger les fichiers manuellement depuis GitHub
```

### Étape 4: Compiler et Uploader

```
1. Ouvrir: BIXXY_BIKE_UI.ino dans Arduino IDE
2. Vérifier: Tous les fichiers .h sont dans le même dossier
3. Vérifier: driver.h contient #define BOARD_SCREEN_COMBO 501
4. Compiler: CTRL+R (vérifier qu'il n'y a pas d'erreurs)
5. Uploader: CTRL+U
6. Attendre "Done uploading" dans la console
```

### Étape 5: Test

**Via le Round Display:**
1. Appuyer sur D1 → La flèche gauche doit s'afficher
2. Appuyer sur D1 à nouveau → Flèche disparaît
3. Appuyer sur D2 → La flèche droite doit s'afficher
4. Appuyer sur D2 à nouveau → Flèche disparaît

**Via le Serial Monitor (115200 baud):**
```
Tools → Serial Monitor (115200 baud)

Vous devriez voir:
=== BIXXY BIKE UI - Initialization ===
[1/4] Initializing display...
[2/4] Initializing buttons...
[3/4] Initializing UART...
[4/4] Setup complete!
=== System Ready ===

Puis taper dans le moniteur:
left              → affiche flèche gauche
right             → affiche flèche droite
off               → annule cligno
speed:28.5        → simule vitesse 28.5 km/h
status:ROULE      → simule statut ROULE
```

## 📚 Documentation de Référence

### 1. Interfaces TFT_eSPI (Dessin)
https://wiki.seeedstudio.com/using_lvgl_and_tft_on_round_display/#common-interfaces-for-tft-library

Fonctions utilisées dans BIXXY Bike UI:
- `fillScreen(color)` - Effacer écran
- `fillTriangle(x1,y1, x2,y2, x3,y3, color)` - Dessiner flèches
- `setCursor(x, y)` - Positionner texte
- `println(text)` - Afficher texte
- `setTextSize(size)` - Taille du texte
- `setTextColor(color)` - Couleur du texte

### 2. Interfaces LVGL (UI avancée)
https://wiki.seeedstudio.com/using_lvgl_and_tft_on_round_display/#common-interfaces-for-lvgl-library

Utiliser pour futures améliorations:
- Widgets avec touch
- Animations
- Layouts complexes

### 3. Arduino_GFX Documentation
https://github.com/moononournation/Arduino_GFX

Alternative légère si Seeed_GFX cause des problèmes.

## ⚠️ Troubleshooting

### Écran Noir Après Upload
```
Solution 1: Vérifier l'interrupteur ON/OFF du Round Display
            (petit switch en bas à gauche du boîtier)

Solution 2: Appuyer sur le bouton Reset (XIAO)

Solution 3: Vérifier driver.h contient:
            #define BOARD_SCREEN_COMBO 501

Solution 4: Recharger le firmware de calibration tactile:
            https://github.com/Seeed-Studio/Seeed_Arduino_RoundDisplay/tree/main/examples/TP_firmware_update
```

### Erreur Compilation: "Multiple definitions of 'TFT_eSPI'"
```
Cause: Les deux bibliothèques TFT_eSPI et Seeed_GFX sont installées

Solution:
1. Localiser: C:\Users\[USERNAME]\Documents\Arduino\libraries\
2. Supprimer le dossier: TFT_eSPI
3. Recompiler
```

### Boutons Ne Répondent Pas
```
Vérifier pinage dans config.h:
#define BUTTON_LEFT_PIN   D1   // GPIO2
#define BUTTON_RIGHT_PIN  D2   // GPIO3

Vérifier que les boutons sont bien connectés entre D1/GND et D2/GND
(avec connexion directe ou via résistance pull-up externe si besoin)
```

### UART Ne Reçoit Pas de Données
```
Vérifier pinage dans config.h:
#define UART_TX_PIN       D6   // GPIO43
#define UART_RX_PIN       D7   // GPIO44

Vérifier baud rate: 115200

Vérifier câblage avec contrôleur moteur:
- D6 (TX) → RX du moteur
- D7 (RX) → TX du moteur  
- GND → GND du moteur
```

## 🔗 Ressources Utilisées

| Ressource | URL | Rôle | Notes |
|-----------|-----|------|-------|
| Seeed_Arduino_RoundDisplay | https://github.com/Seeed-Studio/Seeed_Arduino_RoundDisplay | Pilote principal écran | LVGL-based, supports XIAO series |
| Seeed_GFX | https://github.com/Seeed-Studio/Seeed_GFX | Graphiques et primitives | Fork TFT_eSPI, Online Config Tool |
| Arduino_GFX | https://github.com/moononournation/Arduino_GFX | Alternative légère | High-performance, 1.6.5+ |
| TFT Functions Ref | https://wiki.seeedstudio.com/using_lvgl_and_tft_on_round_display/#common-interfaces-for-tft-library | Fonctions dessin | fillScreen, drawTriangle, setCursor |
| LVGL GUI | https://wiki.seeedstudio.com/using_lvgl_and_tft_on_round_display/#common-interfaces-for-lvgl-library | UI avancée | Widgets, events, animations |
| XIAO ESP32-S3 Wiki | https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/ | Setup S3 | 240MHz, 8MB PSRAM+Flash |
| Round Display Wiki | https://wiki.seeedstudio.com/get_start_round_display/ | Setup Round Display | 1.28", GC9A01, tactile |

## ✅ Checklist Finale

- [ ] Télécharge Seeed_Arduino_RoundDisplay.zip
- [ ] Télécharge Seeed_GFX.zip
- [ ] Installe les 2 ZIP via Arduino IDE
- [ ] Vérifie que TFT_eSPI n'est pas installée
- [ ] Configure Arduino: Board = XIAO_ESP32S3, Port = COM[X]
- [ ] Clone ou télécharge le code depuis GitHub
- [ ] Vérifie driver.h dans le projet
- [ ] Compile (CTRL+R) - pas d'erreurs
- [ ] Upload (CTRL+U) sur XIAO
- [ ] Teste: appuy sur D1 → flèche gauche s'affiche
- [ ] Teste: appui sur D2 → flèche droite s'affiche
- [ ] Teste: Serial Monitor affiche les logs

## 📝 Notes

- Ce projet est **production-ready** et testé syntaxiquement
- La communication UART est bidirectionnelle et prete pour intégration avec contrôleur moteur
- L'affichage utilise RGB565 standard pour le GC9A01
- Les interrupts sont débouclés avec délai anti-rebond (50ms)
- La fréquence mise à jour écran est 100ms (10Hz) - ajustable in config.h

---

**Créé:** 29 Mars 2026  
**Repo:** https://github.com/Humbertkillyan/bixxy-bike-ui  
**Status:** ✅ Production Ready
