# Philosophers

## 1) Le projet en une phrase

Faire un programme qui simule des philosophes autour d'une table : ils **mangent, dorment, pensent** en boucle, partagent des fourchettes, sans bugs de concurrence, avec des messages propres, et une détection correcte quand l'un meurt.

---

## 2) Rappel des règles du sujet

### Entrée du programme

Le programme reçoit :

`number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]`

- `number_of_philosophers` : nombre de philosophes (et donc de fourchettes)
- `time_to_die` (ms) : temps maximum sans commencer un repas avant de mourir
- `time_to_eat` (ms) : durée d'un repas
- `time_to_sleep` (ms) : durée du sommeil
- `number_of_times_each_philosopher_must_eat` (optionnel) : si tout le monde a mangé au moins ce nombre de fois, on arrête la simulation

### Comportement attendu

- Chaque philosophe est un **thread**.
- Il y a une fourchette entre chaque paire de philosophes.
- Pour manger, un philosophe doit prendre **2 fourchettes** (gauche + droite).
- S'il n'y a qu'1 philosophe, il n'a qu'une seule fourchette : il ne peut donc jamais vraiment manger, et il mourra après `time_to_die`.
- La simulation s'arrête :
  - à la mort d'un philosophe, ou
  - si l'argument optionnel est fourni et que tous les philosophes ont atteint le quota.

### Logs obligatoires

Format exact :

- `timestamp_in_ms X has taken a fork`
- `timestamp_in_ms X is eating`
- `timestamp_in_ms X is sleeping`
- `timestamp_in_ms X is thinking`
- `timestamp_in_ms X died`

Contraintes :

- Les messages ne doivent pas se mélanger entre eux (affichage protégé).
- Le message de mort doit être affiché très rapidement (<= 10 ms après la mort).
- **Aucune data race**.

### Fonctions autorisées

`memset, printf, malloc, free, write, usleep, gettimeofday, pthread_create, pthread_detach, pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock`

---

## 3) Concepts théoriques indispensables

## 3.1 Thread (pthread)

Un **thread**, c'est comme une petite "ligne de travail" qui tourne en même temps que d'autres dans le même programme.

- Tous les threads partagent la même mémoire du programme.
- Chaque thread a quand même sa propre pile (ses variables locales d'appels de fonctions).

Dans `philo`, chaque philosophe est un thread qui exécute une routine en boucle :

1. Prendre les fourchettes
2. Manger
3. Dormir
4. Penser

### `pthread_create`

Crée un thread :

- on lui donne la fonction à exécuter
- on lui passe souvent un pointeur vers la structure du philosophe

### `pthread_join` / `pthread_detach`

- `join` : on attend qu'un thread se termine
- `detach` : on ne l'attend pas, le système nettoie ses ressources à la fin

Pour `philo`, une approche classique est de créer tous les threads puis de les `join` proprement en fin de simulation.

---

## 3.2 Mutex

Un **mutex**, c'est un cadenas.
Il sert à empêcher deux threads de toucher en même temps à la même ressource.

- `pthread_mutex_lock` : je prends le cadenas
- `pthread_mutex_unlock` : je rends le cadenas

Dans `philo`, on utilise en général :

1. **1 mutex par fourchette** (ressource partagée)
2. **1 mutex d'affichage** (éviter mélange des logs)
3. éventuellement un mutex pour protéger des champs partagés (`stop`, `last_meal`, compteurs...)

---

## 3.3 Data race

Une **data race**, c'est quand plusieurs threads lisent/écrivent la même donnée en même temps sans protection.
Résultat : comportement imprévisible.

Exemples typiques dans `philo` :

- `last_meal` lu par le thread de monitoring et écrit par le philosophe
- un flag global de fin (`simulation_stopped`) lu/écrit par plusieurs threads
- compteur `meals_eaten`

Solution : protéger tous ces accès par mutex (ou stratégie de propriété stricte + synchro explicite).

---

## 3.4 Deadlock

Un **deadlock**, c'est un blocage total : chacun attend l'autre, et plus rien n'avance.

Exemple classique :

- chaque philosophe prend sa fourchette gauche
- puis attend indéfiniment la droite

Personne ne peut continuer.

### Stratégies anti-deadlock

Plusieurs stratégies possibles (en choisir une et rester cohérent) :

1. **Ordre total des fourchettes**
	- toujours prendre d'abord la fourchette avec le plus petit index, puis l'autre
2. **Pair/impair**
	- les pairs prennent droite puis gauche, les impairs font l'inverse
3. **Décalage de départ**
	- faire attendre un peu certains philosophes au début (réduit les conflits)

La stratégie 1 (ordre total) est souvent la plus robuste pour raisonner.

---

## 3.5 Starvation

La **starvation** (famine), c'est quand un philosophe n'arrive presque jamais à manger, même si le programme n'est pas bloqué.

Dans ce projet, on veut une simulation propre, stable, sans data races, et qui limite ce problème.

---

## 3.6 Temps en millisecondes

Le sujet se base sur le temps en ms.

Points clés :

- stocker un `start_time`
- calculer `timestamp = now - start_time`
- vérifier régulièrement : `now - last_meal > time_to_die`
- `usleep` travaille en microsecondes (1 ms = 1000 us)

Astuce : une fonction utilitaire type `current_time_ms()` simplifie beaucoup le code.

---

## 4) Architecture recommandée

## 4.1 Structures utiles

- `t_rules` / `t_table` : paramètres communs de la simulation (nombres, temps, drapeau d'arrêt, tableaux de mutex/fourchettes...)
- `t_philo` : état d'un philosophe (`id`, `last_meal`, `meals_eaten`, pointeurs vers fourchettes, thread...)

> Comme les globals sont interdites, ces structures sont allouées et passées via pointeurs.

## 4.2 Responsabilités

1. **Parsing/validation des arguments**
2. **Initialisation** (structures + mutex + philosophes)
3. **Lancement des threads philosophes**
4. **Monitoring** (mort / condition de fin par quota)
5. **Arrêt propre** (join + destroy mutex + free)

---

## 5) Déroulé logique d'une simulation

1. Lire et valider les arguments
2. Initialiser la table et les philosophes
3. Noter `start_time`
4. Créer les threads
5. Chaque philosophe boucle :
	- prendre 2 fourchettes (avec lock)
	- log `has taken a fork` (x2)
	- mettre à jour `last_meal`
	- log `is eating`
	- attendre `time_to_eat`
	- relâcher 2 fourchettes (unlock)
	- log `is sleeping` puis attendre `time_to_sleep`
	- log `is thinking`
6. Le monitoring arrête tout si :
	- un philosophe meurt, ou
	- tous ont atteint `number_of_times_each_philosopher_must_eat`

---

## 6) Cas limites à traiter absolument

1. **`number_of_philosophers = 1`**
2. Valeurs invalides (<= 0, non numériques, overflow)
3. Timingsau format exact
- [ ] Logs sérialisés (pas de chevauchement)
- [ ] Mort détectée et affichée rapidement
- [ ] Pas de data races
- [ ] Arrêt propre + destruction des mutex
- [ ] Makefile avec `NAME`, `all`, `clean`, `fclean`, `re`

---

## 8) Plan d'attaque concret

1. Écrire les fonctions de temps + affichage protégé par mutex
2. Parsing robuste des arguments
3. Initialisation complète des structures/mutex
4. Routine philosophe minimale (sans optimisation)
5. Monitoring de mort fiable
6. Condition d'arrêt sur quota de repas
7. Gestion des cas limites + nettoyage complet
8. Tests intensifs de timings

--- très petits (sensible au scheduler)
4. Éviter d'afficher des actions après une mort
5. Nettoyage même en cas d'erreur partielle d'initialisation

---

## 7) Checklist de conformité

- [ ] Pas de variables globales
- [ ] Un thread par philosophe
- [ ] Un mutex par fourchette
- [ ] Logs au format exact
- [ ] Logs sérialisés (pas de chevauchement)
- [ ] Mort détectée et affichée rapidement
- [ ] Pas de data races
- [ ] Arrêt propre + destruction des mutex
- [ ] Makefile avec `NAME`, `all`, `clean`, `fclean`, `re`

---

## 8) Plan d'attaque concret

1. Écrire les fonctions de temps + affichage protégé par mutex
2. Parsing robuste des arguments
3. Initialisation complète des structures/mutex
4. Routine philosophe minimale (sans optimisation)
5. Monitoring de mort fiable
6. Condition d'arrêt sur quota de repas
7. Gestion des cas limites + nettoyage complet
8. Tests intensifs de timings

---
