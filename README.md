# LocalGuard 0.5 — experimental

# LocalGuard 0.4 — experimental

# LocalGuard 0.2 — prototype Windows 10/11 x64
C++17, local, observation uniquement. Aucun envoi réseau, aucune neutralisation.

Visual Studio Community avec C++ Desktop et CMake :
```
cmake -S . -B build
cmake --build build --config Release
.\build\Release\LocalGuard.exe
```
Ctrl+C pour arrêter. Lancer dans un dossier accessible en écriture. ETW peut nécessiter une console élevée; en cas d'échec, le programme conserve les relevés toutes les 5 secondes. Ne pas désactiver l'antivirus.

ETW Kernel-Process compte les créations de processus; le moteur examine le compteur toutes les 5 secondes (seuil expérimental 30). Les tables TCP établies IPv4/IPv6 et UDP IPv4/IPv6 sont relevées périodiquement. UDP compte les points de terminaison locaux, pas les flux distants. Les alertes TCP par PID incluent le nom de l'exécutable si disponible; une réutilisation de PID peut entraîner une attribution erronée. Le journal est local et rotatif. Profil statistique en mémoire, non persistant.

Test portable du détecteur : `g++ -std=c++17 Detector.cpp DetectorTest.cpp -o test && ./test` (sans NDEBUG).

Limites : compilation Windows et ETW non validés ici; pertes ETW non mesurées; aucun suivi du contenu réseau, intégrité des fichiers, service Windows ou modèle neuronal. Seuils heuristiques non calibrés; charge Dual Core non mesurée. Les alertes ne prouvent pas une attaque. Une surveillance périodique peut manquer des connexions brèves.

## V0.3
Adaptive four-feature mean/variance model, heuristic risk score 0–100 (not a probability), and persistent profile.dat. Samples with alerts are excluded from training; this does not prevent slow poisoning or unsafe initial training. Train on a known-clean machine. Delete profile.dat while stopped to reset. ETW counts are treated as zero when unavailable, which can distort the baseline; reset after changing ETW availability. Protect the working directory with OS permissions. No automatic blocking. Windows build and dual-core resource usage remain unverified. File integrity, neural network and resource instrumentation are deferred.

Run detector tests: `cmake -S . -B build`, `cmake --build build --config Release`, `ctest --test-dir build -C Release --output-on-failure`.


Module ajouté : Response.
Mode observation uniquement. Les recommandations ne modifient pas le pare-feu. Aucun service ni transfert réseau ajouté. watch.txt est un fichier de test facultatif; empreinte non cryptographique en mémoire. Le rapport local ne contient que le score. Les modules restent dans un seul processus. Compilation Windows non vérifiée.


Module ajouté : Health.
Mode observation uniquement. Les recommandations ne modifient pas le pare-feu. Aucun service ni transfert réseau ajouté. watch.txt est un fichier de test facultatif; empreinte non cryptographique en mémoire. Le rapport local ne contient que le score. Les modules restent dans un seul processus. Compilation Windows non vérifiée.
