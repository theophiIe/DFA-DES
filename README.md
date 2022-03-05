# DFA-DES

L'objectif de ce projet est de retrouver à partir d'un message clair et d'un message chiffré ainsi que 32 messages chiffrés fautés la clé secrete K.  

Le programme en C va d'abord rechercher la sous clé numéro 16, puis va récuperer les 56 bits de la clé.
On va ensuite attaquer les 8 bits manquants, les 2^8 possibilités sont écrits dans un fichier texte `potential.txt`.  

Le programme en python va utiliser le fichier contenant les 256 clés possible pour déterminer laquelle est la vraie, une fois trouver on ajoute les bits de parités.

## Compilation
```bash
make
```

## Execution
```bash
make run
```
