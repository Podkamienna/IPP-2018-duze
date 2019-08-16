//
// Created by alicja on 03.07.19.
//

#ifndef DROGI_DICTIONARY_H
#define DROGI_DICTIONARY_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Dictionary Dictionary;

/**
 * @brief Alokuje pamięć pod i zwraca wskaźnik
 * na nowy słownik
 * @return Zwraca wskaźnik na nowy słownik lub
 * NULL jeżeli się nie udało zaalokować pamięci
 */
Dictionary *initializeDictionary();

/**
 * @brief Funkcja wyszukująca w słowniku zadanej wartości
 * @param dictionary — słownik, w którym będzie szukane
 * @param name — nazwa do której przypisana wartość ma być znaleziona
 * @return Pole słownika lub NULL jeżeli któryś z parametrów jest
 * nieprawidłowy lub szukane pole nie istnieje
 */
void *searchDictionary(Dictionary *dictionary, const char *name);

/**
 * @brief Funkcja wkładająca do słownika nowy element
 * @param dictionary — słownik, do którego ma być wkładane
 * @param name — tekst, do którego ma być przypisana wkładana wartość
 * @param value — wartość do włożenia
 * @return Wartość @p true jeżeli udało się dodać wartość.
 * Wartość @p false jeżeli któryś z parametrów ma niepoprawną wartość,
 * nie udało się zaalokować pamięci lub tekst ma już przypisaną wartość
 * w słowniku.
 */
bool insertDictionary(Dictionary *dictionary, const char *name, void *value);

//TODO opis
size_t getId(Dictionary *dictionary);

/**
 * @brief Usuwa wartość przypisaną do zadanego tekstu z zadanego słownika
 * @param dictionary — słownik z którego ma być usuwane
 * @param name — tekst do którego przypisana wartość ma byś usuwana
 * @param deleteValue — funkcja usuwająca wartość
 */
void deleteFromDictionary(Dictionary *dictionary, const char *name, void deleteValue(void *));

/**
 * @brief Funkcja usuwająca słownik.
 * @param dictionary — słownik który ma być usunięty
 * @param deleteValue — funkcja usuwająca element ze słownika
 */
void deleteDictionary(Dictionary *dictionary, void deleteValue(void *));

#endif //DROGI_DICTIONARY_H
