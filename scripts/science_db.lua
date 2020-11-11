sys_vaisseau = ScienceDatabase():setName('Controle du vaisseau')

item = sys_vaisseau:addEntry('Station de Pilotage')
item:setLongDescription([[La station de pilotage permet de diriger le vaisseau dans l'espace. On decide de la direction, de la vitesse et de l'utilisation des reacteurs d'hyper-espace (warp ou jump). C'est egalement avec cette station que l'on peut se docker sur un vaisseau ou une station. Enfin, en combat, la station de pilotage peut utiliser des manoeuvres de combat pour se deplacer plus rapidement.]])

item = sys_vaisseau:addEntry('Station Artilleur')
item:setLongDescription([[La Station Artilleur permet de se defendre, en activant ou desactivant les boucliers, et d'attaquer un ennemi a l'aide de lasers et de missiles. Attention, si le vaisseau dispose d'armes nucleaires, celles-ci sont initialement entreposees dans la soute et doivent d'abord etre activees manuellement (pour eviter tout risque d'utilisation non souhaitee).]])

item = sys_vaisseau:addEntry('Salle des Machines')
item:setLongDescription([[La salle des machines permet de verifier le niveau d'energie du vaisseau et de rendre plus puissant divers systemes du vaisseau, aidant ainsiles actions des autres stations. Rendre plus puissant certains systemes les fait surchauffer, mais le systeme de refroidissement est automatique.

Attention, lorsque l'etat d'un systeme est inferieur a 0%, le systeme n'est pas utilisable, il faut le reparer. C'est egalement dans la salle des machines que peuvent etre decidees les reparations a effectuer.

Liste des systemes (et leur effet dans le vaisseau) :

Reacteur : Le reacteur genere de l'energie et de l'oxygene. Ajouter de la puissance du reacteur augmente votre taux de production d'energie et d'oxygene.

Armes Laser : Ajouter de l'alimentation dans le systeme d'armes a faisceau laser augmente la cadence de tir, ce qui les amene a faire plus de degats.
Notez que chaque tir laser ajoute automatiquement de la chaleur supplementaire au systeme.

Syteme de Missiles : Augmenter la puissance du systeme de missiles reduit le temps de rechargement des tubes de lancement.

Manoeuvre : Augmenter la puissance du systeme de manoeuvre permet au vaisseau de tourner plus vite.

Moteurs sub-luminique: Ajouter de la puissance aux moteurs a impulsion augmente votre vitesse de vol a impulsion.

Jump Drive: Ajouter de la puissance au Jump drive augmente son rechargement.

Boucliers: Ajouter de la puissance aux boucliers augmente leur taux de recharge et diminue la quantite de degradation que vos boucliers subissent lorsqu'ils sont endommages.

Docks : Augmenter de la puissance au docks augmente la vitesse de transfert des drones dans le docker

Scanner : Augmenter la puissance des drones permet d'accroitre la portee de leur controle a distance.
]])

item = sys_vaisseau:addEntry('Station analyste')
item:setLongDescription([[La station analyste represente les yeux du vaisseau. Elle permet d'obtenir des informations sur les objets, stations et vaisseaux gravitant autour du vaisseau. En outre, la station scientifique dispose d'une base de donnees d'informations sur les vaisseaux, les stations et les races aliens. Enfin, la station scientifique dispose de differents outils de reparation et de hacking d'ondes et de frequences.]])

item = sys_vaisseau:addEntry('Station de navigation')
item:setLongDescription([[La station de navigation permet de communiquer avec les autres stations et vaisseaux. Cette station dispose d'un radar a courte portee pour son propre vaisseau et tous les installations alliées. Il est possible de placer des points de navigation et d'envoyer des sondes de balayage pour mieux explorer l'espace environnant. Enfin, cette station permet d'analyser les routes gravitationnelles et de guider le pilotage pour les long voyages.]])

item = sys_vaisseau:addEntry('Station de docks')
item:setLongDescription([[La station de docks permet de gerer les slots destines aux drones de la station. La plupart des slots permettent de stocker des drones, mais certains permettent de lancer les drones (lanceur), de recharger les drones en energie ou de les reparer.

La station de docks permet egalement de gerer l'extraction de minerai effectue par les drones.]])

item = sys_vaisseau:addEntry('Station de drones')
item:setLongDescription([[La station de drones permet de prendre à distance le controle des drones de la station. Il est alors possible de les piloter, sans s'eloigner de trop de la station. Les drones sont capables d'extraire des minerais.]])

item = sys_vaisseau:addEntry('Station de Logs')
item:setLongDescription([[La station de logs permet d'etre informer des informations internes et externes au vaisseau. Le vaisseau dispose d'un log interne, qui reference l'etat du vaisseau, l'oxygene restant, et diverses informations internes au vaisseau. Le vaisseau dispose egalement d'un log externe, qui reference les contacts avec l'exterieur ainsi que la distance vers les principales destinations possibles du vaisseau.

Le log interne est egalement disponible dans la salle des machines (cliquez sur le bandeau du bas pour l'afficher ou le reduire).
Le log externe est egalement disponible dans la station de relai (cliquez sur le bandeau du bas pour l'afficher ou le reduire).
]])


weapons = ScienceDatabase():setName('Armes')

item = weapons:addEntry('Laser')
item:addKeyValue('Portee', 'variable')
item:addKeyValue('Degats', 'variable')
item:setLongDescription([[Le tir laser est l'arme standard des vaisseaux spatiaux de combat. Les tirs lasers ont l'avanage d'etre tele-guides, l'artilleur n'a besoin que de selectionner sa cible.

Les boucliers peuvent etre traverses en utilisant une frequence adequate. De meme, il est possible de configurer les lasers pour affecter des systemes particuliers de la cible.
]])

item = weapons:addEntry('TCC')
item:addKeyValue('Portee', '5.4u')
item:addKeyValue('Degats', '35')
item:setLongDescription([[La tete chercheuse conventionnelle est l'arme standard des vaisseaux spatiaux de combat. Bien plus efficace que le lanceur TBHV, elle prend peu de place et provoque des degats importants sur les carlingues. Elle est par contre nettement moins efficace contre des boucliers.
]])

item = weapons:addEntry('TCN')
item:addKeyValue('Portee', '5.4u')
item:addKeyValue("Rayon d'effet", '1u')
item:addKeyValue('Degats au centre', '160')
item:addKeyValue('Degats au bord', '30')
item:setLongDescription([[La tete chercheuse nucleaire est similaire a la tete chercheuse conventionnelle, sauf qu'elle se deplace plus lentement, tourne plus difficilement, mais provoque des degats bien plus importants. L'explosion nucleaire atteint notamment les autres vaisseaux spatiaux dans un rayon de 1U autour du point d'impact, pouvant ainsi faire de multiples victimes avec un seul tir.

Certains capitaines sont opposes a l'utilisation de cette arme, du fait des trop larges explosions qu'elle peut provoquer, notamment avec un grand risque de tir ami.

Les boucliers protegent les equipages des radiations nucleaires suite a une explosion. Ne pas activer les boucliers en cas d'explosion nucleaire est une erreur souvent fatale.
]])

item = weapons:addEntry('Mine')
item:addKeyValue("Dist. d'envoi", '1.2u')
item:addKeyValue('Declanchement', '0.6u')
item:addKeyValue("Rayon d'effet", '1u')
item:addKeyValue('Degats au centre', '160')
item:addKeyValue('Degats au bord', '30')
item:setLongDescription([[Les Mines sont souvent une arme defensive placees pour proteger une station ou une zone de l'espace. Il existe egalement des champs d'asteroides, vestige des anciennes guerres martiennes.

Certains capitaines courageux utilisent les mines comme armes offensives, mais la faible portee, l'absence de visee et le rayon d'effet rend cet usage tres complexe.
]])

item = weapons:addEntry('TCI')
item:addKeyValue('Portee', '5.4u')
item:addKeyValue("Rayon d'effet", '1u')
item:addKeyValue('Degats au centre', '160')
item:addKeyValue('Degats au bord', '30')
item:setLongDescription([[
Une tete chercheuse a impulsion electromagnetique est une emission d'ondes electromagnetiques breve et de tres forte amplitude qui peut detruire de nombreux appareils electriques et electroniques (relies au courant et non-proteges) et brouiller les telecommunications.

Une TCI affecte les boucliers d'energie des vaisseaux spatiaux dans un rayon de 5U autour du point d'impact, laissant la carlingue intacte. De ce fait, de nombreux capitaines les preferent aux armes nucleaires car elles peuvent etre utilises sans detruire la cible.

Il existe deux types de bombes creant des TCI. La plus simple est l'UWB a bande ultra large qui tire profit d,un explosif generant un champ magnetique, generalement une bombe H, la seconde est la HPM qui utilise un generateur de micro-ondes de haute puissance.
]])

item = weapons:addEntry('TBHV')
item:addKeyValue('Portee', '5.4u')
item:addKeyValue('Degats', '7 chaque, 35 total')
item:addKeyValue('Morceaux', '5')
item:setLongDescription([[Un lanceur TBHV (tete balistique a haute velocite) projete des morceaux de metal a forte densite a tres forte vitesse. Cette arme est tres simple et est disponible sur la plupart des vaisseaux. Utiliser un lanceur HVLI n'est pas tres complexe : l'arme n'est pas tele-guidee et ne peut tirer qu'en ligne droite.

Chaque tir d'un lanceur HVLI envoit 5 projectiles, afin de maximiser les chances de succes.
]])

space_objects = ScienceDatabase():setName('Elements naturels')
item = space_objects:addEntry('Asteroide')
item:setLongDescription([[
Un asteroide est une planete mineure qui est composee de roches, de metaux et de glaces, et dont les dimensions varient de l'ordre du metre (limite actuelle de detection) a plusieurs centaines de kilometres.

Lorsque leur diametre avoisine ou depasse 1 000 km, ce qui est rarissime, leur forme peut etre spherique et ils peuvent prendre alors egalement le statut de planete naine. C'est le cas de Ceres. Dans le cas contraire, ils ont generalement une forme irreguliere et font partie de la classe des petits corps.

Les asteroides (appeles meteoroides pour les plus petits) penetrant dans l'atmosphere d'un autre corps creent des meteores. Si une partie de l'asteroide reussit a toucher la surface du corps impacte, elle forme alors une meteorite et peut creer un cratere d'impact.

On sait a present que les asteroides sont des restes du disque protoplanetaire qui ne se sont pas regroupes en planetes pendant sa formation.
]])

item = space_objects:addEntry('Nebuleuse')
item:setLongDescription([[
Une nebuleuse (du latin nebula, nuage) designe, en astronomie, un objet celeste compose de gaz rarefie, ionise et/ou de poussieres interstellaires. Etudiees par des astrophysiciens specialises dans l'etude du milieu interstellaire, les nebuleuses jouent un role cle dans la formation des etoiles. Les nebuleuses peuvent former des systemes d'etoiles en s'effondrant gravitationnellement. Ainsi, le systeme solaire se serait forme a partir d'une nebuleuse solaire. 
les nebuleuses peuvent affecter la composition des asteroides les traversant, du fait du gaz ionise qu'elles peuvent contenir.
]])

item = space_objects:addEntry('Trou noir')
item:setLongDescription([[
En astrophysique, un trou noir est un objet celeste si compact que l'intensite de son champ gravitationnel empeche toute forme de matiere ou de rayonnement de s'en echapper. De tels objets ne peuvent ni emettre, ni reflechir la lumiere et sont donc noirs, ce qui en astronomie revient a dire qu'ils sont invisibles. Toutefois, plusieurs techniques d'observation indirecte dans differentes longueurs d'ondes ont ete mises au point et permettent d'etudier les phenomenes qu'ils induisent. En particulier, la matiere happee par un trou noir est chauffee a des temperatures considerables avant d'etre « engloutie » et emet une quantite importante de rayons X. Envisagee des le xviiie siecle, dans le cadre de la mecanique classique, leur existence — predite par la relativite generale — est une certitude pour la quasi-totalite des astrophysiciens et des physiciens theoriciens.

Dans le cadre de la relativite generale, un trou noir est defini comme une singularite gravitationnelle occultee par un horizon absolu appele horizon des evenements. Selon la physique quantique, un trou noir est susceptible de s'evaporer par l'emission d'un rayonnement de corps noir appele rayonnement de Hawking.
]])

item = space_objects:addEntry('Trou de ver')
item:setLongDescription([[
Un trou de ver est, en physique, un objet hypothetique qui relierait deux feuillets distincts ou deux regions distinctes de l'espace-temps et se manifesterait, d'un cote, comme un trou noir et, de l'autre cote, comme un trou blanc.

Un trou de ver formerait un raccourci a travers l'espace-temps. Pour le representer plus simplement, on peut se representer l'espace-temps non en quatre dimensions mais en deux dimensions, a la maniere d'un tapis ou d'une feuille de papier. La surface de cette feuille serait pliee sur elle-meme dans un espace a trois dimensions.

L'utilisation du raccourci « trou de ver » permettrait un voyage du point A directement au point B en un temps considerablement reduit par rapport au temps qu'il faudrait pour parcourir la distance separant ces deux points de maniere lineaire, a la surface de la feuille. Visuellement, il faut s'imaginer voyager non pas a la surface de la feuille de papier, mais a travers le trou de ver ; la feuille etant repliee sur elle-meme permet au point A de toucher directement le point B. La rencontre des deux points serait le trou de ver.
]])

item = space_objects:addEntry('Cometes')
item:setLongDescription([[Les cometes sont de petits corps celestes, generalement de quelques kilometres de diametre, principalement composes de glaces volatiles. Elles possedent des orbites hautement excentriques, avec un perihelie souvent situe dans le Systeme solaire interne et un aphelie au-dela de Pluton. Lorsqu'une comete passe pres d'une etoile, la proximite de celle-ci provoque la sublimation et l'ionisation de sa surface, creant une queue : une longue trainee de gaz et de poussiere.
]])


minerai = ScienceDatabase():setName('Ressources')
item = minerai:addEntry('Generalites')
item:setLongDescription([[Differents Ressources peuvent etre extraites des asteroides. Cela permet de fournir de grande quantite de ressources naturelles et rend les stations orbitales totalement autonome sur cet aspect.

Des Ressources sont disponibles dans des quantites differentes selon leur type. Certaines sont egalement plus rares, d'autres plus complexes a reperer par scanner. Enfin, les asteroides presents dans des nebuleuses ont une tendance a generer des Ressources plus rare, du fait des radiations et du gaz ionise contenus dans les nebuleuses.

Une fois scanne par la station scientifique, les Ressources peuvent etre extraites par un drone si celui-ci se situe a moins de 1U de distance. De maniere generale, les drones peuvent contenir 10 unites de ressources.
]])

item = minerai:addEntry('Fer')
item:addKeyValue('Type', 'Metaux')
item:addKeyValue('Probabilite', '10%')
item:addKeyValue('Probabilite (nebuleuse)', '3%')
item:setLongDescription([[Le fer est l'element chimique de numero atomique 26, de symbole Fe.

Le corps simple est le metal et le materiau ferromagnetique le plus courant dans la vie quotidienne, le plus souvent sous forme d'alliages divers. Le fer pur est un metal de transition ductile, mais l'adjonction de tres faibles quantites d'elements d'additions modifie considerablement ses proprietes mecaniques. Allie au carbone et avec d'autres elements d'additions il forme les aciers, dont la sensibilite aux traitements thermomecaniques permet de diversifier encore plus les proprietes du materiau.
]])

item = minerai:addEntry('Cuivre')
item:addKeyValue('Type', 'Metaux')
item:addKeyValue('Probabilite', '6%')
item:addKeyValue('Probabilite (nebuleuse)', '8%')
item:setLongDescription([[
Le cuivre est l'element chimique de numero atomique 29, de symbole Cu. Le corps simple cuivre est un metal. Le cuivre est un element du groupe 11, de la periode 4, un element du bloc d metal de transition chalcophile.

Il possede une orbitale s occupee par un seul electron sur des sous-couches p et d totalement remplies, ce qui permet la formation de liaisons metalliques (configuration electronique Ar 3d10 4s). Le Cuivre ont un caractere de noblesse et de rarete accru, le premier caractere s'expliquant par leurs rayons atomiques faibles et leurs compacites d'empilement atomique, leurs potentiels d'ionisation plus importants à cause des sous-couches d, leurs points de fusion relativement eleves et leurs faibles reactivites ou relatives inerties chimiques.
]])

item = minerai:addEntry('Titane')
item:addKeyValue('Type', 'Metaux')
item:addKeyValue('Probabilite', '1.5%')
item:addKeyValue('Probabilite (nebuleuse)', '6%')
item:setLongDescription([[
Le titane est l'element chimique de numero atomique 22, de symbole Ti.

C'est un metal de transition leger, resistant, d'un aspect blanc metallique, qui resiste à la corrosion. Le titane est principalement utilise dans les alliages legers et resistants, et son oxyde est utilise comme pigment blanc. On trouve cet element dans de nombreux minerais mais ses principales sources sont le rutile et l'anatase. Il appartient au groupe des titanes avec le zirconium (Zr), le hafnium (Hf) et le rutherfordium (Rf).

Les proprietes industriellement interessantes du titane sont sa resistance à la corrosion, souvent associee à la resistance à l'erosion et au feu, la biocompatibilite, mais aussi ses proprietes mecaniques (resistance, ductilite, fatigue, etc.) qui permettent notamment de façonner des pieces fines et legeres.
]])

item = minerai:addEntry('Carbone')
item:addKeyValue('Type', 'Organique')
item:addKeyValue('Probabilite', '10%')
item:addKeyValue('Probabilite (nebuleuse)', '3%')
item:setLongDescription([[
Le carbone est l'element chimique de numero atomique 6 et de symbole C. Il possede trois isotopes naturels : 12C, 13C et 14C ; les deux premiers sont stables tandis que le troisieme est radioactif de demi-vie 5 730 annees luxiennes.

Le carbone est l'element le plus leger du groupe 14 du tableau periodique. Le corps simple carbone presente plusieurs formes allotropiques dont principalement le graphite et le diamant. L'element carbone forme divers composes inorganiques comme le dioxyde de carbone CO2, et une grande variete de composes organiques et de polymeres. C'est l'element de base de toutes les formes de vie connues.

Le carbone est le 4e element le plus abondant dans la galaxie. De nombreuses structures basees sur le carbone ont egalement ete synthetisees : charbon actif, noir de carbone, fibres, nanotubes, fullerenes et graphene.

Les materiaux à base de carbone ont des applications dans de nombreux autres domaines : materiaux composites, batteries lithium-ion, depollution de l'air et de l'eau, electrodes pour les fours à arc ou la synthese de l'aluminium, etc
]])

item = minerai:addEntry('Azote')
item:addKeyValue('Type', 'Organique')
item:addKeyValue('Probabilite', '10%')
item:addKeyValue('Probabilite (nebuleuse)', '3%')
item:setLongDescription([[
L'azote est l'element chimique de numero atomique 7, de symbole N (du latin nitrogenium). C'est la tete de file du groupe des pnictogenes. Dans le langage courant, l'azote designe le corps simple N2 (diazote).

Les mineraux contenant de l'azote sont essentiellement les nitrates, notamment le nitrate de potassium KNO3 (constituant du salpetre) ou nitre, qui servait autrefois à faire des poudres explosives, et le nitrate de sodium NaNO3 (constituant du salpetre du Chili).

L'azote a de nombreux usages industriels. Il est notamment massivement employe comme engrais (sous forme de composes d'ammonium), au point que c'est aujourd'hui la cause de pollution pour de nombreuses planetes à vocation agricoles.
]])

item = minerai:addEntry('Calcium')
item:addKeyValue('Type', 'Organique')
item:addKeyValue('Probabilite', '6%')
item:addKeyValue('Probabilite (nebuleuse)', '8%')
item:setLongDescription([[
Le calcium est l'element chimique de numero atomique 20, de symbole Ca.

C'est un metal alcalino-terreux gris-blanc et assez dur qui ne se trouve jamais à l'etat de corps pur dans la nature. Il est essentiel pour la matiere organique : formation des os, des dents et des coquilles. Le calcium joue egalement un role tres important en physiologie cellulaire.
]])

item = minerai:addEntry('Sodium')
item:addKeyValue('Type', 'Chimique')
item:addKeyValue('Probabilite', '10%')
item:addKeyValue('Probabilite (nebuleuse)', '3%')
item:setLongDescription([[
Le sodium est l'element chimique de numero atomique 11, de symbole Na (du latin natrium). Le corps simple sodium est un metal mou, de couleur argentee et tres reactif, qui fait partie des metaux alcalins. On ne rencontre pratiquement pas le sodium sous cette forme dans la nature, mais il est tres abondant sous la forme de composes chimiques.

Le sodium possede 22 isotopes connus, avec un nombre de masse variant entre 18 et 37. Seul le sodium 23 (23Na) est stable, ce qui fait du sodium un element monoisotopique. À part 22Na et 24Na, isotopes radioactifs cosmogeniques de demi-vies 2,604 ans et 14,96 heures, les radioisotopes du sodium ont tous une demi-vie inferieure à une minute, voire une seconde pour la majorite d'entre eux. En pratique, seul 23Na est trouve dans la nature et le sodium est donc considere comme un element mononucleidique.
]])

item = minerai:addEntry('Silicium')
item:addKeyValue('Type', 'Chimique')
item:addKeyValue('Probabilite', '6%')
item:addKeyValue('Probabilite (nebuleuse)', '8%')
item:setLongDescription([[Le silicium (SiO2) entre dans la composition de nombreux mineraux.

La silice existe a l'etat libre sous differentes formes cristallines ou amorphes et a l'etat combine dans les silicates, les groupes SiO2 etant alors lies a d'autres atomes (Al : Aluminium, Fe : Fer, Mg : Magnesium, Ca : Calcium, Na : Sodium, K : Potassium...).

Les silicates sont les constituants principaux du manteau et de l'ecorce terrestre. La silice libre est egalement tres abondante dans la nature, sous forme de quartz, de calcedoine et de terre de diatomee. La silice represente 60,6 % de la masse de la croûte terrestre continentale.

La silice pure se presente sous la forme d'un mineral dur et existe sous des formes variees dans la nature.
]])

item = minerai:addEntry('Souffre')
item:addKeyValue('Type', 'Chimique')
item:addKeyValue('Probabilite', '1.5%')
item:addKeyValue('Probabilite (nebuleuse)', '6%')
item:setLongDescription([[
]])

item = minerai:addEntry('Hydrogene')
item:addKeyValue('Type', 'Gaz')
item:addKeyValue('Probabilite', '10%')
item:addKeyValue('Probabilite (nebuleuse)', '3%')
item:setLongDescription([[
L'hydrogene est l'element chimique de numero atomique 1, de symbole H. L'hydrogene present majoritairement sur les planetes est presque entierement constitue de l'isotope stable 1H (un proton, zero neutron), mais comporte environ 0,01 % de 2H (un proton, un neutron), stable egalement. Un troisieme isotope 3H (un proton, deux neutrons), instable, est produit dans les explosions nucleaires. Ces trois isotopes sont respectivement appeles "protium", "deuterium" et "tritium".

L'hydrogene peut avoir les nombres d'oxydation 0 (dihydrogene H2 ou hydrogene metallique), +I (dans la plupart de ses composes chimiques) et -I (dans les hydrures metalliques). L'hydrogene est un element electropositif, frequemment ionise à l'etat H+ ou H3O+. Mais il forme aussi des liaisons covalentes, notamment dans l'eau et la matiere organique.

L'hydrogene est le principal constituant de la plupart des etoiles (dont l'energie provient de la fusion thermonucleaire de cet hydrogene), et de la matiere interstellaire ou intergalactique. C'est un composant majeur des planetes geantes, sous forme metallique ou sous la forme de dihydrogene solide.
]])

item = minerai:addEntry('Oxygene')
item:addKeyValue('Type', 'Gaz')
item:addKeyValue('Probabilite', '6%')
item:addKeyValue('Probabilite (nebuleuse)', '8%')
item:setLongDescription([[
L'oxygene est l'element chimique de numero atomique 8, de symbole O. C'est la tete de file du groupe des chalcogenes, souvent appele groupe de l'oxygene.

Une molecule de formule chimique O2, appelee communement "oxygene" et, par les chimistes, dioxygene est constituee de deux atomes d'oxygene relies par liaison covalente : aux conditions normales de temperature et de pression, le dioxygene est un gaz, qui constitue 20,8 % du volume de l'atmosphere terrestre au niveau de la mer.

L'oxygene est un non-metal qui forme tres facilement des composes, notamment des oxydes, avec pratiquement tous les autres elements chimiques. Cette facilite se traduit par des energies de formation elevees mais, cinetiquement, le dioxygene est souvent peu reactif à temperature ambiante. Ainsi un melange de dioxygene et de dihydrogene, de fer ou de soufre, etc., n'evolue qu'extremement lentement.
]])

item = minerai:addEntry('Xenon')
item:addKeyValue('Type', 'Gaz')
item:addKeyValue('Probabilite', '1.5%')
item:addKeyValue('Probabilite (nebuleuse)', '6%')
item:setLongDescription([[
Le xenon est l'element chimique de numero atomique 54, de symbole Xe. C'est un gaz noble, inodore et incolore. Dans une lampe à decharge, il emet une lumiere bleue. Le xenon est le plus rare et le plus cher des gaz nobles.

On extrait le xenon par distillation de l'air. Pour distiller de l'air, il faut le rendre liquide en le compressant (il s'echauffe en restant gazeux, mais en le maintenant compresse et en le refroidissant, il se liquefie). On peut alors extraire le xenon par distillation fractionnee de l'air devenu liquide.
]])

item = minerai:addEntry('Neodyme')
item:addKeyValue('Type', 'Actifs')
item:addKeyValue('Probabilite', '6%')
item:addKeyValue('Probabilite (nebuleuse)', '8%')
item:setLongDescription([[
Le neodyme est un element chimique, de symbole Nd et de numero atomique 60. C'est un metal gris argent du groupe des terres rares. Il fait partie de la famille des lanthanides. À temperature ambiante, il est ductile, malleable et s'oxyde rapidement à l'air.
]])

item = minerai:addEntry('Plutonium')
item:addKeyValue('Type', 'Actifs')
item:addKeyValue('Probabilite', '1.5%')
item:addKeyValue('Probabilite (nebuleuse)', '6%')
item:setLongDescription([[
Le plutonium est l'element chimique de symbole Pu et de numero atomique 94. C'est un metal radioactif transuranien de la famille des actinides. Il se presente sous la forme d'un solide cristallise dont les surfaces fraiches sont gris argente mais se couvrent en quelques minutes, en presence d'humidite, d'une couche terne de couleur grise, tirant parfois sur le vert olive, constituee d'oxydes et d'hydrures ; l'accroissement de volume qui en resulte peut atteindre 70 % d'un bloc de plutonium pur, et la substance ainsi formee tend à se desagreger en une poudre pyrophorique.

On connait pas moins de six allotropes du plutonium à pression atmospherique, et un septieme au-delà de 60 MPa. Ils ont des proprietes bien tranchees et souvent inhabituelles pour un metal. Ainsi, le plutonium alpha, stable à temperature ambiante, est l'un des tres rares metaux à cristalliser dans le systeme monoclinique ; ses proprietes physiques et structurelles relevent davantage des mineraux que des metaux usuels, tandis que ses proprietes mecaniques rappellent celles de la fonte. Le plutonium delta, stable à temperature plus elevee ou avec une faible fraction molaire de gallium, cristallise en revanche dans une maille cubique à faces centrees, avec une masse volumique inferieure de pres de 20 % à celle du plutonium alpha ; il est davantage metallique, avec des proprietes mecaniques semblables à l'aluminium, mais un coefficient de dilatation thermique negatif (il se contracte en chauffant). Le plutonium est egalement l'un des rares elements chimiques dont le liquide est plus dense que le solide au point de fusion. L'existence de multiples allotropes aux energies internes voisines rend la mise en forme du plutonium particulierement delicate, au point qu'on utilise plutot un alliage plutonium-gallium, qui stabilise la phase δ à temperature ambiante, ce qui facilite l'usinage des pieces en plutonium.

Le plutonium 239 et le plutonium 241 sont des isotopes fissiles par neutrons thermiques, ce qui signifie qu'ils peuvent contribuer à une reaction nucleaire en chaine et qu'ils peuvent etre utilises dans la conception d'armes nucleaires et de reacteurs nucleaires. Le plutonium 240 presente un taux de fission spontanee tres eleve qui impose d'en maintenir un taux inferieur à 7 % dans le plutonium de qualite militaire. Le plutonium 238 a une demi-vie de 88 ans et emet des particules alpha ; c'est une source de chaleur souvent utilisee par les generateurs thermoelectriques à radioisotopes pour alimenter certaines sondes spatiales en electricite. La separation des isotopes du plutonium est difficile et ils sont generalement produits specifiquement par des reacteurs specialises.
]])

item = minerai:addEntry('Uranium')
item:addKeyValue('Type', 'Actifs')
item:addKeyValue('Probabilite', '6%')
item:addKeyValue('Probabilite (nebuleuse)', '8%')
item:setLongDescription([[
L'uranium est un element chimique de symbole U et de numero atomique 92, de la famille des actinides.

C'est un metal lourd radioactif (emetteur alpha) de periode tres longue (~ 4,4688 milliards d'annees pour l'uranium 238 et ~ 703,8 millions pour l'uranium 235). Sa radioactivite, additionnee à celle de ses descendants dans sa chaine de desintegration, developpe une puissance de 0,082 watt par tonne d'uranium, ce qui en fait, avec le thorium 232 (quatre fois plus abondant, mais trois fois moins radioactif) et le potassium 40, la principale source de chaleur qui tend à maintenir les hautes temperatures du manteau terrestre, en ralentissant de beaucoup son refroidissement.

L'isotope 235U est le seul isotope fissile naturel. Sa fission libere une energie voisine de 202,8 MeV par atome fissionne dont 9,6 MeV d'energie non recuperable, communiquee aux neutrinos produits lors de la fission. L'energie recuperable est plus d'un million de fois superieure à celle des combustibles fossiles pour une masse equivalente.
]])

physics_principle = ScienceDatabase():setName('Principes physiques')
item = physics_principle:addEntry('Jump')
item:setLongDescription([[
L'hyperespace (ou JUMP) est un ensemble de conditions physiques qui, quand elles sont rassemblees, autorisent un objet a sauter d'un point a un autre de l'espace, sans parcourir le trajet intermediaire. On peut obtenir ces conditions en utilisant un propulseur specifique. Dans les conditions de saut hyperspatial, le vaisseau se trouve a egale distance de tous les points de l'univers. Il ne subit donc pas d'acceleration car il passe d'un point a un autre sans s'etre deplace. Le vaisseau disparait pour reapparaitre a un autre point instantanement et sans vitesse relative.

Le JUMP est soumis a d'importantes restrictions ; son utilisation est notamment tres sensible a la gravitation, les differents corps celestes et leurs champs gravitationnels perturbant grandement les sauts via l'hyperespace. Les vaisseaux doivent donc sauter d'un point eloigne de tout corps vers un autre point relativement eloigne de tout corps celeste, ce qui implique des longs trajets a des vitesses inferieures a celle de la lumiere avant qu'un saut ne soit possible avec une securite et une precision suffisantes.

Le JUMP est particulierement gourmant en energie et en espace de stockage pour l'installation du systeme. De ce fait, tous les vaisseaux ne peuvent etre equipes de systeme de JUMP.
]])

-- La duree maximale atteinte avec un systeme JUMP est actuellement de 500U, avec de forts dommages mentaux et physiques pour les scientifiques ayant tente l'experience.

item = physics_principle:addEntry('Gravitation')
item:setLongDescription([[
La gravitation, l'une des quatre forces fondamentales qui regissent l'Univers, est l'interaction physique responsable de l'attraction des corps massifs entre eux. Elle se manifeste notamment par l'attraction terrestre qui nous retient au sol, la gravite, qui est responsable de plusieurs manifestations naturelles ; les marees, l'orbite des planetes autour du Soleil, la sphericite de la plupart des corps celestes en sont quelques exemples. D'une maniere plus generale, la structure a grande echelle de l'univers est determinee par la gravitation.

Plusieurs theories ont tente de rendre compte de la gravitation. Aujourd'hui encore, la theorie de la relativite generale d'Albert Einstein reste la plus satisfaisante. Pour rendre les mesures experimentales compatibles avec les previsions theoriques, des observations en cosmologie ont amene a postuler des hypotheses supplementaires : l'existence d'une forme d'energie sombre, constituant environ 68% de la densite d'energie totale de l'univers, et d'une matiere noire « froide ». La loi de la gravitation de Newton, elaboree a la fin du xviie siecle, demeure cependant une excellente approximation dans les cas non relativistes (vitesses faibles par rapport a celle de la lumiere et masses de l'ordre de la masse solaire).

Aux echelles microscopiques, la gravitation est la plus faible des quatre interactions fondamentales de la physique ; elle devient dominante au fur et a mesure que les echelles de grandeur augmentent. Avec la force electromagnetique, elle est l'une des forces a agir au-dela de la dimension du noyau atomique. De plus, comme elle est toujours attractive, elle domine sur les forces electromagnetiques qui l'emportent a plus courte portee, etant tantot attractives, tantot repulsives.
]])

item = physics_principle:addEntry('Electromagnetisme')
item:setLongDescription([[
L'electromagnetisme est la branche de la physique qui etudie les interactions entre particules chargees electriquement, qu'elles soient au repos ou en mouvement, et plus generalement les effets de l'electricite, en utilisant la notion de champ electromagnetique. Il est d'ailleurs possible de definir l'electromagnetisme comme l'etude du champ electromagnetique et de son interaction avec les particules chargees.

L'electromagnetisme est, avec la mecanique, une des grandes branches de la physique dont le domaine d'application est considerable. Ainsi, outre l'electricite, l'electromagnetisme permet de comprendre l'existence des ondes electromagnetiques, c'est-a-dire aussi bien les ondes radio que la lumiere, ou encore les micro-ondes et le rayonnement gamma. De ce point de vue, l'optique tout entiere peut etre vue comme une application de l'electromagnetisme. L'interaction electromagnetique est egalement une des quatre interactions fondamentales qui permet de comprendre (avec la mecanique quantique) l'existence, la cohesion et la stabilite des edifices chimiques tels que les atomes ou les molecules, des plus simples aux plus complexes.

Du point de vue de la physique fondamentale, le developpement theorique de l'electromagnetisme classique est a la source de la theorie de la relativite restreinte au debut du xxe siecle. La necessite de concilier theorie electromagnetique et mecanique quantique a conduit a construire l'electrodynamique quantique, qui interprete l'interaction electromagnetique comme un echange de particules appelees photons. En physique des particules, l'interaction electromagnetique et l'interaction faible sont unifiees dans le cadre de la theorie electrofaible.
]])

item = physics_principle:addEntry('Interaction faible')
item:setLongDescription([[L'interaction faible (aussi appelee force faible et parfois force nucleaire faible) est l'une des quatre interactions fondamentales de la nature, les trois autres etant les interactions electromagnetique, nucleaire forte et gravitationnelle. Elle est responsable de la desintegration radioactive de particules subatomiques et est a l'origine de la fusion nucleaire dans les etoiles. Elle affecte toutes les categories de fermions connues, a commencer par les electrons, les quarks et les neutrinos.

Dans le modele standard de la physique des particules, l'interaction faible est causee par l'echange de bosons W+, W- et Z0. L'effet le plus connu en est la radioactivite ß. La plupart des particules sont sujettes a la desintegration causee par l'interaction faible. Les bosons W et Z ont une masse tres elevee, ce qui explique qu'elle a une portee tres courte. Par ailleurs, son intensite est generalement plus faible de plusieurs ordres de magnitude que celles des interactions electromagnetique et forte, ce qui explique son nom. L'interaction faible a plusieurs proprietes uniques, parmi lesquelles sa capacite a changer la saveur des quarks et a briser la symetrie de parite et la symetrie CP.

La radioactivite ß est a l'origine de la nucleosynthese dans les etoiles. C'est elle qui rend possible la datation par le carbone 14, en transformant le carbone 14 en azote 14. Elle est aussi a l'origine de la radioluminescence, utilisee dans l'illumination au tritium et dans les generateurs betavoltaiques.]])

item = physics_principle:addEntry('Interaction forte')
item:setLongDescription([[L'interaction forte, ou force forte, appelee parfois force de couleur, est l'une des trois interactions du modele standard aux cotes de l'interaction electromagnetique et l'interaction faible. La toute premiere des interactions observees dans l'histoire, l'interaction gravitationnelle, ajoute sa contribution dans la recherche d'une theorie du tout rassemblent les quatre interactions fondamentales.

L'interaction forte agit sur les particules porteuses d'une charge de couleur : quarks, antiquarks et gluons, ces derniers etant les porteurs de l'interaction elle-meme (de la meme façon que la force electromagnetique est portee par les photons). En assurant une neutralite de la couleur, cette force maintient les quarks ensemble pour former les hadrons. Selon le type des quarks composant un hadron, on parlera de baryon (trois quarks) ou de meson (paires quark/antiquark).

Un effet derive de la force forte est responsable de la cohesion des nucleons (protons et neutrons) au sein du noyau de l'atome, la force nucleaire. Un autre effet derive est la cohesion meme du noyau atomique, la liaison nucleaire.]])

item = physics_principle:addEntry('Signature radar')
item:setLongDescription([[
La signature est l'ensemble des elements qui permettent de caracteriser la presence, le type, ou l'identite d'un objet par un capteur.

Elle depend donc des phenomenes physiques utilises (ondes electromagnetiques - hertziennes, visibles, infra-rouge, etc.- ou mecaniques -acoustiques, etc.), du type de traitement effectue (imagerie 1-D, 2-D ou 3-D, analyse en bande large ou en bande etroite, extraction automatique, etc.) et du principe de fonctionnement du capteur (actif -illuminant la cible et analysant le signal reflechi ou reemis-, passif -utilisant une illumination naturelle ou un signal emis spontanement- ou interception -espionnant le signal emis par un capteur de la cible).

La plupart des vaisseaux spatiaux actuels sont actuellement equipes de trois capteurs radar : rouge, vert et blue. Ces trois capteurs permettent de reperer trois phenomenes a travers de longues distances :

- La signature biologique peut etre captee via le capteur vert

- La signature energetique peut etre captee via le capteur rouge

- La signature gravitationnelle peut etre captee via le capteur bleu.
]])


