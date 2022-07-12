
#include "words.h"

#include <QRandomGenerator>

Sieve::Sieve () {
    vowels.resize (33);
    thuds.resize (33);
    pairThuds.resize (33);
    ringi.resize (33);
    sonor.resize (33);

    vowels.fill (false);
    thuds.fill (false);
    ringi.fill (false);
    sonor.fill (false);

    vowels.setBit (0, true);
    vowels.setBit (5, true);
    vowels.setBit (6, true);
    vowels.setBit (9, true);
    vowels.setBit (15, true);
    vowels.setBit (20, true);
    vowels.setBit (28, true);
    vowels.setBit (30, true);
    vowels.setBit (31, true);
    vowels.setBit (32, true);

    thuds.setBit (11, true);
    thuds.setBit (16, true);
    thuds.setBit (18, true);
    thuds.setBit (19, true);
    thuds.setBit (21, true);
    thuds.setBit (22, true);
    thuds.setBit (23, true);
    thuds.setBit (24, true);
    thuds.setBit (25, true);
    thuds.setBit (26, true);

    pairThuds.setBit (11, true);
    pairThuds.setBit (16, true);
    pairThuds.setBit (18, true);
    pairThuds.setBit (19, true);
    pairThuds.setBit (21, true);
    pairThuds.setBit (25, true);

    ringi.setBit (1, true);
    ringi.setBit (2, true);
    ringi.setBit (3, true);
    ringi.setBit (4, true);
    ringi.setBit (7, true);
    ringi.setBit (8, true);

    sonor.setBit (10, true);
    sonor.setBit (12, true);
    sonor.setBit (13, true);
    sonor.setBit (14, true);
    sonor.setBit (17, true);
}

bool Sieve::isVowel (QChar c) {
    if (c.unicode () <= 1077) {
        return vowels.testBit (c.unicode () - 1072);
    }
    else {
        return vowels.testBit (c.unicode () - 1071);
    }
}

bool Sieve::isThud (QChar c) {
    if (c.unicode () <= 1077) {
        return thuds.testBit (c.unicode () - 1072);
    }
    else {
        return thuds.testBit (c.unicode () - 1071);
    }
}

bool Sieve::isPairThud (QChar c) {
    if (c.unicode () <= 1077) {
        return pairThuds.testBit (c.unicode () - 1072);
    }
    else {
        return pairThuds.testBit (c.unicode () - 1071);
    }
}

bool Sieve::isRingi (QChar c) {
    if (c.unicode () <= 1077) {
        return ringi.testBit (c.unicode () - 1072);
    }
    else {
        return ringi.testBit (c.unicode () - 1071);
    }
}

bool Sieve::isSonor (QChar c) {
    if (c.unicode () <= 1077) {
        return sonor.testBit (c.unicode () - 1072);
    }
    else {
        return sonor.testBit (c.unicode () - 1071);
    }
}

Syllable::Syllable () {
    pattern = "";
}

Syllable::Syllable (QString ptrn) {
    pattern = ptrn;
    letterFreq.resize (pattern.size ());
    for (int i = 0; i < letterFreq.size (); i ++) {
        letterFreq [i].resize (lettersCount + 1);
        letterFreq [i].fill (0);
    }
}

void Syllable::calculateLetterFrequency () {
    for (int i = 0; i < instances.size (); i ++) {
        for (int j = 0; j < pattern.size (); j ++) {
            QChar c = instances [i] [j];
            int normalCode;
            if (c.unicode () <= 1077) {
                normalCode = c.unicode () - 1072;
            }
            else {
                normalCode = c.unicode () - 1071;
            }
            ++ letterFreq [j] [normalCode];
        }
    }
    for (int i = 0; i < pattern.size (); i ++) {
        int total = 0;
        for (int j = 0; j < lettersCount; j ++) {
            total += letterFreq [i] [j];
        }
        letterFreq [i] [lettersCount] = total;
    }
}

void Syllable::addInstance (QString str) {
    instances.append (str);
}

int Syllable::length () {
    return pattern.size ();
}

QString Syllable::getPattern () {
    return pattern;
}

int Syllable::getSyllableFrequency () {
    return instances.size ();
}

QVector <int> & Syllable::getLetterFrequency (int idx) {
    return letterFreq [idx];
}

Filter::Filter () {}

bool Filter::check (QString word) {
    if (word.size () < 2) {
        return false;
    }
    if (word.size () == 2) {
        if (word [0] == word [1]) {
            return false;
        }
    }
    if (word.size () > 11) {
        return false;
    }
    Sieve s;
    if (s.isSonor(word [0]) && s.isSonor(word [1])) {
        return false;
    }
    // check if "ы/ь/ъ" is not in the beginning
    if ((QChar (word [0]).unicode () == 1100)
            || (QChar (word [0]).unicode () == 1101)
            || (QChar (word [0]).unicode () == 1102)) {
        return false;
    }
    // check if there are 3 vowels or 4 consonants
    int consCount = 0;
    int vowelsCount = 0;
    for (int i = 0; i < word.size (); i ++) {
        if (s.isVowel (word [i])) {
            consCount = 0;
            ++ vowelsCount;
        }
        else {
            vowelsCount = 0;
            ++ consCount;
        }
        if ((consCount == 4) || (vowelsCount == 3)) {
            return false;
        }
    }
    if (word.size () < 3) {
        return true;
    }
    // check if there are 2 same 2or3-letter syllables, or 3 same letters
    if ((word [0] == word [1]) && (word [0] == word [2])) {
        return false;
    }
    int sameLetterCount = 0;
    QString m1 = "0";
    for (int i = 0; i < word.size (); i ++) {
        if (word [i] == m1 [0]) {
            ++ sameLetterCount;
        }
        else {
            m1 [0] = word [i];
        }
        if (sameLetterCount == 3) {
            return false;
        }
    }
    if (word.size () < 4) {
        return true;
    }
    for (int i = 0; i < word.size (); i ++) {
        for (int j = i; j < word.size (); j += 2) {
            if (j + 4 > word.size ()) {
                break;
            }
            if (word.mid (j, 2) == word.mid (j + 2, 2)) {
                return false;
            }
            if ((word.mid (j, 2) == "ээ")
                    || (word.mid (j, 2) == "яя")
                    || (word.mid (j, 2) == "ьа")
                    || (word.mid (j, 2) == "ьу")
                    || (word.mid (j, 2) == "ьы")
                    || (word.mid (j, 2) == "ьэ")) {
                return false;
            }
            if ((word.mid (j + 2, 2) == "ээ")
                    || (word.mid (j, 2) == "яя")
                    || (word.mid (j, 2) == "ьа")
                    || (word.mid (j, 2) == "ьу")
                    || (word.mid (j, 2) == "ьы")
                    || (word.mid (j, 2) == "ьэ")) {
                return false;
            }
            if ((!s.isVowel (word [j])) && (word [j] == word [j + 2])) {
                return false;
            }
        }
    }
    if (word.size () < 6) {
        return true;
    }
    for (int i = 0; i < word.size (); i ++) {
        for (int j = i; j < word.size (); j += 3) {
            if (j + 6 > word.size ()) {
                break;
            }
            if (word.mid (j, 3) == word.mid (j + 3, 3)) {
                return false;
            }
        }
    }
    return true;
}

Reangor::Node::Node (int lp, int rp, int re, int num, bool fin) {
    left = lp;
    right = rp;
    ret = re;
    recognizedPatternNumber = num;
    final = fin;
}

void Reangor::analyze () {
    for (int i = 0; i < syllables.size (); i ++) {
        syllables [i].calculateLetterFrequency ();
    }
    int total = 0;
    for (int i = 0; i < syllCountMax; i ++) {
        total += syllCountFreq [i];
    }
    syllCountFreq [syllCountMax] = total;
    syllableFrequency.resize (syllables.size () + 1);
    total = 0;
    for (int i = 0; i < syllables.size (); i ++) {
        syllableFrequency [i] = syllables [i].getSyllableFrequency ();
        total += syllableFrequency [i];
    }
    syllableFrequency [syllables.size ()] = total;
}

int choose (QVector <int> & vec) {
    int randomNumber = QRandomGenerator::global ()->bounded (0, vec [vec.size () - 1]);
    int thrsh = 0;
    for (int i = 0; i < vec.size () - 1; i ++) {
        thrsh += vec [i];
        if (randomNumber < thrsh) {
            return i;
        }
    }
    return -1;
}

QString Reangor::generate () {
    QString result = "";
    int randSyllCount = choose (syllCountFreq);
    for (int i = 0; i < randSyllCount; i ++) {
        int randSyllNum = choose (syllableFrequency);
        for (int j = 0; j < syllables [randSyllNum].length (); j ++) {
            int randCode = choose (syllables [randSyllNum].getLetterFrequency (j));
            if (randCode <= 6) {
                randCode += 1072;
            }
            else {
                randCode += 1071;
            }
            result += QChar (randCode);
        }
    }
    result [0] = result [0].toUpper ();
    return result;
}

void Reangor::reset () {
    //
}

RAGConstantTheoretical::RAGConstantTheoretical () {
    syllables.append (Syllable ());
    syllables.append (Syllable ("1"));
    syllables.append (Syllable ("10"));
    syllables.append (Syllable ("01"));
    syllables.append (Syllable ("100"));
    syllables.append (Syllable ("001"));
    syllables.append (Syllable ("010"));
    syllables.append (Syllable ("0010"));
    syllables.append (Syllable ("0100"));
    nodes.resize (25);
    // third param: -1 - unsuccessful exit; -2 - successful exit.
    nodes [0] = Node (1, 2, -2, 0, false);
    nodes [1] = Node (3, 4, -1, 0, false);
    nodes [2] = Node (5, 6, -2, 1, false);
    nodes [3] = Node (7, 8, -1, 0, false);
    nodes [4] = Node (9, 10, -2, 3, false);
    nodes [5] = Node (11, 12, -2, 2, false);
    nodes [6] = Node (-1, -1, 2, 1, true);
    nodes [7] = Node (-1, -1, -1, 0, true);
    nodes [8] = Node (13, 14, -2, 5, false);
    nodes [9] = Node (15, 16, -2, 6, false);
    nodes [10] = Node (-1, -1, 2, 3, true);
    nodes [11] = Node (18, 17, -2, 4, false);
    nodes [12] = Node (-1, -1, 4, 1, true);
    nodes [13] = Node (-1, -1, 0, 7, true);
    nodes [14] = Node (-1, -1, 2, 5, true);
    nodes [15] = Node (19, 20, -2, 8, false);
    nodes [16] = Node (21, 22, 4, 3, false);
    nodes [17] = Node (-1, -1, 4, 2, true);
    nodes [18] = Node (24, 23, -1, 0, false);
    nodes [19] = Node (-1, -1, 1, 8, true);
    nodes [20] = Node (-1, -1, 4, 6, true);
    nodes [21] = Node (-1, -1, 5, 6, true);
    nodes [22] = Node (-1, -1, 10, 3, true);
    nodes [23] = Node (-1, -1, 8, 2, true);
    nodes [24] = Node (-1, -1, 3, 4, true);

    syllCountMax = 5;
    syllCountFreq.resize (syllCountMax + 1);
    syllCountFreq.fill (0);
}

bool RAGConstantTheoretical::recognize (QString word) {
    lastRecognition = " ";
    int syllCount = 0;
    int startIdx = 0;
    int endIdx = -1;
    int currentNodeIdx = 0;
    while (true) {
        if (endIdx != word.size () - 1) { // if we are not in the end of the word
            if (!nodes [currentNodeIdx].final) { // if node is not final
                ++ endIdx; // we will take next letter
                if (!sieve.isVowel (word [endIdx])) { // if letter is consonant
                    currentNodeIdx = nodes [currentNodeIdx].left; // we will go to left node
                }
                else { // if letter is vowel
                    currentNodeIdx = nodes [currentNodeIdx].right; // we will go to right node
                }
                // next cycle step
            }
            else { // if node is final
                if (nodes [currentNodeIdx].ret == -1) {
                    lastRecognition = word + " (" + lastRecognition + ")";
                    return false; // unsuccessful recognition
                }
                syllables [nodes [currentNodeIdx].recognizedPatternNumber].addInstance (
                            word.mid (startIdx, syllables [nodes [currentNodeIdx].recognizedPatternNumber].length ()));
                ++ syllCount;
                // syllable recognized. Adding new instance.
                lastRecognition += word.midRef (startIdx, syllables [nodes [currentNodeIdx].recognizedPatternNumber].length ()); // debug
                lastRecognition += '(' + syllables [nodes [currentNodeIdx].recognizedPatternNumber].getPattern () + ") "; // debug
                startIdx += syllables [nodes [currentNodeIdx].recognizedPatternNumber].length ();
                currentNodeIdx = nodes [currentNodeIdx].ret; // go to the "return" node
            }
        }
        else { // if we are in the end of the word
            if (currentNodeIdx == 0) {
                ++ syllCountFreq [syllCount];
                return true; // recognition successful
            }
            if (nodes [currentNodeIdx].ret == -1) {
                lastRecognition = word + " (" + lastRecognition + ")";
                return false; // unsuccessful recognition
            }
            syllables [nodes [currentNodeIdx].recognizedPatternNumber].addInstance (
                        word.mid (startIdx, syllables [nodes [currentNodeIdx].recognizedPatternNumber].length ()));
            ++ syllCount;
            // syllable recognized. Adding new instance.
            lastRecognition += word.midRef (startIdx, syllables [nodes [currentNodeIdx].recognizedPatternNumber].length ()); // debug
            lastRecognition += '(' + syllables [nodes [currentNodeIdx].recognizedPatternNumber].getPattern () + ") "; // debug
            startIdx += syllables [nodes [currentNodeIdx].recognizedPatternNumber].length ();
            if (nodes [currentNodeIdx].ret == - 2) {
                ++ syllCountFreq [syllCount];
                return true; // recognition successful
            }
            else {
                currentNodeIdx = nodes [currentNodeIdx].ret; // go to the "return" node
            }
        }
    }
}

RAGAdaptiveRussianSyllables::RAGAdaptiveRussianSyllables () {
    syllables.append (Syllable ());
    syllCountMax = 15;
    syllCountFreq.resize (syllCountMax + 1);
    syllCountFreq.fill (0);
}

void RAGAdaptiveRussianSyllables::addSyllable (QString str) {
    lastRecognition += ' ' + str;
    QString syllPattern = "";
    // making pattern from syllable <str>
    for (int i = 0; i < str.size (); i ++) {
        if (sieve.isVowel (str [i])) {
            syllPattern += '1';
        }
        else {
            syllPattern += '0';
        }
    }
    // trying to find this pattern
    for (int i = 0; i < syllables.size (); i ++) {
        if (syllPattern == syllables [i].getPattern ()) { // if we find it,
            syllables [i].addInstance (str);              // we'll add new instance
            return;
        }
    }
    // if such pattern is new
    syllables.append (syllPattern); // we'll add this pattern
    syllables [syllables.size () - 1].addInstance (str); // and new instance
}

bool RAGAdaptiveRussianSyllables::recognize (QString word) {
    // to know what idx last vowel has
    int lastVowelIdx = -1;
    for (int i = word.size () - 1; i >= 0; i --) {
        if (sieve.isVowel (word [i])) {
            lastVowelIdx = i;
            break;
        }
    }
    if (lastVowelIdx == -1) {
        return false; // this word has no vowels? Cannot work with it
    }

    lastRecognition = "";
    int syllCount = 0;
    int startIdx = 0;
    int endIdx = 0;
    while (true) {
        if (!sieve.isVowel (word [endIdx])) { // if letter is consonant
            ++ endIdx; // to the next letter
            // new cycle step
        }
        else { // if letter is vowel
            if (endIdx == lastVowelIdx) {  // if it is last vowel in the word
                this->addSyllable (word.mid (startIdx, word.size () - startIdx)); // add remaining letters to the last syllable
                ++ syllCount;
                ++ syllCountFreq [syllCount];
                return true; // recognition successful
            }
            if (QChar (word [endIdx + 1]).unicode () == 1081) { // if next letter is short-i
                if (!sieve.isVowel (word [endIdx + 2])) { // if letter after short-i is consonant
                    this->addSyllable (word.mid (startIdx, endIdx + 2 - startIdx)); // add short-i to prev syll
                    ++ syllCount;
                    endIdx += 2; // continue with last consonant
                    startIdx = endIdx;
                    continue; // new cycle step
                }
            }
            else if (sieve.isSonor (word [endIdx + 1])) { // if next letter is russian l/m/n/r
                int shift = 2;
                if (QChar (word [endIdx + 2]).unicode () == 1100) { // if letter after it is "мягкий знак"
                    ++ shift;
                }
                if (sieve.isPairThud (word [endIdx + shift])
                        || sieve.isRingi (word [endIdx + shift])) { // and letter after it is pair consonant
                    this->addSyllable (word.mid (startIdx, endIdx + shift - startIdx)); // add l/m/n/r to prev syll
                    ++ syllCount;
                    endIdx += shift; // continue with last consonant
                    startIdx = endIdx;
                    continue; // new cycle step
                }
            }
            // if there are no exceptions
            this->addSyllable (word.mid (startIdx, endIdx - startIdx + 1)); // vowel ends current syllable
            ++ syllCount;
            ++ endIdx;
            startIdx = endIdx;
        }
    }
}
