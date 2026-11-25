# LocalGuard 0.1
Windows 10/11, C++17, Visual Studio 2022 avec développement Desktop C++ et CMake.

Dans PowerShell, depuis le dossier extrait :

    cmake -S . -B build
    cmake --build build --config Release
    .\build\Release\LocalGuard.exe

Arrêt : Ctrl+C. Laisser la console ouverte. Exécuter sans droits administrateur initialement.
Échantillonnage toutes les 5 secondes environ. Profil en mémoire : 20 échantillons acceptés, puis moyenne exponentielle. Les anomalies ne mettent pas à jour le profil (sauf premier échantillon). Démarrer sur une machine saine. Journal local guard.log, rotation à environ 1 Mio, une archive. Aucun blocage, aucun envoi réseau.

Limites : instantanés des connexions TCP établies et nombre de processus uniquement, pas de capture de paquets, UDP, contenu, mémoire, identité des exécutables ni détection garantie. Les connexions brèves peuvent échapper au sondage. Seuils expérimentaux, faux positifs possibles. Profil perdu au redémarrage. Pas de service Windows ni de protection anti-altération. Ce prototype ne remplace pas Defender. Compilation Windows et performances non vérifiées. Ne pas déployer sur infrastructure critique sans validation.
