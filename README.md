**1. Génération des évènements avec MadGraph**

Lancez MadGraph depuis votre terminal (version 2.9.27 ici) et exécutez les commandes suivantes pour générer le processus $pp->Z->mu+ mu-$ au format LHE :
```bash
> generate p p > z > mu+ mu-
> output ppZmumu
> launch
```
	

Pour modifier les paramètres physique, rendez-vous dans le ficher 'run_card.dat' (il se trouve dans votre dossier ppZmumu crée lors de l'étape précédente). Afin de réaliser les mêmes histogrammes, il vous faut lancer deux runs :
 	- Run 01 : N_events= 10000 et Ebeam1 = Ebeam2 = 6500 GeV (il s'agit du run de référence avec les paramètres par défaut de MadGraph);
	- Run 02 : Changez Ebeam2 =  1000 GeV.

Après avoir modifier le fichier 'run_card.dat' comme indiqué, il n'est pas utile de relancer MadGraph, il suffit de taper la commande suivante :

MG5_aMC> bin/generate_events

Pour chaque run, les événements sont générés sous forme d'archive 'Events/run_01/unweighted_events.lhe.gz' et 'Events/run_02/unweighted_events.lhe.gz'. Décompressez ces fichiers manuellement ou avec la commande :

MG5_aMC> gunzip unweighted_events.lhe.gz



**2. Conversion du fichier LHE en arbre ROOT**

Utilisez le script 'lhe2root.C' pour transformer les fichiers lhe en arbre ROOT. Depuis votre terminal, lancez ROOT (version  6.40.04) :

> root -l

et exécutez la macro :

[] .x lhe2root.C("ppZmumu/Events/run_01/unweighted_events.lhe", "nominal.root", -1)
[] .x lhe2root.C("ppZmumu/Events/run_02/unweighted_events.lhe", "asymetrique.root", -1)

N.B. : Assurez vous que votre fichier 'lhe2root.C' se trouve bien dans votre répertoire de travail.



**3. Exécution de l'analyse**

Le filtrage cinématique, le calcul des efficacités et les ajustements statistiques sont réalisés par la classe 'AnalyseZ'. Dans ROOT, ouvrez le fichier de données et lancez l'analyse :

[] TFile f("nominal.root");
[] Events->Process("AnalyseZ.C+");

N.B. : Avant de commencer cette étape, assurez vous que vos fichiers 'nominal.root', 'asymetrique.root', 'AnalyseZ.C' et 'AnalyseZ.h' soient dans votre répertoire de travail.

L'exécution de cette commande affiche dans le terminal :
	- le nombre d'événements (N_tot); 
	- le nombre d'évenements retenus après la sélection (N_pass);
	- l'efficacite globale (eps = N_pass/N_tot);
	- l'incertitude binomiale;
	- l'incertitude de Clopper-Pearson.

Elle génère également les figures suivantes :
	- l'efficacité différentielle ajustée en fonction de la pseudo-rapidité \eta;
	- l'ajustement de la masse invariante m_{\mu\mu} par une fonction de Breit-Wigner relativiste.

Afin de produire les différents histogrammes réalisés pour différentes valeur de l'impulsion transverse pT, il vous faudra modifier la sélection des évenements ligne 81 du ficher 'AnalyseZ.C' :

 " if (muons_event[i].Pt() > 25.0 && std::abs(muons_event[i].Eta()) < 2.4) "

Pour réaliser cette analyse avec les données du run 02, dans ROOT, ouvrez le fichier de données et lancez l'analyse :
	
[] TFile f("asymetrique.root");
[] Events->Process("AnalyseZ.C+");

