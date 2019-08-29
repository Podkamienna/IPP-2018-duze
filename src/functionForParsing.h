/** @file
 * Interfejs modułu z funkcjami używanymi do parsowania wejścia.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 25.08.2019
 */

#ifndef DROGI_FUNCTION_FOR_PARSING_H
#define DROGI_FUNCTION_FOR_PARSING_H

#include "vector.h"

/**
 * Enumerator określający typ wejścia,
 */
typedef enum InputType InputType;

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map Map;

enum InputType {
    INVALID, ///< wejście w którym jest błąd
    IGNORE, ///< wejście, które ma być ignorowane
    CORRECT ///< poprawne wejście
};

/**
 * @brief Sprawdza poprawność danych wejściowych.
 * @param string — napis do sprawdzenia poprawności
 * @param stringLengthPtr — wskaźnik na długość sprawdzanego napisu. Jeżeli napis
 * będzie poprawny, to po wykonaniu funkcji wartość zostanie zmniejszona o 1. Zakładam,
 * że nie jest NULLem.
 * @return Wartość @p INVALID, jeżeli napis zawiera znak '/0' przed znakiem '/n'
 * lub jest NULLem. Wartość @p IGNORE, jeżeli pierwszy znak napisu to '\n' lub '#'.
 * Wartość @p correct w przeciwnym razie.
 */
InputType checkInputCorrectness(char *string, size_t *stringLengthPtr);

/**
 * @brief Rozdziela zadany napis na wektor napisów, oddzielając
 * je w miejscach ze znakiem DELIMITER.
 * @param splittedInput — wektor, w którym po wykonaniu funkcji
 * mają się znaleźć rozdzielone napisy
 * @param stringToSplit — napis, który ma zostać podzielony na mniejsze
 * @return Wartość @p true, jeżeli wszystko się dało. Wartość @p false, jeżeli
 * któryś z parametrów jest NULLem.
 */
bool split(Vector *splittedInput, char *stringToSplit);

/**
 * @brief Dopasowuje wektor napisów do funkcji z modułu map i ją
 * wywołuje z odpowiednimi parametrami.
 * @param map — mapa, na której ma być wywołana funkcja.
 * @param splittedVector — rozdzielony wektor
 * @return Wartość @p true, jeżeli wszystko się powiodło. Wartość @p false,
 * jeżeli nie udało się dopasować wejścia do funkcji, długość wektora z argumentami nie
 * odpowiednia dla dopasowanej funkcji, któryś z argumentów nie był poprawny, nie udało się
 * zaalokować pamięci,
 */
bool execute(Map *map, Vector *splittedVector);

#endif //DROGI_FUNCTION_FOR_PARSING_H
