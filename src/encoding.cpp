#include <QString>

namespace encoding {

const QString alphabet = "_АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЭЮЯ";

const int K = 5;

size_t get_char_idx(QChar c) { return alphabet.indexOf(c); }

QString encode(const QString& input_text) {
    QString result;
    result.reserve(input_text.size());

    for (QChar c : input_text) {
        result.push_back(alphabet.at(get_char_idx(c) + K % alphabet.size()));
    }

    return result;
}

QString decode(const QString& input_text) {
    QString result;
    result.reserve(input_text.size());

    for (auto c : input_text) {
        result.push_back(alphabet.at(get_char_idx(c) - K % alphabet.size()));
    }

    return result;
}

};  // namespace encoding
