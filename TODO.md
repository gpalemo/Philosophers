# TODO — Workflow complet pour `philo`

## 0) Cible du projet (à garder en tête)

- [ ] Le programme compile avec `make`
- [ ] Pas de variables globales
- [ ] 1 thread par philosophe
- [ ] 1 mutex par fourchette
- [ ] Logs au bon format, non mélangés
- [ ] Pas de data race
- [ ] Arrêt propre (mort ou quota de repas)

---

## 1) Préparer la base du projet

- [ ] Vérifier/compléter le `Makefile` (`NAME`, `all`, `clean`, `fclean`, `re`)
- [ ] Définir l'architecture des fichiers (`src/`, `include/`)
- [ ] Créer les structures principales dans `philo.h` (`t_table`, `t_philo`)
- [ ] Créer une mini liste de fonctions à implémenter (juste les signatures)

Sortie attendue : projet propre, qui compile au moins avec des fichiers vides/squelettes.

---

## 2) Parsing et validation des arguments

- [ ] Parser les 4/5 arguments
- [ ] Refuser valeurs invalides (non numériques, <= 0, overflow)
- [ ] Stocker les valeurs dans `t_table`
- [ ] Gérer proprement les erreurs (message + retour non nul)

Sortie attendue : lancer `./philo ...` ne crash pas, et refuse correctement les mauvaises entrées.

---

## 3) Outils de temps + affichage sécurisé

- [ ] Faire `current_time_ms()`
- [ ] Stocker `start_time`
- [ ] Faire une fonction `print_state(philo, "...")`
- [ ] Protéger l'affichage avec un mutex

Sortie attendue : les timestamps sont cohérents et les logs ne se chevauchent pas.

---

## 4) Initialisation complète de la simulation

- [ ] Allouer tableau de philosophes
- [ ] Allouer tableau de mutex (fourchettes)
- [ ] Initialiser chaque mutex
- [ ] Relier chaque philosophe à ses 2 fourchettes
- [ ] Initialiser `last_meal`, `meals_eaten`, etc.

Sortie attendue : tout est prêt avant de créer les threads.

---

## 5) Routine philosophe (version 1 simple)

- [ ] Créer la routine : prendre fourchettes → manger → dormir → penser
- [ ] Mettre à jour `last_meal` au bon moment
- [ ] Incrémenter `meals_eaten`
- [ ] Ajouter un petit décalage de départ (optionnel) pour réduire les conflits

Sortie attendue : la simulation tourne, même si tout n'est pas encore parfait.

---

## 6) Éviter le deadlock (étape clé)

- [ ] Choisir une stratégie (recommandé : ordre total des fourchettes)
- [ ] L'appliquer partout, de façon cohérente
- [ ] Vérifier qu'il n'y a pas de blocage permanent

Sortie attendue : pas de situation où tout le monde reste bloqué à attendre.

---

## 7) Monitoring et condition d'arrêt

- [ ] Ajouter une boucle de surveillance (dans le main ou thread dédié)
- [ ] Détecter la mort : `now - last_meal > time_to_die`
- [ ] Afficher `died` au bon format
- [ ] Arrêter la simulation quand un philosophe meurt
- [ ] Gérer aussi l'arrêt par quota de repas (si argument optionnel)

Sortie attendue : fin de simulation fiable et propre.

---

## 8) Cas limites obligatoires

- [ ] Cas `number_of_philosophers = 1`
- [ ] Timings très petits (stress)
- [ ] Éviter des logs après la mort
- [ ] Vérifier qu'aucun mutex ne reste lock à la fin

Sortie attendue : comportement propre même dans les cas pénibles.

---

## 9) Nettoyage mémoire + destruction mutex

- [ ] `pthread_join` de tous les threads créés
- [ ] `pthread_mutex_destroy` pour chaque mutex
- [ ] `free` de toutes les allocations
- [ ] Retour propre du programme

Sortie attendue : arrêt sans fuite évidente ni ressources oubliées.

---

## 10) Tests

- [ ] Test basique (3 à 5 philosophes)
- [ ] Test stress (beaucoup de philosophes)
- [ ] Test avec quota de repas
- [ ] Test cas 1 philosophe
- [ ] Vérifier visuellement les logs (format + ordre cohérent)

Sortie attendue : version stable pour correction.

---

## Routine de travail quotidienne (simple)

- [ ] Je choisis **1 seule étape** de ce TODO
- [ ] Je code seulement cette étape
- [ ] Je compile et je teste
- [ ] Si c'est stable, je coche la case
- [ ] Je passe à l'étape suivante

> Règle d'or : avancer petit, tester souvent, cocher quand c'est vraiment fini.

