# Feu tricolore d'INTech

Ici réside le code le plus récent du feu tricolore du local

Le feu utilise maintenant un ESP32 connecté au réseau pour Maj plus rapide et interface web (de merde)

Oui le README est dégu ok, j'en ai rien à battre. Comme le code lol.

# Build

Il faut PlatformIO, perso j'utilise Core parce que fuck VS Code.

Il faut remplir les valeurs du réseau WIFI et DE l'OTA dans le fichier `secrets.py` en copiant la template `secrets.py.example`

Pour compiler:
`pio run`

La première fois il faut tout faire en USB en enlevant `upload_protocol` dans `platformio.ini`

Il faut upload le code puis les données:
`pio run -t upload`
`pio run -t uploadfs`

Après la première fois, l'upload à travers le WIFI est possible, les mêmes commandes s'applique donc.