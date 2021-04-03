Національний технічний університет України
«Київський політехнічний інститут»





Лабораторна робота №3
з дисципліни “Проектування складних систем”








Виконала:
студентка 4-го курсу
групи ІП-73
Думінська Д.А.
Перевірив:
Долголенко О.М.











Київ 2020







Обґрунтування вибору ємності та організації оперативної пам’яті, ємності та типу вінчестера для робочих станцій. 

Обраний процесор: Intel Pentium Gold G5420
Обрана материнська плата: Gigabyte Z390 M
Обрана ємність та організація оперативної пам’яті: 
Kingston HyperX Predator Black 32GB (4x8GB) 2400MHz DDR4 CL12 XMP
Обрана ємність та тип вінчестеру
SSD 2.5" 128GB Patriot
1.	Чим відрізняється трьохканальний режим роботи оперативної пам’яті від двоканального, одноканального?
В подвійних та потрійних архітектурах збільшено кількість проводів даних, доступних в шині пам'яті, таким чином збільшено і теоретичну пропускну здатність відповідно вдвічі або втричі. Принцип роботи двоканального і трьохканального режиму пам'яті полягає в використанні відповідної кількості каналів для об'єднаного доступу до банку пам'яті. 
У системах, що підтримують двоканальну технологію, шина даних пам'яті розширена з 64 до 128 біт. Це означає, що є 128 проводів, що з'єднують контролер пам'яті з cамою пам'яттю, їх позначають від D0 до D127. Оскільки кожен модуль пам'яті може приймати лише 64 біти одночасно, два модулі пам'яті використовуються для заповнення 128-бітної шини даних (зображено на рисунку). Оскільки доступ до двох модулів здійснюється одночасно, вони повинні бути однаковими (ємність, тактова частота). Аналогічно з трьохканальною архітектурою.
 

2.	Як організувати трьохканальний (двоканальний) режим роботи оперативної пам’яті в робочих станціях?
Для досягнення двоканального режиму необхідно виконувати такі умови:
1.	Однаковий обсяг пам’яті. Приклади: 1 ГБ, 2 ГБ, 4 ГБ.
2.	Розташувати DIMM у симетричних слотах пам'яті.
3.	Процесор, що підтримує двоканальну архітектуру (правда для більшості сучасних процесорів)
Не потрібне виконання наступних умов:
●	Однакова марка
●	Однакова швидкість (МГц)
Найповільніший модуль DIMM в системі, визначає швидкість каналу пам'яті.
 
 
Триканальний режим роботи пам’яті вмикається, коли в кожному з трьох синіх слотів пам'яті встановлені однакові модулі пам'яті. Якщо лише два сині слоти пам'яті заповнені відповідними модулями DIMM, вмикається двоканальний режим.
 
3.	Яка ієрархія пам’яті в робочих станціях?
У більшості сучасних ПК використовується наступна ієрархія пам'яті:
●	Регістри процесора, організовані в регістровий файл 
●	Кеш процесора 1го рівня (L1) - час доступу порядку декількох тактів, розміром в десятки кілобайт
●	Кеш процесора 2го рівня (L2) - більший час доступу (від 2 до 10 разів повільніше L1), близько пів мегабайта або більше
●	Кеш процесора 3го рівня (L3) - час доступу близько сотні тактів, розміром від кількох мегабайт до сотень
●	Кеш процесора 4го рівня (L4) - час доступу кілька сотень тактів, розміром одну-кілька сотень мегабайт. 
●	ОЗУ системи - час доступу від сотень до, можливо, тисячі тактів, але величезні розміри, від декількох гігабайт до декількох терабайт. 
●	Дисковий сховище - не один мільйон тактів, якщо дані не були закешірованни або забуферізованни заздалегідь, розміри до декількох терабайт
 
Загальна структура пам'яті ідентична Skylake.
■	Cache
■	L0 µOP cache:
■	1,536 µOPs, 8-way set associative
■	32 sets, 6-µOP line size
■	statically divided between threads, per core, inclusive with L1I
■	L1I Cache:
■	32 KiB, 8-way set associative
■	64 sets, 64 B line size
■	shared by the two threads, per core
■	L1D Cache:
■	32 KiB, 8-way set associative
■	64 sets, 64 B line size
■	shared by the two threads, per core
■	4 cycles for fastest load-to-use (simple pointer accesses)
■	5 cycles for complex addresses
■	64 B/cycle load bandwidth
■	32 B/cycle store bandwidth
■	Write-back policy
■	L2 Cache:
■	Unified, 256 KiB, 4-way set associative
■	Non-inclusive
■	1024 sets, 64 B line size
■	12 cycles for fastest load-to-use
■	64 B/cycle bandwidth to L1$
■	Write-back policy
■	L3 Cache/LLC:
■	Up to 2 MiB Per core, shared across all cores
■	Up to 16-way set associative
■	Inclusive
■	64 B line size
■	Write-back policy
■	Per each core:
■	Read: 32 B/cycle (@ ring clock)
■	Write: 32 B/cycle (@ ring clock)
■	42 cycles for fastest load-to-use
■	Side Cache:
■	64 MiB & 128 MiB eDRAM
■	Per package
■	Only on the Iris Pro GPUs
■	Read: 32 B/cycle (@ eDRAM clock)
■	Write: 32 B/cycle (@ eDRAM clock)
■	System DRAM:
■	2 Channels
■	8 B/cycle/channel (@ memory clock)
■	42 cycles + 51 ns latency
■	TLBs:
■	ITLB
■	4 KiB page translations:
■	128 entries; 8-way set associative
■	dynamic partitioning
■	2 MiB / 4 MiB page translations:
■	8 entries per thread; fully associative
■	Duplicated for each thread
■	DTLB
■	4 KiB page translations:
■	64 entries; 4-way set associative
■	fixed partition
■	2 MiB / 4 MiB page translations:
■	32 entries; 4-way set associative
■	fixed partition
■	1G page translations:
■	4 entries; 4-way set associative
■	fixed partition
■	STLB
■	4 KiB + 2 MiB page translations:
■	1536 entries; 12-way set associative
■	fixed partition
■	1 GiB page translations:
■	16 entries; 4-way set associative
■	fixed partition
RAM:
32GB (4x8GB) 2400MHz DDR4 CL12 XMP
SSD:
SSD 2.5" 128GB Patriot


4.	Чим відрізняється сторінкова організація віртуальної пам’яті від сегментної організації?
Основна відмінність між сторінковою організацією та сегментацією полягає в тому, що сторінка має фіксований розмір блоку, тоді як сегмент має змінний.
Сегментація дозволяє програмам, даним і кодам розбиватися на незалежні адресні простори. З іншого боку, підкачка сторінок використовується при отриманні великої лінійної адреси. 
Сегментація може розрізняти та захищати процедури та дані окремо, тоді як сторінкова організація не може розрізняти та захищати процедури та дані окремо.
Сегментація може призвести до зовнішньої фрагментації, оскільки пам'ять заповнюється блоками змінного розміру. Навпаки, підкачка сторінок може призвести до внутрішньої фрагментації, оскільки сторінка має фіксований розмір блоку, але може трапитися так, що процес не набуває всього розміру блоку, що генерує внутрішню фрагментацію в пам'яті.
При сегментації доступно багато незалежних адресних просторів, тоді як у підкачках адресний простір є одновимірним.
При сегментації розмір сегментованої сторінки визначається користувачем. З іншого боку, у підкачці - апаратним забезпеченням.
5.	Які розміри сторінок віртуальної пам’яті підтримує архітектура x86-64?
Наприклад, процесори x86 / x86-64 мають звичайний розмір сторінки 4 кБ, але вони також можуть використовувати сторінки розміром 4 МБ та 2 МБ відповідно. 
Однак використання великих розмірів сторінок призводить до деяких проблем. Області пам'яті, що використовуються для великих сторінок, повинні бути суміжними у фізичній пам'яті. Якщо розмір одиниці для адміністрування фізичної пам’яті збільшити до розміру сторінок віртуальної пам’яті, кількість витраченої пам’яті зростатиме. Усі види операцій з пам'яттю (наприклад, завантаження виконуваних файлів) вимагають вирівнювання до меж сторінки. Це в середньому означає, що кожне зіставлення витрачає половину розміру сторінки у фізичній пам’яті для кожного зіставлення. Ці відходи можна легко скласти; таким чином, це ставить верхню межу розумного розміру одиниці для розподілу фізичної пам'яті.
Звичайно не практично збільшувати розмір блоку до 2 МБ для розміщення великих сторінок на x86-64. Це просто занадто великий розмір. Але це в свою чергу означає, що кожна велика сторінка повинна складатися з багатьох менших сторінок. І ці маленькі сторінки повинні бути суміжними у фізичній пам’яті. Виділення 2 Мб суміжної фізичної пам’яті з розміром одиничної сторінки 4 кБ може бути складним завданням. Потрібно знайти вільну область із 512 суміжними сторінками. Це може бути надзвичайно складно (або неможливо), якщо система деякий час працює, а фізична пам’ять стає фрагментованою.
6.	Як здійснюється підкачка потрібного блоку даних із віртуальної пам’яті в кеш першого рівня?
 
На початку кожного звернення до кеш-пам'яті контролер спочатку зчитує комірку каталогу із даним індексом, порівнює біти тега зі старшими бітами адреси пам'яті та аналізує ознаку дійсності. Такий аналіз виконується в спеціальному циклі стеження, який ще називають циклом запиту. 
Якщо в результаті аналізу з'ясується, що потрібний блок не знаходиться в кеші, то генерується або продовжується цикл звернення до основної пам'яті (випадок кеш-промаху).  Коли ж є кеш-попадання, то запит обслуговується кеш-пам'яттю. У випадку кеш-промаху після зчитування основної пам'яті нові дані розміщуються в рядку кеша за умови, що він чистий, а в його тегу розташовуються старші біти адреси і встановлюється ознака дійсності даних. З основної пам'яті рядок переписується в кеш повністю, незалежно від обсягу даних, що вимагаються, оскільки ознака дійсності належить до всіх його байтів. Якщо контролер реалізує випереджаюче зчитування (read ahead), то в наступні вільні цикли шини поновиться і наступний рядок, якщо він був чистим. Читання “із запасом” дає змогу за необхідності здійснювати пакетний цикл зчитування з кеша через межу рядка.


7.	Як здійснюється звільнення блоку даних в кеші першого рівня для виконання наступної підкачки потрібного блоку з віртуальної пам’яті?
Щоб звільнити місце для нового запису при cache miss, можливо, кеш-пам'яті доведеться вилучити один із існуючих записів. Евристика, яку вона використовує для вибору місця виселення, називається політикою заміщення. 
Алгоритми заміни кешу часто спрямовані на мінімізацію різних параметрів такі як коефіцієнт звернень, коефіцієнт потрапляння байт, вартість доступу та затримка.
Найбільш широко використовувані алгоритми заміни кешу включають LRU,  LFU, LRU-Min, LRU-Threshold тощо.
LRU означає найменш нещодавно використаний, і ідея цієї політики полягає в тому, щоб замінити блоки, які менше використовуються в кеш-пам'яті. Після початкової точки, де будуть зайняті всі блоки, у випадку, якщо потрібно отримати доступ до нового блоку, і він відсутній у кеш-пам'яті, LRU поставить цей новий блок у положення найстарішого блоку без звернення. У разі потрапляння блок потрапляння переміщується в положення найновішого блоку. Основна відмінність LRU від FIFO - після потрапляння. FIFO не змінює порядок блоків у кеші, замість цього LRU ставить блок потрапляння в найновіше положення.
 
PLRU - одна з найпоширеніших політик заміни блоків в останніх поколіннях кеш-пам'яті. Широко використовується промисловістю, це найпоширеніша політика в комерційних продуктах AMD та Intel. Псевдо-LRU (також зване Tree-LRU) полягає у збереженні стану кеш-пам'яті у двійковому дереві. Це дерево вказує на те, яке місце кеш-пам'яті є блоком для виселення та заміни на випадок промаху.
 
8.	Що таке TLB буфери і яке їх призначення?
Віртуальна пам’ять мала б серйозний вплив на продуктивність, якщо б  вимагала читання таблиці сторінок при кожному завантаженні чи зберіганні.  На щастя, доступ до таблиці сторінок має велику часову локальність. Часова та просторова локальність доступу до даних та великий розмір сторінки означають, що багато послідовних завантажень або сховищ, ймовірно, посилаються на одну сторінку. Отже, якщо процесор запам'ятовує останній запис таблиці сторінок, який він прочитав, він, ймовірно, може повторно використовувати цей переклад, не перечитуючи таблицю сторінок. Загалом, процесор може зберігати останні записи таблиці на декількох сторінках у невеликому кеші, який називається буфером зовнішнього перекладу (TLB). 
TLB - це кеш, який використовується для зменшення часу, необхідного для доступу до місця користувальницької пам'яті, є частиною блоку управління пам'яттю мікросхеми (MMU). 
Процесор "дивиться в сторону", щоб знайти переклад у TLB перед тим, як отримати доступ до таблиці сторінок у фізичній пам'яті. У реальних програмах переважна більшість звернень потрапляє в TLB, уникаючи трудомістких читань таблиць сторінок з фізичної пам'яті.
TLB організований як повністю асоціативний кеш і, як правило, вміщує від 16 до 512 записів. Кожен запис TLB містить номер віртуальної сторінки та відповідний фізичний номер сторінки. Доступ до TLB здійснюється за номером віртуальної сторінки. Якщо TLB ‘hits’, він повертає відповідний фізичний номер сторінки. В іншому випадку процесор повинен прочитати таблицю сторінок у фізичній пам’яті. TLB розроблений для того, щоб бути достатньо малим аби отримати доступ до нього менш ніж за один цикл. Незважаючи на це, TLB, як правило, мають hit rate більше 99%. TLB зменшує кількість доступів до пам'яті, необхідних для більшості інструкцій із завантаження або зберігання, з двох до одного.
TLB на сучасних архітектурах Intel мають дворівневу ієрархію. Перший рівень (тобто L1) складається з двох частин, однієї, яка кешує переклади для кодових сторінок, що називається L1 instruction TLB (L1 iTLB), і однієї, яка кешує переклади для сторінок даних, що називається L1 data TLB (L1 dTLB). Другий рівень TLB (L2 sTLB) є більшим і використовується для перекладу як коду, так і даних. 
9.	Привести в протоколі на лабораторну роботу малюнок, що ілюструє роботу та рівень асоціативності якогось з TLВ буферів мікропроцесора робочої станції.
TLBs:
a.	ITLB
i.	4 KiB page translations:
1.	128 entries; 8-way set associative
2.	dynamic partitioning
ii.	2 MiB / 4 MiB page translations:
1.	8 entries per thread; fully associative
2.	Duplicated for each thread
b.	DTLB
i.	4 KiB page translations:
1.	64 entries; 4-way set associative
2.	fixed partition
ii.	2 MiB / 4 MiB page translations:
1.	32 entries; 4-way set associative
2.	fixed partition
iii.	1G page translations:
1.	4 entries; 4-way set associative
2.	fixed partition
c.	STLB
i.	4 KiB + 2 MiB page translations:
1.	1536 entries; 12-way set associative
2.	fixed partition
ii.	1 GiB page translations:
1.	16 entries; 4-way set associative
2.	fixed partition

128 entries; 8-way set associative
 

10.	Які умови повинні бути дотримані, щоб технологія NCQ (Native Command Queuing) працювала в робочих станціях?
Native Command Queuing (NCQ) - це розширення протоколу Serial ATA, що дозволяє жорстким дискам внутрішньо оптимізувати порядок виконання команд читання та запису. Щоб технологія NCQ працювала, вона повинна підтримуватися в хост-контролері / чипсеті, адаптері хост-шини SATA та на жорсткому диску.  Протокол може бути реалізований лише на власних жорстких дисках Serial ATA.
NCQ дозволяє одночасно виконувати декілька команд у приводі. Диски, що підтримують NCQ, мають внутрішню чергу, де команди можуть бути динамічно переплановані або переупорядковані. Диски, які підтримують NCQ здатні ефективно розподіляти операції читання та запису по доступних каналах, що може зменшити кількість непотрібних рухів головки накопичувача, що призводить до підвищення продуктивності для робочих навантажень, де неодноразові одночасні запити на читання / запис є найчастішими. Якщо поглянути узагальнено - NCQ покращує час доступу, швидкість довільного читання / запису і введення / виводу. 
Без підтримки NCQ команди надходять на контроллер диска, і виконуються ніяк не сортуючись, мають пріоритет тільки за часом надходження в контроллер. Якщо підтримується NCQ, то при надходженні команди оцінюється розташування даних на диску і пріоритет команд. Залежно від цього операції сортуються і виконуються.
