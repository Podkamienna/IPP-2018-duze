/** @file
 * Interfejs modułu do znajdowania najkrótszej ścieżki.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 22.08.2019
 */

#ifndef DROGI_FIND_PATH_H
#define DROGI_FIND_PATH_H

#include "definitions.h"
#include "list.h"

#include <stdbool.h>

/**
 * Struktura z dystansem pomiędzy dwoma miastami.
 */
typedef struct Distance Distance;

/**
 * Struktura przechowująca wyniki znajdowania najkrótszej
 * ścieżki między dwoma miastami.
 */
typedef struct FindPathResult FindPathResult;

/**
 * @brief Alokuje pamięć pod i zwraca wskaźnik na nową strukturę
 * typu FindPathResult.
 * @return Wartość @p NULL jeżeli nie udało się zaalokować
 * pamięci. Wskaźnik na nową strukturę w przeciwnym razie.
 */
FindPathResult *getNewFindPathResult();

/**
 * @brief Usuwa zadaną strukturę.
 * @param findPathResult — struktura do usunięcia
 */
void deleteFindPathResult(FindPathResult *findPathResult);

/**
 * @brief Sprawdza, czy zadana struktura jest poprawna.
 * @param findPathResult — struktura do sprawdzenia poprawności
 * @return Wartość @p false, jeżeli zadana struktura jest NULLem
 * lub nie istnieje jednoznaczna ścieżka.
 */
bool isCorrectPathResult(FindPathResult *findPathResult);

/**
 * @brief Konwertuje FindPathResult do drogi krajowej.
 * @param findPathResult — struktura do przekonwertowania
 * @return Wartość @p NULL, jeżeli nie udałó się zaalokować pamięci
 * lub parametr był NULLem, lub odpowiednia droga krajowa.
 */
Route *findPathResultToRoute(FindPathResult *findPathResult);

/**
 * @brief Konwertuje FindPathResult do ścieżki.
 * @param findPathResult — struktura do przekonwertowania
 * @return Wartość @p NULL, jeżeli nie udałó się zaalokować pamięci
 * lub parametr był NULLem, lub odpowiednia ścieżka.
 */
List *findPathResultToPath(FindPathResult *findPathResult);

/**
 * @brief Funkcja porównująca struktury typu FindPathResult
 * @param findPathResult1 — pierwsza struktura do porównania (nie może być NULLem)
 * @param findPathResult2 — druga struktura do porównania (nie może być NULLem)
 * @return Wartość dodatnią jeżeli pierwsza struktura jest większa, 0 jeżeli są równe
 * coś ujemnego jeżeli druga jest większa.
 */
int compareFindPathResults(FindPathResult *findPathResult1, FindPathResult *findPathResult2);

/**
 * @brief Znajduje najkrótszą ścieżkę pomiędzy zadanymi miastami, nie przechodzącą
 * przez rzeczy z zadanej listy.
 * @param map — mapa w której ścieżka jest wyszukiwana
 * @param source — miasto początkowe
 * @param destination — miasto końcowe
 * @param restrictedPath — lista rzeczy na które nie wolno wchodić
 * @return Wartość @p NULL jeżeli nie udało się zaalokować pamięci,
 * strukturę z polem path ustawionym na NULL, jeżeli nie udało się jednoznacznie
 * wyznaczyć ścieżki, z polem isUnique ustawionym na false jeżeli ścieżka jest niejednoznaczna,
 * a z polem isUnique ustawionym na true jeżeli nie udało się znaleźć żadnej.
 */
FindPathResult *findPath(Map *map, City *source, City *destination, List *restrictedPath);

#endif //DROGI_FIND_PATH_H
