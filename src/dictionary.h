/** @file
 * Interfejs klasy będącej słownikiem.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 05.07.2019
 */

#ifndef DROGI_DICTIONARY_H
#define DROGI_DICTIONARY_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * Słownik.
 */
typedef struct Dictionary Dictionary;

/**
 * @brief Alokuje pamięć pod i zwraca wskaźnik
 * na nowy słownik
 * @return Zwraca wskaźnik na nowy słownik lub
 * NULL jeżeli się nie udało zaalokować pamięci
 */
Dictionary *initializeDictionary();

/**
 * @brief Funkcja usuwająca słownik.
 * @param dictionary — słownik który ma być usunięty
 * @param deleteValue — funkcja usuwająca element ze słownika
 */
void deleteDictionary(Dictionary *dictionary, void deleteValue(void *));

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

/**
 * @brief Zwraca liczbę oznaczającą ilość elementów w słowniku.
 * @param dictionary — słownik z którego informacje będą uzyskane
 * @return Wartość @p 0 gdy słownik jest NULLem lub ilość elementów
 * zawartych w słowniku.
 */
size_t getId(Dictionary *dictionary);


#endif //DROGI_DICTIONARY_H
