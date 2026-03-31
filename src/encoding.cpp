#include <QString>

namespace encoding {

const QString alphabet = "_АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

size_t get_char_idx(QChar c) { return alphabet.indexOf(c); }

QString encode(const QString& input_text, int key) {
    QString result;
    result.reserve(input_text.size());

    for (QChar c : input_text) {
        size_t idx = (get_char_idx(c) + key) % alphabet.size();
        result.push_back(alphabet.at(idx));
    }

    return result;
}

QString decode(const QString& input_text, int key) {
    QString result;
    result.reserve(input_text.size());

    for (QChar c : input_text) {
        int idx = static_cast<int>(get_char_idx(c)) - key;
        if (idx < 0) {
            idx = alphabet.size() - idx * -1 % alphabet.size();
        }
        result.push_back(alphabet.at(idx));
    }

    return result;
}

};  // namespace encoding
