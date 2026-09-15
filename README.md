## 1. Génération des événements avec MadGraph

Lancez MadGraph depuis votre terminal (version 2.9.27 ici) et exécutez les commandes suivantes pour générer le processus $p p \rightarrow Z \rightarrow \mu^+ \mu^-$ au format LHE :

```text
generate p p > z > mu+ mu-
output ppZmumu
launch
```

Pour modifier les paramètres physiques, rendez-vous dans le fichier `run_card.dat` (situé dans votre dossier `ppZmumu` créé lors de l'étape précédente). Afin de réaliser les mêmes histogrammes, il vous faut lancer deux runs :

- **Run 01** : `N_events = 10000` et `Ebeam1 = Ebeam2 = 6500 GeV` (il s'agit du run de référence avec les paramètres par défaut de MadGraph).
- **Run 02** : Changez `Ebeam2 = 1000 GeV`.

Après avoir modifié le fichier `run_card.dat` comme indiqué, il n'est pas utile de relancer MadGraph, il suffit d'exécuter la commande suivante :

```bash
bin/generate_events
```

Pour chaque run, les événements sont générés sous forme d'archive dans `Events/run_01/unweighted_events.lhe.gz` et `Events/run_02/unweighted_events.lhe.gz`. Décompressez ces fichiers manuellement ou avec la commande :

```bash
gunzip unweighted_events.lhe.gz
```

---

## 2. Conversion du fichier LHE en arbre ROOT

Utilisez le script `lhe2root.C` pour transformer les fichiers LHE en arbre ROOT. Depuis votre terminal, lancez ROOT (version 6.40.04) :

```bash
root -l
```

Puis exécutez la macro :

```cpp
.x lhe2root.C("ppZmumu/Events/run_01/unweighted_events.lhe", "nominal.root", -1)
.x lhe2root.C("ppZmumu/Events/run_02/unweighted_events.lhe", "asymetrique.root", -1)
```

> [!NOTE]
> Assurez-vous que votre fichier `lhe2root.C` se trouve bien dans votre répertoire de travail.

---

## 3. Exécution de l'analyse

Le filtrage cinématique, le calcul des efficacités et les ajustements statistiques sont réalisés par la classe `AnalyseZ`. Dans ROOT, ouvrez le fichier de données et lancez l'analyse :

```cpp
TFile f("nominal.root");
Events->Process("AnalyseZ.C+");
```

> [!NOTE]
> Avant de commencer cette étape, assurez-vous que vos fichiers `nominal.root`, `asymetrique.root`, `AnalyseZ.C` et `AnalyseZ.h` soient présents dans votre répertoire de travail.

L'exécution de cette commande affiche dans le terminal :
- Le nombre total d'événements ($N_{\text{tot}}$) ;
- Le nombre d'événements retenus après la sélection ($N_{\text{pass}}$) ;
- L'efficacité globale ($\varepsilon = N_{\text{pass}} / N_{\text{tot}}$) ;
- L'incertitude binomiale ;
- L'incertitude de Clopper-Pearson.

Elle génère également les figures suivantes :
- L'efficacité différentielle ajustée en fonction de la pseudo-rapidité $\eta$ ;
- L'ajustement de la masse invariante $m_{\mu\mu}$ par une fonction de Breit-Wigner relativiste.

Afin de produire les différents histogrammes réalisés pour différentes valeurs de l'impulsion transverse $p_T$, il vous faudra modifier la sélection des événements ligne 81 du fichier `AnalyseZ.C` :

```cpp
if (muons_event[i].Pt() > 25.0 && std::abs(muons_event[i].Eta()) < 2.4)
```

Pour réaliser cette analyse avec les données du Run 02, dans ROOT, ouvrez le fichier correspondant et lancez l'analyse :

```cpp
TFile f("asymetrique.root");
Events->Process("AnalyseZ.C+");
```
