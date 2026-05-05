# Miniprojekt 2 - Algorytmy grafowe

Projekt realizuje implementację grafów ważonych wraz z algorytmami wyszukiwania najkrótszej ścieżki (ścieżki o najmniejszej wadze)

## Spis treści
* [Wymagania](#1-wymagania)
* [Kompilacja i uruchomienie](#2-kompilacja-i-uruchomienie)
* [Struktura Projektu](#3-struktura-projektu)
* [Szczegółowy Opis Plików i Modułów](#4-szczegółowy-opis-plików-i-modułów)

## 1. Wymagania
* **Kompilator:** GCC 15+ / Clang 17+ (wymagane wsparcie dla C++20)
* **System budowania:** CMake 3.10+
* **System operacyjny:** macOS / Windows / Linux

## 2. Kompilacja i uruchomienie
Aby zbudować projekt, wykonaj poniższe kroki w terminalu:

1. Stwórz folder budowania:
   ```bash
   mkdir build && cd build
   ```

2. Skonfiguruj projekt za pomocą CMake:
   ```bash
   cmake ..
   ```

3. Skompiluj:
   ```bash
   cmake --build .
   ```

4. Uruchom program:
   ```bash
   ./mini_projekt_1
   ```

## 3. Struktura Projektu

```text
├── src/
│   ├── main.cpp
│   ├── graph_list.hpp
│   ├── graph_list.cpp
│   ├── graph_matrix.hpp
│   ├── graph_matrix.cpp
│   ├── menu.hpp
│   ├── data_prepare.hpp
│   └── data_prepare.cpp
├── data/
│   └── main_seed/
│       ├──g00.csv
│       ├── ...
│       └──g99.csv
├── build/
│   ├── binary
│   └── ...
├── CMakeLists.txt
└── README.md
```

## 4. Szczegółowy Opis Plików i Modułów


### A. Moduły implementacyjne

#### `graph_list`
`graph_list.hpp` / `graph_list.cpp` – Implementacja grafu jako listy sąsiedztwa oraz algorytmów Dijkstry i Bellmana-Forda do wyszukiwania najkrótszej ścieżki z zadanego wierzchołka do innych jako metody klasy.

#### `graph_matrix`
`graph_matrix.hpp` / `graph_matrix.cpp` – Implementacja grafu jako macierzy sąsiedztwa oraz algorytmów Dijkstry i Bellmana-Forda do wyszukiwania najkrótszej ścieżki z zadanego wierzchołka do innych jako metody klasy.


### B. Logika Aplikacji i Interfejs Użytkownika

#### `Menu`
`Menu.hpp` / `Menu.cpp` – Warstwa prezentacji. Moduł odpowiada za wyświetlanie opcji w konsoli, walidację danych wprowadzanych przez użytkownika oraz wywoływanie odpowiednich metod dla danego grafu. Pozwala na manualne testowanie funkcjonalności.

#### `main.cpp`
Minimalistyczny punkt startowy programu. Odpowiada za stworzenie instancji klasy `Menu` i uruchomienie jej głównej pętli. Nie zawiera logiki biznesowej ani zmiennych globalnych.

### D. Pozostałe Elementy Projektu

#### `data/` (Katalog danych)
Folder przechowujący fizyczne pliki `.txt` (np. `random_data_5000.txt`). Pliki te stanowią stałą bazę testową, gwarantując, że każda struktura jest badana na dokładnie tym samym zestawie danych.

#### `build/` (Katalog kompilacji)
Katalog roboczy CMake. Przechowuje plik wykonywalny (**binary**) oraz pliki pośrednie (`.o`, `CMakeFiles`). Katalog ten jest zazwyczaj ignorowany przez system kontroli wersji Git.

#### `CMakeLists.txt` (Konfiguracja)
Plik tekstowy sterujący procesem kompilacji. Określa nazwę projektu, flagi kompilatora, standard **C++20** oraz instrukcje kopiowania folderu `data/` do lokalizacji pliku wykonywalnego.

#### `README.md` (Dokumentacja)
Główny plik informacyjny projektu. Zawiera instrukcje instalacji, kompilacji, opis struktury oraz cele badawcze projektu.
