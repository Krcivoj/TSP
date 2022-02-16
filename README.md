# TSP

U ovome se repozitoriju nalaze sve potrebne datoteke za pokretanje koda i grafičkog sučelja, kao i testni primjeri i naši rezultati testiranja. 
Potrebno je na računalu imati Qt Creator verzije 6.0.1 s osnovnim paketima u instalaciji. 

## Pokretanje koda i opis korištenja

Kod se pokreće unutar Qt Creatora pritiskom na tipku Run(zelena strelica) i tada se otvara grafičko sučelje. Prozor koji se otvara sadrži mogućnost odabira datoteke s tesnim primjerima. 
Za odabir željene datoteke, potrebno je kliknuti na gumb (Load data) i u izborniku datoteka pronaći datoteku koju želite otvoriti. Uz kod, u mapi Data nalaze se test primjeri koje smo mi koristili.
Također, moguće je koristiti i vlastite test primjere, jedino što je bitno da budu spremljene u .csv file tako da je svaka točka prikazana u svojem redu i koordinate su međusobno odvojene zarezom. Prilikom računanja ili učitavanja moguće je proizvoljno skalirati vrijednosti kako bi se gradovi ljepše prikazali na zaslonu, kao i uključiti/isključiti prikazivanje feromonskih tragova.
U grafičkom sučelju moguće je odabrati vrijednosti parametara koje će algoritam koristiti. Algoritam je moguće pokrenuti jednom pritiskom na Solve, tada se kao rezultat prikaže karta gradova i zeleno označeni konačni put. Drugi način pokretanja je koristeći test gumb. Klikom na njega potrebno je odrediti gdje će se spremati testna datoteka, kao i koliko puta će se pozvati rješavanje problema na tom testnom primjeru. 
