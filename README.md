#LED-TISCH

Materialien:
============

-   IKEA Lack Tisch

-   Plexiglas halbdurchsichtig (milchig)

-   Arduino Nano (clone / original)

-   HC-05

-   WS2812 oder WS2811 streifen (5m / 150 LED )

-   Holz oder Pappefür Matrix

-   Taster mit LED (5V)

-   Netzteil 5V 10A

-   Arduino SD reader

-   SD Karte

-   Wiederstand (300 -- 500 Ohm)


Aufbau:
=======

-   Loch in Tischplatte sägen (mindestens 40x40cm)

-   Löscher an den Seiten in Tischplatte sägen für Netzteil, Taster, SD

-   Plexiglas passend zusägen

-   LED Streifen in Stücke zu je 12 LEDs Schneiden (12stck)

-   LED Streifen mit Kabeln passend zusammenlöten

-   Matrix aus Holz oder Pappe bauen (Matrix.dxf für CNC, ansonsten von
    Hand)(22stck)

-   LED Streifen an zusammengesteckte Matrix kleben, dabei die
    Datenrichtung beachten (Heißkleber) (S-Layout verwenden (siehe unter
    Bilder layout.png))

-   Arduino anschließen (siehe unter Bildern schaltplan.png bzw
    Schaltplan.fzz)

    -   HC-05:

        -   State an D7

        -   RXD an D6

        -   TXD an D5

        -   GND an GND

        -   VCC an 5V

    -   SD:

        -   Siehe Bild SD.png

    -   Taster:

        -   GND der LED an GND

        -   C (ein pol des Tasters) an GND

        -   NO (ungedrückt offen) an D8

    -   WS2812:

        -   GND an GND(Netzteil)

        -   5V / VCC an 5V(Netzteil)

        -   Din / Data an D3

    -   Netzteil:

        -   GND an Arduino und WS2812

        -   5V an Arduino und WS2812

-   Zum Testen der LEDs und der SDs die Beispielprogramme der SD und
    FastLED Bibilothek verwenden

	
Inbetriebnahme:
===============

-   Arduino mit Code Flashen (LED-Tisch.ino)

-   Glediator oder Jinx! Herunterladen

-   Bootanimation aufnehmen und unter dem Namen Boot.dat abspeichern

-   Weiter Animationen unter anderem Namen auf der SD speichern

-   Filemenue.bat ausführen (unter linux in filemenue.sh umbenennen)

-   App auf android gerät installieren

-   In den Geräteeinstellungen mit dem Gerät HC-05 verbinden.


Animation abspielen:
====================

-   In der App das Gerät auswählen und auf Verbinden gehen.

-   Passendes Programm auswählen

-   Auf Ausführen klicken


Geschwindigkeit ändern:
=======================

-   In der App das Gerät auswählen und auf Verbinden gehen.

<!-- -->

-   Den Regler unter Geschwindigkeit verstellen, oder

-   In der Textbox eine Geschwindigkeit zwischen 1 und 40 FPS (frames
    per second) eingeben

-   Auf OK klicken


Programm im Loop abspielen:
===========================

-   Darauf achten, dass kein Programm läuft UND ein gerät über bluetooth
    verbunden ist.

-   Den Taster drücken

-   Die LED sollte leuchten

-   Ansonsten nochmal versuchen

-   Programm ausführen (App)

-   Zum Verlassen des Loop modes den Taster drücken

-   Die LED sollte ausgehen.

