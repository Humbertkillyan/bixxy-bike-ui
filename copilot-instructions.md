---
name: "BIXXY Bike UI Project"
description: "Arduino project for XIAO ESP32-S3 with Round Display - Bike turn signal UI and status indicator"
---

# BIXXY Bike UI - Affichage Clignotants et Statut Vélo

## 📋 Vue d'ensemble du Projet

Interface utilisateur pour un vélo électrique utilisant :
- **MCU:** Seeed Studio XIAO ESP32-S3
- **Écran:** Round Display pour XIAO (1.28", 240x240px)
- **Boutons:** 2 boutons d'interrupt (cligno gauche/droite) sur D1 et D2
- **Comm:** UART via D6/D7 pour vitesse et statut du moteur

### Fonctionnalités Principales

1. **Clignotants (Turn Signals)**
   - Bouton D1 → Flèche gauche sur écran (toggle)
   - Bouton D2 → Flèche droite sur écran (toggle)

2. **Affichage Principal**
   - Vitesse (reçue par UART)
   - Statut: `ROULE` / `ARRET` / `FREIN`
   - Affichage automatique quand pas de cligno actif

3. **Communication UART**
   - **Réception:** Vitesse + Statut du contrôleur moteur (D6/D7)
   - **Transmission:** État des boutons lors de changements

---

## 🔧 Configuration Matérielle

### Brochages

| Composant | Pin | GPIO | Notes |
|-----------|-----|------|-------|
| Bouton Cligno Gauche | D1 | GPIO2 | Interrupt, Pull-up interne |
| Bouton Cligno Droite | D2 | GPIO3 | Interrupt, Pull-up interne |
| UART TX (vers moteur) | D6 | GPIO43 | - |
| UART RX (du moteur) | D7 | GPIO44 | - |
| Round Display | Connecteur XIAO | - | SPI + Touch I2C |

### Configuration XIAO ESP32-S3

- **Carte:** XIAO_ESP32S3
- **Vitesse:** 240 MHz
- **Mémoire:** 8MB PSRAM + 8MB Flash
- **RTC:** Disponible sur Round Display (I2C)

---

## 📚 Bibliothèques Requises

### Installation via Arduino IDE

1. **Seeed_Arduino_RoundDisplay**
   ```
   https://github.com/Seeed-Studio/Seeed_Arduino_RoundDisplay.git
   ```
   - Pilote d'écran et tactile

2. **Seeed_GFX** (Fork de TFT_eSPI)
   ```
   https://github.com/Seeed-Studio/Seeed_GFX.git
   ```
   - Primitives graphiques (flèches, texte, etc.)

3. **I2C BM8563 RTC** (optionnel, pour horloge RTC)
   - Rechercher dans Arduino IDE Library Manager

### Instructions d'Installation

```bash
# Sketches → Include Library → Add .ZIP Library
# Sélectionner les fichiers ZIP téléchargés depuis GitHub
```

---

## 🔐 GitHub SSH Configuration

Ce projet utilise une clé SSH centralisée (`id_ed25519`) partagée entre tous les projets Bixxy Bike.

### Vérification SSH

```bash
ssh -T git@github.com
# → Hi USERNAME! You've successfully authenticated...

# Si besoin :
ssh-add ~/.ssh/id_ed25519
```

### Projets Liés

- [BIXXY_BIKE_BACKLIGHT](https://github.com/HumbertNguyen/BIXXY_BIKE_BACKLIGHT) - Contrôleur backlight moteur
- [BIXXY_BIKE_UI](https://github.com/HumbertNguyen/bixxy-bike-ui) - UI clignotants/statut (ce repo)

---

## 📁 Structure du Projet

```
bixxy-bike-ui/
├── README.md
├── BIXXY_BIKE_UI.ino         # Sketch principal
├── config.h                  # Définitions de pins et constantes
├── display.h                 # Gestion de l'affichage
├── buttons.h                 # Gestion des interrupts
├── uart.h                    # Protocole UART
└── driver.h                  # Configuration Seeed_GFX
```

---

## 🚀 Workflow Développement & Déploiement

### 1. Configuration Locale

```bash
cd C:\Users\Humbe\CODES_ARDUINO\PROJECTS\BIXXY_BIKE_UI
git clone git@github.com:HumbertNguyen/bixxy-bike-ui.git .
```

### 2. Upload Arduino

1. Ouvrir `BIXXY_BIKE_UI.ino` dans Arduino IDE
2. Sélectionner board: **XIAO_ESP32S3**
3. Sélectionner port COM
4. Compiler et uploader

### 3. Test Initial

- Appuyer sur D1 → Vérifier la flèche gauche s'affiche
- Appuyer sur D2 → Vérifier la flèche droite s'affiche
- Vérifier la communication UART sur le moniteur série

### 4. Commit et Push

```bash
git add .
git commit -m "feat: implement turn signals and status display UI"
git push origin main
```

---

## 🐛 Débogage

### Serial Monitor (115200 baud)

- Messages de démarrage
- État des boutons pressés
- Données UART reçues/envoyées

### Round Display

- Écran blanc au démarrage = Bon
- Pas d'affichage = Vérifier l'interrupteur d'alimentation (en bas à gauche)
- Touch non-réactif = Faire `TP_firmware_update`

---

## 📝 Notes pour les Agents Futurs

1. **GPIO Contraintes:** GPIO0, 45, 46 sont des strapping pins - NE PAS utiliser
2. **Mémoire:** Si besoin de features avancées, XIAO ESP32-S3 Plus a 16MB Flash
3. **UART:** Débits testés: 9600, 115200 (recommandé 115200)
4. **Interrupts:** D1/D2 sont tactiles (TOUCH1/TOUCH2) - Préférer GPIO interrupt physique
5. **TF Card:** Possible sur Round Display (CS=D2), mais pas utilisé ici

---

## 📝 Exemple de Premier Push

```bash
# 1. Initialiser le repo local (si non cloné)
git init
git remote add origin git@github.com:USERNAME/PROJECT_NAME.git
git branch -M main

# 2. Ajouter et commiter les fichiers
git add .
git commit -m "Initial commit"

# 3. Pousser vers GitHub
git push -u origin main
```

---

## 🔗 Commands Rapides

```bash
# Vérifier l'état SSH
ssh -T git@github.com

# Lister les clés chargées
ssh-add -l

# Charger la clé SSH (si nécessaire)
ssh-add C:\Users\Humbe\.ssh\id_ed25519

# Voir la configuration du remote
git remote -v

# Changer le remote de HTTPS à SSH
git remote set-url origin git@github.com:USERNAME/PROJECT_NAME.git
```

---

## 📚 Documentation de Référence

- [GitHub SSH Keys Documentation](https://docs.github.com/en/authentication/connecting-to-github-with-ssh)
- [Generating a new SSH key](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)
- [Testing SSH connection](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/testing-your-ssh-connection)

---

**Dernière mise à jour :** 29 Mars 2026
