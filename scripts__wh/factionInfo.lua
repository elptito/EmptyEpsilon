neutre = FactionInfo():setName("Neutre")
neutre:setGMColor(128, 128, 128)
neutre:setDescription([[Les neutres se distinguent principalement par le fait qu’ils n’ont aucune affiliation forte avec aucune faction.
La plupart des commercants se considerent comme independants, bien que la neutralite est une douce Utopie.]])

imperial = FactionInfo():setName("Imperium")
imperial:setGMColor(255, 255, 255)
imperial:setDescription([[L'Imperium est un empire qui regroupe et defend l’humanite contre les races extraterrestres, les puissances chaotiques et l'heresie.
Il s'etend jusqu'aux confins de la galaxie et regroupe un nombre incalculable de mondes. Il a ete fonde et depuis dirige au 30e millenaire par le Saint Empereur, etre eternel dont le corps se trouve sur le trone d'or de Terra, la planete mere de l'humanite.
L'Imperium regente ainsi l'humanite sous un regime a la fois theocratique et totalitaire, qui maintient l'humanite dans l'obscurantisme et assure une devotion complete au culte de l'Empereur.]])

mechanicum = FactionInfo():setName("Adeptus Mechanicus")
mechanicum:setGMColor(128, 255, 0)
mechanicum:setDescription([[Les Magos du Clerge de Mars, ou Adeptus Mechanicus, sont les adorateurs de l'Omnimessie, le Dieu Machine. En tant que tels, ils ont pour mission sacree la recherche et le developpement de la Technoarcana, ainsi que la decouverte et la protection du savoir ancien remontant au Moyen-Age Technologique. Leurs milliards de Serviteurs etendent leur domination sur les Mondes Forges de toute la Galaxie.
Les membres de l'Adeptus Mechanicus sont donc les detenteurs des sombres et antiques mysteres de la technologie. Grace a leur antique technologie de clonage, les Veneres Ancetres de Mars sont capables d'etendre leurs vies sur plusieurs millenaires. Grace a cela, ces carcasses ratatinees maintiennent leur mainmise sur les secrets technologiques enfouis sous la surface de la planete rouge. Mais si les corps des Hauts Techno-Mages sont maintenus en vie, leurs esprits, eux, continuent de vieillir, les plongeant peu a peu dans la folie a mesure que les siecles passent.
A travers leurs rites mystiques et leurs ceremonies, leurs chants aux phrases entrelacees, leurs huiles et leurs onguents prepares avec soin, les Techno-Pretres benissent les armes et les machines de guerre de l'humanite au nom du Dieu Machine. Mais, a cause de la senilite des Hauts Techno-Mages, cette veneration se transforme parfois en superstition ou en folie, entrainant des sacrifices sanglants.]])

chaos = FactionInfo():setName("Chaos")
chaos:setGMColor(255, 0, 0)
chaos:setEnemy(neutre)
chaos:setEnemy(imperial)
chaos:setDescription([[Les Chaotiques sont d’anciens Imperiaux, corrompus par les puissances chaotiques et qui se sont rebelles contre l'empereur pendant l’Heresie d'Horus. Battus, ils ont cherche refuge pour la plupart dans l'Œil de la terreur, ou ils ont developpe des mutations terrifiantes.
Les membres du Chaos sont generalement plus forts que leur anciens camarades grace a leurs voyages dans le Warp.
Ils vouent depuis l'Heresie d'Horus une haine farouche et sans limites envers tout ce qui appartient a l'Imperium ainsi que tout ce qu'il represente: l'Humanite.]])

eldars = FactionInfo():setName("Eldars")
eldars:setGMColor(255, 128, 0)
eldars:setEnemy(chaos)
eldars:setDescription([[Les Eldars sont une des plus anciennes races de la galaxie et une des plus developpees, bien que mourante. En effet leur cycle de reproduction est bien plus lent que le rythme auquel ils sont tues dans les divers conflits de la galaxie.
Race fiere, elle considere les humains comme un peuple jeune, immature et imbu de lui-meme. Leur reputation de fourberie au sein de l'Imperium vient de leur tendance a la ruse et a l'usage des sous-race (les humains pour ne citer qu'eux) dans leurs conflits. Sans forcement que ces guerriers sachent qu'ils se battent pour les interets Eldars. Leur technologie est meconnue par les technaugures imperiaux, car tres avancee et basee sur des energies etranges, tels les cristaux et d'autres armes redoutables pouvant faire appel a la puissance du Warp.]])

tau = FactionInfo():setName("Tau")
tau:setGMColor(0, 0, 255)
tau:setDescription([[Les Tau forment une civilisation tres jeune (seulement 6 000 ans). D'une peau bleu gris, d'une taille moyenne inferieure a celle des hommes de quelques centimetres, ils ont une allure humanoide mais ont des pieds dotes de sabots et leur esperance de vie est moins longue que les humains. Avant d'etre totalement unie leur espece etait divisee en peuplades, chacune vivant dans des endroits propres.
A cette epoque tous ces peuples se massacraient et l'ensemble de l'espece etait proche de l'extinction. C'est alors qu'apparurent les Etheres qui unifierent la race et qui donnerent a chaque peuplade une place dans la societe en les separant en cinq castes differentes : les constructeurs devinrent la caste de la terre, les diplomates celle de l'eau, les messagers et pilotes celle de l'air, les guerriers des plaines celle du feu, et les Etheres devinrent la cinquieme caste, a la tete de l'empire. Depuis, tous vivent en harmonie et en paix avec la peur de retourner a cette epoque de terreur, d'ou leur combat pour le bien supreme contre les ennemis de l'exterieur. ]])

xenos = FactionInfo():setName("Xenos")
xenos:setGMColor(255, 0, 128)
xenos:setDescription([[Le terme Xeno designe toute creature extra-terrestre du point de vue humain et recouvre par extention toutes les civilisations non-humaines de la Galaxie; connues ou inconnues, existantes ou eteintes.
Les relations avec les races xenos sont strictement interdites aux citoyens imperiaux, etant condamnees et abhorrees par la chaire imperiale avec la meme haine que les sorcieres, les mutants et les demons. Pour l’Imperium, les xenos ne meritent que d’etre extermines pour avoir commis le crime fondamental consistant a vouloir disputer la domination du cosmos a l’humanite.
Apres des millenaires de guerres incessantes et de xenocides, la plupart des especes qui ont survecu au contact avec les hommes ont fini par hair et craindre l’Imperium a leur tour ou, comme les Orks, a se rejouir de ce conflit permanent.]])
