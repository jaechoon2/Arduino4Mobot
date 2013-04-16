Repozytorium zawiera przykładową konfigurację środowiska Arduino, przystosowaną
do programowania platformy mobilnej robota Explorer A1, opartej na mikrokontrolerze
ATmega128. Jakkolwiek konfiguracja powstała specyficznie pod ten mikrokontroler oraz
pod platformę Explorer A1, może być użyta także z innymi typami mikrokontrolerów Atmel AVR,
o ile definiują one podobny zestaw zasobów sprzętowych jak w mikrokontrolerach użytych
we wszystkich modelach Arduino.
W repozytorium znajduje się narzędzie, które w prosty sposób umożliwia konwersję mapy
pinów mikrokontrolera na piny Arduino, zapisanej w arkuszu kalkulacyjnym Libre/OpenOffice na
nagłówek "pins_arduino.h".

wymagania
------------
* do uruchomienia narzędzia konwersji, wymaganay jest python przynajmniej w wersji 2.7
* potrzebny jest także moduł pythona odfpy.
* Arduino IDE w wersji przynajmniej 1.0.1
* całość repozytorium należy umieścić w katalogu sketchbook, znajdującym się w katalogu
  domowym użytkownika.
* do programowania robota, wymagany jest programator SPI AVR, można użyć każdego dopuszczalnego
  dla avrdude programatora (na przykład USBasp). Opcjonalnie, można wgrać do Mobot Explorer załączony
  bootloader i za pomocą konwertera USB-USART, programować Mobot Explorer jak zwykłe Arduino. Ważne
  w tym przypadku jest posiadanie przez kowerter linii DTR(CTS), która podłączona do linii reset mikrokontrolera
  służy do resetowania mikrokontrolera oraz uruchamiania bootloadera.
  Będąc jednak w posiadaniu programatora SPI dla AVR, można programować bez użycia bootladera, przy okazji
  zyskując dodatkowe 4KB pamięci flash na własny program. Należy pamiętać, że programowanie z użyciem programatora
  wymaga wybrania opcji "Załaduj z użyciem programatora" z menu Plik Arduino, zamiast standardowej opcji z przycisku.


użytkowanie środowiska Arduino
------------
* przejść do katalogu ~/sketchbook (stworzyć taki katalog, jeżeli nie istnieje)
* wykonać git clone https://github.com/lucidm/Arduino4Mobot.git

użytkowanie narzędzia odstopins.py
------------
* narzędzie znajduje się w katalogu hardware/arduino/tools
* uruchomienie narzędzia ./odstopins.py -i Arduino-Mobot.ods -o pins_arduino.h -s Mobot -m ATmega128
  spowoduje wygenerowanie pliku pins_arduino.h w aktualnym katalogu. Plik wygenerowany będzie na podstawie
  pliku arkusza Arduino-Mobot.ods z zakładki Mobot tego arkusza dla mikrokontrolera ATmega128. Opcje -i, -s oraz -m
  są wymagane.
* plik pins_arduino.h służy do mapowania pinów Arduino na piny mikrokontrolera. Opis sposobu w jaki Arduino IDE korzysta
  z tego pliku można znaleźć na przykład na stronie:
  http://urbanhonking.com/ideasfordozens/2009/05/18/an_tour_of_the_arduino_interna/

przykładowy szkic Arduino
------------
* po wgraniu kopii repozytorium do katalogu ~/sketchbook
* uruchomić Arduino
* w menu "Narzędzia" programu Arduino wybrać Płytka->Mobot A1 Explorer
* w menu "Plik" wybrać Szkicowanik->Mobot aby wgrać przykładowy szkic.
* podłączyć złącze STK200 robota do programatora za pomocą 10 pinowego paska,
  pamiętając jednocześnie o wyjęciu wcześniej modułu radiowego z JP6 na czas
  programowania. W przeciwnym wypadku, programator oraz moduł radiowy, korzystający
  z tych samych linii procesora, mogą siebie wzajemnie zakłócać, w efekcie może nie dać
  się poprawnie zaprogramować mikrokontroler.
* po wgraniu szkicu wcisnąć skrót klawiaturowy CTRL+Shift+U, co spowoduje skompilowanie szkicu
  oraz załadowanie go do mikrokontrolera za pomocą programatora. Po prawidłowym zaprogramowaniu
  robota i uruchomieniu na naładowanych akumulatorach, powinny ruszyć oba silniki, dodatkowo dioda
  LED 3 będzie się zapalać, jeżeli prąd pobierany przez którykolwiek zespół silników będzie większy
  niż 600mA.
* mapowanie pinów mikrokontrolera na piny arduino oraz piny na pinheaderach robota zdefiniowany jest
  w pliku hardware/arduino/tools/Arduino-Mobot.ods. Pierwsza kolumna określa numery pinów użytych jako
  nuery pinów w Arduino.
* opis elektroniki platformy mobilnej Mobot Explorer A1. Znajduje się na stronie:
  http://mobot.pl/index.php?site=products&type=853&details=8049

przetestowane biblioteki i zasoby sprzętowe Arduino działające z platformą
------------
* Serial,
* Serial1,
* LiquidCrystal,
* Timery
* PWM
* I2C
* SPI

Dodatkowo w podkatalogu libraries znajduje się zestaw bibliotek do obsługi silników napędowych platformy,
modułu radiowego, odczytu sonarowych czujników odległości oraz dokonywania pomiarów prądu pobieranego przez
silniki i napięcia baterii akumulatorów. Biblioteki dodaje się do szkicu wybierając w Arduino menu
Szkic->Importuj bibliotekę->Mobot. Przykład wykorzystania znajduje się w szkicu Mobot.

