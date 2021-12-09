- ZA TESTIRANJE LEVELA => promijeniti varijablu članicu mLevel u [wanted_level-1] u zaglavlju game.h 
- .EXE FILE => x64/Release ili x64/Debug

[POSSIBLE_BUGS]
= lopta može zauvijek ostati unutar pillar-a (imamo infinite kolizije)
= lopta može (u 3. levelu) zauvijek pogađati ista mjesta i ne spustiti se do player-a
= igrač može dovesti brod do lopte prebrzo i u tom slučaju imamo samo previše sound-ova

[PROMJENE I RJEŠENJA BUGOVA]
= infnite kolizije kod lijevog i desnog pillar-a su rješene tako da više ne rotiram x-coord brzine za 90deg nego ga uvijek postavljam na pozitivnu
	odnosno negativnu vrijednost
= zauvijek pogađanje istih mjesta sam rješio tako što dodam malu slučajno odabranu vrijednost u brzini X-coord [-5,5] pri koliziji sa ciglom pa
	lopta blago mijenja smjer i kad-tad će doći do player-a (funkcija smallRandomShift())
= previše sound-ova kod player-a rješavam na način da pamtim zadnji hitac koji se pojavio i ako nova kolizija rezultira istim
	hicem tada taj sound ne puštam. 

= cigle više ne spremamo u vektor preko pokazivača jer sam shvatio da koristim emplace_back pa nema potrebe za CCtor-om
= zvukove sada spremamo u deque jer se veličina spremnika stalno mijenja, a ne trebam prolaziti po spremniku već brišem front() čim
	zvuk završi