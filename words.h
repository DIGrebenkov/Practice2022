#ifndef WORDS_H
#define WORDS_H

#include <QString>
#include <QStringList>
#include <QBitArray>
#include <QVector>

#include <QDebug> // debug

class Sieve {
private:
    QBitArray vowels;
    QBitArray thuds;
    QBitArray pairThuds;
    QBitArray ringi;
    QBitArray sonor;

public:
    Sieve ();
    bool isVowel (QChar c);
    bool isThud (QChar c);
    bool isPairThud (QChar c);
    bool isRingi (QChar c);
    bool isSonor (QChar c);
};

class Syllable {
private:
    const int lettersCount = 33;
    QString pattern;
    QStringList instances;
    QVector <QVector <int>> letterFreq;
public:
    Syllable ();
    Syllable (QString ptrn);
    int length ();
    void addInstance (QString str);
    QString getPattern ();
    int getSyllableFrequency ();
    void calculateLetterFrequency ();
    QVector <int> & getLetterFrequency (int idx);
};

class Filter
{
public:
    Filter ();
    bool check (QString str);
};

// recognizer, analyzer, generator
class Reangor {
protected:
    struct Node {
        int left;
        int right;
        int ret;
        int recognizedPatternNumber;
        bool final;
        Node () {}
        Node (int lp, int rp, int re, int num, bool fin);
    };
    QVector <Node> nodes;
    QVector <Syllable> syllables;
    QVector <int> syllCountFreq;
    int syllCountMax;
    QVector <int> syllableFrequency;
public:
    Sieve sieve;
    Filter filter;
    QString lastRecognition; // debug
    virtual bool recognize (QString word) = 0;
    virtual void analyze ();
    virtual QString generate ();
    void reset ();
    virtual ~Reangor () {};
};

class RAGConstantTheoretical: public Reangor {
public:
    RAGConstantTheoretical ();
    bool recognize (QString word) override;
};

class RAGAdaptiveRussianSyllables: public Reangor {
private:
    void addSyllable (QString str);
public:
    RAGAdaptiveRussianSyllables ();
    bool recognize (QString word) override;
};

#endif // WORDS_H
